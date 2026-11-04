#pragma once
#include "student.hpp"
#include <optional>
#include <string>
Student normalize_student(Student value);
std::optional<std::string> validate_student(const Student& value);
