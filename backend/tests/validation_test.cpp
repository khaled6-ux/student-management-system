#include "student_service.hpp"

#include <cassert>
#include <iostream>

int main() {
    const Student valid{0, "  Alex Morgan  ", 20, " Computer Science ", 3.70};
    assert(!validate_student(valid));

    const Student normalized = normalize_student(valid);
    assert(normalized.name == "Alex Morgan");
    assert(normalized.major == "Computer Science");

    assert(validate_student(Student{0, " ", 20, "Computer Science", 3.7}).has_value());
    assert(validate_student(Student{0, "Alex", 0, "Computer Science", 3.7}).has_value());
    assert(validate_student(Student{0, "Alex", 121, "Computer Science", 3.7}).has_value());
    assert(validate_student(Student{0, "Alex", 20, "", 3.7}).has_value());
    assert(validate_student(Student{0, "Alex", 20, "Computer Science", -0.1}).has_value());
    assert(validate_student(Student{0, "Alex", 20, "Computer Science", 4.1}).has_value());

    std::cout << "Student validation tests passed.\n";
    return 0;
}
