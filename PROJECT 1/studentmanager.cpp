#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include "studentmanager.h"
using namespace std;

bool studentManager::idExists(int id)
{
    for (student student : students)
    {
        if (student.getID() == id)
        {
            return true;
        }
    }

    return false;
}

void studentManager::addStudent()
{
    int id;
    string name;
    int age;
    string major;
    double grade;

    cout << "Enter student ID: ";
   
    while (!(cin >> id) || id <= 0)
    {
        cout << "Invalid ID. Enter a positive number: ";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
   
    if (idExists(id))
    {
        cout << "\nStudent with ID " << id << " already exists.\n";
        return;
    }

    cin.ignore();

    cout << "Enter student name: ";
    getline(cin, name);

    cout << "Enter student age: ";

    while (!(cin >> age)|| age <= 0 || age > 100)
    {
        cout << "Invalid input. Enter a number: ";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore();

    cout << "Enter student major: ";
    getline(cin, major);

    cout << "Enter student grade: ";
    while (!(cin >> grade) || grade < 0 || grade > 100)
    {
        cout << "Invalid grade. Enter a number between 0 and 100: ";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    student newStudent(name, id, age, major, grade);

    students.push_back(newStudent);

    cout << "\nStudent added successfully!\n";

    saveToFile();
}
void studentManager::viewStudents()
{
    if (students.empty())
    {
        cout << "\nNo students found.\n";
        return;
    }

    cout << "\n===== Student List =====\n";

    for (student student : students)
    {
        cout << "\nID: " << student.getID() << endl;
        cout << "Name: " << student.getNAME() << endl;
        cout << "Age: " << student.getAGE() << endl;
        cout << "Major: " << student.getMAJOR() << endl;
        cout << "Grade: " << student.getGRADE() << endl;
        cout << "------------------------\n";
    }
}
void studentManager::searchStudent()
{
    int searchId;

    cout << "Enter student ID to search: ";
    cin >> searchId;

    for (student student : students)
    {
        if (student.getID() == searchId)
        {
            cout << "\nStudent found!\n";
            cout << "ID: " << student.getID() << endl;
            cout << "Name: " << student.getNAME() << endl;
            cout << "Age: " << student.getAGE() << endl;
            cout << "Major: " << student.getMAJOR() << endl;
            cout << "Grade: " << student.getGRADE() << endl;

            return;
        }
    }

    cout << "\nStudent not found.\n";
}
void studentManager::updateStudent()
{
    int updateId;

    cout << "Enter student ID to update: ";
    cin >> updateId;

    for (student& student : students)
    {
        if (student.getID() == updateId)
        {
            string newName;
            int newAge;
            string newMajor;
            double newGrade;

            cin.ignore();

            cout << "Enter new name: ";
            getline(cin, newName);

            cout << "Enter new age: ";
            cin >> newAge;

            cin.ignore();

            cout << "Enter new major: ";
            getline(cin, newMajor);

            cout << "Enter new grade: ";
            cin >> newGrade;

            student.setNAME(newName);
            student.setAGE(newAge);
            student.setMAJOR(newMajor);
            student.setGRADE(newGrade);

            cout << "\nStudent updated successfully!\n";

            saveToFile();

            return;
        }
    }

    cout << "\nStudent not found.\n";
}
void studentManager::deleteStudent()
{
    int deleteId;

    cout << "Enter student ID to delete: ";
    cin >> deleteId;

    for (int i = 0; i < students.size(); i++)
    {
        if (students[i].getID() == deleteId)
        {
            students.erase(students.begin() + i);

            cout << "\nStudent deleted successfully!\n";

            saveToFile();

            return;
        }
    }

    cout << "\nStudent not found.\n";
}

void studentManager::saveToFile()
{
    ofstream file("students.txt");

    for (student student : students)
    {
        file << student.getID() << "|"
            << student.getNAME() << "|"
            << student.getAGE() << "|"
            << student.getMAJOR() << "|"
            << student.getGRADE() << endl;
    }

    file.close();
}

void studentManager::loadFromFile()
{
    ifstream file("students.txt");

    if (!file.is_open())
    {
        return;
    }

    string line;

    while (getline(file, line))
    {
        size_t pos1 = line.find("|");
        size_t pos2 = line.find("|", pos1 + 1);
        size_t pos3 = line.find("|", pos2 + 1);
        size_t pos4 = line.find("|", pos3 + 1);

        int id = stoi(line.substr(0, pos1));

        string name =
            line.substr(pos1 + 1, pos2 - pos1 - 1);

        int age =
            stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));

        string major =
            line.substr(pos3 + 1, pos4 - pos3 - 1);

        double grade =
            stod(line.substr(pos4 + 1));

        student student(name, id, age, major, grade);

        students.push_back(student);
    }

    file.close();

}

void studentManager::sortStudents()
{
    if (students.empty())
    {
        cout << "\nNo students available to sort.\n";
        return;
    }

    int choice;

    cout << "\n===== Sort Students =====\n";
    cout << "1. Sort by ID\n";
    cout << "2. Sort by Name\n";
    cout << "3. Sort by Grade\n";
	cout << "4. Sort by Age\n";
    cout << "Choose an option: ";

    cin >> choice;

    if (choice == 1)
    {
        sort(students.begin(), students.end(),
            [](const student& a, const student& b)
            {
                return a.getID() < b.getID();
            });

        cout << "\nStudents sorted by ID.\n";
    }

    else if (choice == 2)
    {
        sort(students.begin(), students.end(),
            [](const student& a, const student& b)
            {
                return a.getNAME() < b.getNAME();
            });

        cout << "\nStudents sorted by name.\n";
    }

    else if (choice == 3)
    {
        sort(students.begin(), students.end(),
            [](const student& a, const student& b)
            {
                return a.getGRADE() > b.getGRADE();
            });

        cout << "\nStudents sorted by grade.\n";
    }

    else if (choice == 4)
    {
        sort(students.begin(), students.end(),
            [](const student& a, const student& b)
            {
                return a.getAGE() > b.getAGE();
            });

        cout << "\nStudents sorted by age.\n";
    }

    else
    {
        cout << "\nInvalid sorting option.\n";
        return;
    }

    viewStudents();
}

void studentManager::showstatistics()
{
    if (students.empty())
    {
        cout << "\nNo students available.\n";
        return;
    }

    double totalGrade = 0;

    double highestGrade = students[0].getGRADE();
    double lowestGrade = students[0].getGRADE();

    string highestStudent = students[0].getNAME();
    string lowestStudent = students[0].getNAME();

    for (const student& student : students)
    {
        double grade = student.getGRADE();

        totalGrade += grade;

        if (grade > highestGrade)
        {
            highestGrade = grade;
            highestStudent = student.getNAME();
        }

        if (grade < lowestGrade)
        {
            lowestGrade = grade;
            lowestStudent = student.getNAME();
        }
    }

    double averageGrade = totalGrade / students.size();

    cout << "\n===== Student Statistics =====\n";
    cout << "Total students: " << students.size() << endl;
    cout << "Average grade: " << averageGrade << endl;
    cout << "Highest grade: " << highestGrade
        << " (" << highestStudent << ")" << endl;

    cout << "Lowest grade: " << lowestGrade
        << " (" << lowestStudent << ")" << endl;
}