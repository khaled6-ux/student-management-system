#pragma once

#include "student.hpp"

#include <optional>
#include <string>
#include <vector>

class StudentRepository {
public:
    explicit StudentRepository(std::string database_path = {});

    std::vector<Student> list() const;
    std::optional<Student> get(int id) const;
    Student create(const Student& student) const;
    bool update(int id, const Student& student) const;
    bool remove(int id) const;
    bool health() const;

private:
    std::string database_path_;
};
