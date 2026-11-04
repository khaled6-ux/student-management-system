#include "student_repository.hpp"

#include <pqxx/pqxx>

#include <cstdlib>
#include <stdexcept>
#include <string>

namespace {
std::string connection_string() {
    const char* value = std::getenv("DATABASE_URL");
    if (value == nullptr || std::string(value).empty()) {
        throw std::runtime_error("DATABASE_URL is not configured.");
    }
    return value;
}

template <typename Row>
Student read_student(const Row& row) {
    return {
        row["id"].template as<int>(),
        row["name"].template as<std::string>(),
        row["age"].template as<int>(),
        row["major"].template as<std::string>(),
        row["gpa"].template as<double>()
    };
}
} // namespace

std::vector<Student> StudentRepository::list() const {
    pqxx::connection connection(connection_string());
    pqxx::read_transaction transaction(connection);
    const auto rows = transaction.exec(
        "SELECT id, name, age, major, gpa FROM students ORDER BY id DESC"
    );
    std::vector<Student> students;
    students.reserve(rows.size());
    for (const auto& row : rows) students.push_back(read_student(row));
    return students;
}

std::optional<Student> StudentRepository::get(int id) const {
    pqxx::connection connection(connection_string());
    pqxx::read_transaction transaction(connection);
    const auto rows = transaction.exec_params(
        "SELECT id, name, age, major, gpa FROM students WHERE id = $1", id
    );
    if (rows.empty()) return std::nullopt;
    return read_student(rows.front());
}

Student StudentRepository::create(const Student& student) const {
    pqxx::connection connection(connection_string());
    pqxx::work transaction(connection);
    const auto rows = transaction.exec_params(
        "INSERT INTO students (name, age, major, gpa) VALUES ($1, $2, $3, $4) "
        "RETURNING id, name, age, major, gpa",
        student.name, student.age, student.major, student.gpa
    );
    transaction.commit();
    return read_student(rows.front());
}

bool StudentRepository::update(int id, const Student& student) const {
    pqxx::connection connection(connection_string());
    pqxx::work transaction(connection);
    const auto result = transaction.exec_params(
        "UPDATE students SET name = $1, age = $2, major = $3, gpa = $4, updated_at = NOW() "
        "WHERE id = $5",
        student.name, student.age, student.major, student.gpa, id
    );
    transaction.commit();
    return result.affected_rows() == 1;
}

bool StudentRepository::remove(int id) const {
    pqxx::connection connection(connection_string());
    pqxx::work transaction(connection);
    const auto result = transaction.exec_params(
        "DELETE FROM students WHERE id = $1", id
    );
    transaction.commit();
    return result.affected_rows() == 1;
}

bool StudentRepository::health() const {
    pqxx::connection connection(connection_string());
    pqxx::read_transaction transaction(connection);
    transaction.exec("SELECT 1");
    return true;
}
