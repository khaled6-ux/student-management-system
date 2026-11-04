#include "student_service.hpp"
#include <algorithm>
#include <cctype>
Student normalize_student(Student value) {
 auto trim=[](std::string s){ auto a=s.find_first_not_of(" \t\r\n"); if(a==std::string::npos)return std::string{}; auto b=s.find_last_not_of(" \t\r\n"); return s.substr(a,b-a+1);};
 value.name=trim(value.name); value.major=trim(value.major); return value;
}
std::optional<std::string> validate_student(const Student& raw) {
 auto s=normalize_student(raw);
 if(s.name.empty()||s.name.size()>100)return "Name is required and must be at most 100 characters.";
 if(s.major.empty()||s.major.size()>100)return "Major is required and must be at most 100 characters.";
 if(s.age<1||s.age>120)return "Age must be between 1 and 120.";
 if(s.gpa<0||s.gpa>4)return "GPA must be between 0.00 and 4.00.";
 return std::nullopt;
}
