#include "student_repository.hpp"

#include <sqlite3.h>

#include <cstdlib>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
[[noreturn]] void throw_database_error(sqlite3* database, const std::string& action) {
    const std::string detail = database ? sqlite3_errmsg(database) : "unknown SQLite error";
    throw std::runtime_error(action + ": " + detail);
}

class Connection {
public:
    explicit Connection(const std::string& path) {
        const int result = sqlite3_open_v2(
            path.c_str(),
            &database_,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
            nullptr
        );
        if (result != SQLITE_OK) {
            const std::string detail = database_ ? sqlite3_errmsg(database_) : "could not allocate database handle";
            if (database_) sqlite3_close(database_);
            database_ = nullptr;
            throw std::runtime_error("Could not open SQLite database: " + detail);
        }

        sqlite3_busy_timeout(database_, 5000);
        execute("PRAGMA foreign_keys = ON;");
    }

    ~Connection() {
        if (database_) sqlite3_close(database_);
    }

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    sqlite3* get() const { return database_; }

    void execute(const char* sql) const {
        char* error_message = nullptr;
        const int result = sqlite3_exec(database_, sql, nullptr, nullptr, &error_message);
        if (result != SQLITE_OK) {
            const std::string detail = error_message ? error_message : sqlite3_errmsg(database_);
            sqlite3_free(error_message);
            throw std::runtime_error("SQLite statement failed: " + detail);
        }
    }

private:
    sqlite3* database_{nullptr};
};

using Statement = std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>;

Statement prepare(sqlite3* database, const char* sql) {
    sqlite3_stmt* raw_statement = nullptr;
    if (sqlite3_prepare_v2(database, sql, -1, &raw_statement, nullptr) != SQLITE_OK) {
        throw_database_error(database, "Could not prepare SQLite statement");
    }
    return Statement(raw_statement, sqlite3_finalize);
}

void check_bind(sqlite3* database, int result) {
    if (result != SQLITE_OK) throw_database_error(database, "Could not bind SQLite parameter");
}

void check_step(sqlite3* database, int result, const std::string& action) {
    if (result != SQLITE_DONE) throw_database_error(database, action);
}

Student read_student(sqlite3_stmt* statement) {
    const auto* name = sqlite3_column_text(statement, 1);
    const auto* major = sqlite3_column_text(statement, 3);

    return {
        sqlite3_column_int(statement, 0),
        name ? reinterpret_cast<const char*>(name) : "",
        sqlite3_column_int(statement, 2),
        major ? reinterpret_cast<const char*>(major) : "",
        sqlite3_column_double(statement, 4)
    };
}

void bind_student(sqlite3* database, sqlite3_stmt* statement, const Student& student) {
    check_bind(database, sqlite3_bind_text(statement, 1, student.name.c_str(), -1, SQLITE_TRANSIENT));
    check_bind(database, sqlite3_bind_int(statement, 2, student.age));
    check_bind(database, sqlite3_bind_text(statement, 3, student.major.c_str(), -1, SQLITE_TRANSIENT));
    check_bind(database, sqlite3_bind_double(statement, 4, student.gpa));
}

constexpr const char* schema = R"sql(
CREATE TABLE IF NOT EXISTS students (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL CHECK(length(trim(name)) BETWEEN 1 AND 100),
    age INTEGER NOT NULL CHECK(age BETWEEN 1 AND 120),
    major TEXT NOT NULL CHECK(length(trim(major)) BETWEEN 1 AND 100),
    gpa REAL NOT NULL CHECK(gpa >= 0.0 AND gpa <= 4.0),
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE INDEX IF NOT EXISTS idx_students_name ON students(lower(name));
CREATE INDEX IF NOT EXISTS idx_students_major ON students(lower(major));
)sql";
} // namespace

StudentRepository::StudentRepository(std::string database_path)
    : database_path_(std::move(database_path)) {
    if (database_path_.empty()) {
        const char* configured_path = std::getenv("STUDENT_DB_PATH");
        database_path_ = configured_path && *configured_path
            ? configured_path
            : "data/students.db";
    }

    if (database_path_ != ":memory:") {
        const std::filesystem::path path(database_path_);
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }
    }

    Connection connection(database_path_);
    connection.execute(schema);
}

std::vector<Student> StudentRepository::list() const {
    Connection connection(database_path_);
    auto statement = prepare(
        connection.get(),
        "SELECT id, name, age, major, gpa FROM students ORDER BY id DESC"
    );

    std::vector<Student> students;
    int result = SQLITE_OK;
    while ((result = sqlite3_step(statement.get())) == SQLITE_ROW) {
        students.push_back(read_student(statement.get()));
    }
    if (result != SQLITE_DONE) throw_database_error(connection.get(), "Could not list students");
    return students;
}

std::optional<Student> StudentRepository::get(int id) const {
    Connection connection(database_path_);
    auto statement = prepare(
        connection.get(),
        "SELECT id, name, age, major, gpa FROM students WHERE id = ?1"
    );
    check_bind(connection.get(), sqlite3_bind_int(statement.get(), 1, id));

    const int result = sqlite3_step(statement.get());
    if (result == SQLITE_DONE) return std::nullopt;
    if (result != SQLITE_ROW) throw_database_error(connection.get(), "Could not read student");
    return read_student(statement.get());
}

Student StudentRepository::create(const Student& student) const {
    Connection connection(database_path_);
    auto statement = prepare(
        connection.get(),
        "INSERT INTO students (name, age, major, gpa) VALUES (?1, ?2, ?3, ?4)"
    );
    bind_student(connection.get(), statement.get(), student);
    check_step(connection.get(), sqlite3_step(statement.get()), "Could not create student");

    Student created = student;
    created.id = static_cast<int>(sqlite3_last_insert_rowid(connection.get()));
    return created;
}

bool StudentRepository::update(int id, const Student& student) const {
    Connection connection(database_path_);
    auto statement = prepare(
        connection.get(),
        "UPDATE students SET name = ?1, age = ?2, major = ?3, gpa = ?4, "
        "updated_at = CURRENT_TIMESTAMP WHERE id = ?5"
    );
    bind_student(connection.get(), statement.get(), student);
    check_bind(connection.get(), sqlite3_bind_int(statement.get(), 5, id));
    check_step(connection.get(), sqlite3_step(statement.get()), "Could not update student");
    return sqlite3_changes(connection.get()) == 1;
}

bool StudentRepository::remove(int id) const {
    Connection connection(database_path_);
    auto statement = prepare(connection.get(), "DELETE FROM students WHERE id = ?1");
    check_bind(connection.get(), sqlite3_bind_int(statement.get(), 1, id));
    check_step(connection.get(), sqlite3_step(statement.get()), "Could not delete student");
    return sqlite3_changes(connection.get()) == 1;
}

bool StudentRepository::health() const {
    Connection connection(database_path_);
    auto statement = prepare(connection.get(), "SELECT 1");
    if (sqlite3_step(statement.get()) != SQLITE_ROW) {
        throw_database_error(connection.get(), "SQLite health check failed");
    }
    return true;
}
