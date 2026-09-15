#pragma once
#include<vector>
#include "student.h"

using namespace std;

class studentManager {
private:
    vector<student> students;
	bool idExists(int id);

public:
    void addStudent();
    void viewStudents() ;
    void searchStudent() ;
    void updateStudent();
    void deleteStudent();

    void saveToFile();
    void loadFromFile();

	void sortStudents();
	void showstatistics();
};