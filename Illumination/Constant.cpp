#include "stdafx.h"
using namespace std;

string doubleToString(double num)
{
    stringstream ss;
    string str;
    ss << num;
    ss >> str;
    return str;
}