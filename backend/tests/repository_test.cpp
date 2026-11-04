#include "student_repository.hpp"

#include <cassert>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>

int main() {
    const auto database_path =
        std::filesystem::temp_directory_path() / "student-management-repository-tests.db";
    std::filesystem::remove(database_path);

    {
        StudentRepository repository(database_path.string());
        assert(repository.health());
        assert(repository.list().empty());

        const Student input{0, "Alex Morgan", 20, "Computer Science", 3.75};
        const Student created = repository.create(input);
        assert(created.id > 0);
        assert(created.name == input.name);

        const auto fetched = repository.get(created.id);
        assert(fetched.has_value());
        assert(fetched->name == input.name);
        assert(fetched->age == input.age);
        assert(fetched->major == input.major);
        assert(std::abs(fetched->gpa - input.gpa) < 0.0001);

        const Student changed{0, "Alex M.", 21, "Software Engineering", 3.90};
        assert(repository.update(created.id, changed));

        const auto updated = repository.get(created.id);
        assert(updated.has_value());
        assert(updated->name == changed.name);
        assert(updated->age == changed.age);
        assert(updated->major == changed.major);
        assert(std::abs(updated->gpa - changed.gpa) < 0.0001);

        const auto records = repository.list();
        assert(records.size() == 1);
        assert(repository.remove(created.id));
        assert(!repository.get(created.id).has_value());
        assert(!repository.remove(created.id));
        assert(repository.list().empty());
    }

    std::filesystem::remove(database_path);
    std::cout << "SQLite repository tests passed.\n";
    return 0;
}
