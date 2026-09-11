#include "student.h"

student::student()
{
    ID = 0;
    NAME = "";
    AGE = 0;
    MAJOR = "";
    GRADE = 0.0;
}

student::student(string NAME, int ID, int AGE, string MAJOR, double GRADE)
{
    this->ID = ID;
    this->NAME = NAME;
    this->AGE = AGE;
    this->MAJOR = MAJOR;
    this->GRADE = GRADE;
}

int student::getID()
{
    return ID;
}

string student::getNAME()
{
    return NAME;
}

int student::getAGE()
{
    return AGE;
}

string student::getMAJOR()
{
    return MAJOR;
}

double student::getGRADE()
{
    return GRADE;
}