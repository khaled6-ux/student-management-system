# Student Management System

A console-based Student Management System developed in C++ using Object-Oriented Programming principles.

The project provides a simple way to manage student records while demonstrating fundamental C++ programming concepts such as classes, vectors, file handling, input validation, searching, sorting, and data processing.

## Features

* Add new students
* View all students
* Search students by ID
* Update student information
* Delete students
* Sort students by ID, name, or grade
* View student statistics
* Calculate average grade
* Display highest and lowest grades
* Prevent duplicate student IDs
* Validate user input
* Save student data to a file
* Automatically load saved data when the program starts

## Technologies

* C++
* Object-Oriented Programming
* Standard Template Library (STL)
* Vectors
* File I/O
* Visual Studio
* Git and GitHub

## Project Structure

```text
StudentManagementSystem/
├── Student.h
├── Student.cpp
├── StudentManager.h
├── StudentManager.cpp
├── StudentManagementSystem.cpp
└── students.txt
```

`Student` represents an individual student and stores information such as ID, name, age, major, and grade.

`StudentManager` manages the collection of students and provides operations for adding, searching, updating, deleting, sorting, statistics, and file persistence.

`StudentManagementSystem.cpp` contains the main program menu and controls the application flow.

## Example Menu

```text
====================================
     STUDENT MANAGEMENT SYSTEM
====================================
1. Add Student
2. View Students
3. Search Student
4. Update Student
5. Delete Student
6. Sort Students
7. View Statistics
8. Exit
------------------------------------
Choose an option:
```

## Concepts Practiced

This project was developed to strengthen practical understanding of C++ fundamentals, including object-oriented design, encapsulation, vectors, loops, functions, file handling, searching, sorting, input validation, and Git version control.

## Future Improvements

Possible future improvements include SQLite database integration, course management, multiple grades per student, GPA calculations, authentication, and a graphical user interface.

## Author

Developed as a C++ learning and portfolio project.