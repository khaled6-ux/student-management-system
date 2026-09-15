

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "student.h"
#include "studentmanager.h"
using namespace std;

int main()
{
    studentManager manager;
    manager.loadFromFile();

    int choice;

    do
    {
        cout << "\n===== Student Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. View Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
		cout << "6. Sort Students\n";
		cout << "7. Show Statistics\n";
        cout << "8. Exit\n";
        cout << "Choose an option: ";

		while (!(cin >> choice))
		{
			cout << "Invalid input. Please enter a number: ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

        if (choice == 1)
        {
            manager.addStudent();
        }

        else if (choice == 2)
        {
            manager.viewStudents();
        }

        else if (choice == 3)
        {
            manager.searchStudent();
        }

        else if (choice == 4)
        {
            manager.updateStudent();
        }

        else if (choice == 5)
        {
            manager.deleteStudent();
        }
		else if (choice == 6)
		{
			manager.sortStudents();
		}
		else if (choice == 7)
		{
			manager.showstatistics();
		}
        else if (choice == 8)
        {
            cout << "\nExiting program...\n";
        }

        else
        {
            cout << "\nInvalid option. Try again.\n";
        }

    } while (choice != 8);

    return 0;
}

