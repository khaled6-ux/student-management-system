

#include <iostream>
#include "student.h"
using namespace std;

int main()
{
	student s1("John Doe", 12345, 20, "Computer Science", 3.8);

	cout << "Student Name: " << s1.getNAME() << endl;
	cout << "Student ID: " << s1.getID() << endl;
	cout << "Student Age: " << s1.getAGE() << endl;
	cout << "Student Major: " << s1.getMAJOR() << endl;
	cout << "Student Grade: " << s1.getGRADE() << endl;
}

