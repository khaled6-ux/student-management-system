#pragma once
#include "student.hpp"
#include <optional>
#include <vector>
class StudentRepository { public: std::vector<Student> list() const; std::optional<Student> get(int id) const; Student create(const Student& value) const; bool update(int id,const Student& value) const; bool remove(int id) const; };
