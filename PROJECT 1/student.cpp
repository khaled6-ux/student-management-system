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

int student::getID() const
{
    return ID;
}

string student::getNAME() const
{
    return NAME;
}

int student::getAGE() const
{
    return AGE;
}

string student::getMAJOR() const
{
    return MAJOR;
}

double student::getGRADE() const
{
    return GRADE;
}

void student::setID(int ID)
{
	this->ID = ID;
}
void student::setNAME(string NAME)
{
	this->NAME = NAME;
}
void student::setAGE(int AGE)
{
	this->AGE = AGE;
}
void student::setMAJOR(string MAJOR)
{
	this->MAJOR = MAJOR;
}
void student::setGRADE(double GRADE)
{
	this->GRADE = GRADE;
}
