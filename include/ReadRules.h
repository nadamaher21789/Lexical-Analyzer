#ifndef READRULES_H
#define READRULES_H
#include "map"
#include <iostream>
#include<string>
#include <bits/stdc++.h>

using namespace std;

class ReadRules
{
public:
    // ReadRules();
    void readFile();
    map<string,string> returnDef();
    map<string,pair<int,string> > returnExpr();
    void checkExist(string a,string b);
    vector<string> returnKeyWords();
protected:

private:
};

#endif // READRULES_H
