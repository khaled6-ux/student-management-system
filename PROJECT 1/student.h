#pragma once
#include<string>
using namespace std;


class student {
private:
    string NAME;
    int ID;
    int AGE;
    string MAJOR;
    double GRADE;

public:
    student();
    student(string NAME, int ID, int AGE, string MAJOR, double GRADE);

    int getID();
    string getNAME();
    int getAGE();
    string getMAJOR();
    double getGRADE();
};
