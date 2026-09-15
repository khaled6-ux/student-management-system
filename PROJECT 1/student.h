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

    int getID() const;
    string getNAME() const;
    int getAGE() const;
    string getMAJOR() const;
    double getGRADE() const;

	void setID(int ID);
	void setNAME(string NAME);
	void setAGE(int AGE);
	void setMAJOR(string MAJOR);
	void setGRADE(double GRADE);

};
