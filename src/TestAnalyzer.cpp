#include "TestAnalyzer.h"
#include <iostream>
#include <fstream>
#include <ctype.h>
using namespace std;

TestAnalyzer::TestAnalyzer()
{
    //ctor
}
string TestAnalyzer :: isEnd(vector<char> endStates, char state, unordered_map <char, string> name, string token)
{
    vector<char> :: iterator it = endStates.begin();
    while(it < endStates.end())
    {
        if((*it) == state)
        {
            return name.find(state) -> second;
        }
        it++;
    }
    return "no";
}
void TestAnalyzer :: analyze(vector<Minimization::graph> minimDfa, char startState, vector<char> endStates, unordered_map <char, string> name, unordered_map <string, int> priority, vector<string> keys)
{
    ifstream fin("inputProgram.txt");
    string line;
    freopen("output.txt", "w", stdout);
    bool done = false;
    string l = "";
    int index = 0;
    string tempString = "";

    while(!done)
    {
        if(index >= l.length()){
                if(!getline(fin, line)){
                    done = true;
                    if(tempString == ""){
                        continue;
                    }
                }
                else{
                    l = line;
                    if(l.length()== 0){
                        continue;
                    }
                    index = 0;
                }
        }
        string token = "";
        char c;
        if(tempString != "")
        {
            c = tempString.at(tempString.length() - 1);
            tempString = "";
        }
        else
        {
            c = l.at(index);
            index++;
        }

        if(isspace(c))
        {
            continue;
        }
        token += c;
        unordered_map<string, int> allPossibilities;
        int countPrefix = 0;
        vector<char> currStates;
        currStates.push_back(startState);
        vector<Minimization :: graph> :: iterator minIt = minimDfa.begin();

        bool doneToken = false;
        while(!doneToken)
        {
            doneToken = true;
            vector<char> :: iterator itCurr = currStates.begin();
            vector<char> nextStates;
            while(itCurr < currStates.end())
            {
                vector<Minimization :: graph> :: iterator minIt = minimDfa.begin();
                while(minIt < minimDfa.end())
                {

                    if((*minIt).from == (*itCurr) && (*minIt).input == c)
                    {
                        string s = isEnd(endStates, (*minIt).to, name, token);
                        if(s == "num"){
                            int tempCount = 1;
                            while(index < l.length() && !(isspace(c)) && (isdigit(c) || c == '.')){
                                c = l.at(index);
                                index++;
                                token += c;
                                tempCount++;
                            }
                            if(!isdigit(c)){
                                index--;
                            }
                            allPossibilities[s] = tempCount;
                        }
                        else if(s != "no")
                        {
                            string tempS;
                            int tempCount = countPrefix - 1;
                            char cTemp = c;
                            char nextStatesTemp = (*minIt).to;
                            if(index >= l.length()){
                                tempCount++;
                                tempS = s;
                            }
                            while(s != "no" && index < l.length())
                            {
                                tempS = s;
                                s = "no";
                                vector<Minimization :: graph> :: iterator minItTemp = minimDfa.begin();
                                cTemp = l.at(index);
                                index++;
                                tempString += cTemp;
                                while(minItTemp < minimDfa.end())
                                {
                                    if((*minItTemp).from == (nextStatesTemp) && (*minItTemp).input == cTemp)
                                    {
                                        string t = token;
                                        t += cTemp;
                                        s = isEnd(endStates, (*minItTemp).to, name, t);
                                        nextStatesTemp = (*minItTemp).to;
                                        if(s != "no"){
                                            token += cTemp;
                                        }
                                        break;
                                    }
                                    minItTemp++;
                                }
                                tempCount++;
                            }
                            allPossibilities[tempS] = tempCount;
                        }
                        else
                        {
                            nextStates.push_back((*minIt).to);
                        }
                    }
                    minIt++;
                }
                itCurr++;
            }
            if(nextStates.size() != 0)
            {
                doneToken = false;
                countPrefix++;
                currStates = nextStates;
                if( tempString != "")
                {
                    c = tempString.at(0);
                    tempString = tempString.substr(1, tempString.length() - 1);
                }
                else
                {
                    c = l.at(index);
                    index++;
                    token += c;
                }
            }
        }
        if(allPossibilities.size() == 0)
        {
            cout << "ERROR"<< token <<endl;
        }
        else
        {
            unordered_map<string, int> :: iterator itMap = allPossibilities.begin();
            string match;
            int longest = -1;
            while(itMap != allPossibilities.end())
            {
                if(((itMap) -> second) > longest)
                {
                    match = (itMap) -> first;
                    longest = (itMap) -> second;
                }
                else if (((itMap) -> second) == longest)
                {
                    int p1 = priority.find((itMap) -> first) -> second;
                    int p2 = priority.find(match) -> second;
                    if(p1 < p2)
                    {
                        match = (itMap) -> first;
                    }
                }
                itMap++;
            }
            token.erase(remove(token.begin(), token.end(), ' '), token.end());
            if(match == "keyword" && find(keys.begin(), keys.end(), token) < keys.end())
            {
                cout << token <<endl;
            }
            else if(match == "keyword" && !(find(keys.begin(), keys.end(), token) < keys.end())){

                cout << "id" << endl;
            }
            else if(match == "id" && (find(keys.begin(), keys.end(), token) < keys.end())){
                cout << token << endl;
            }
            else if(match == "!=")
            {
                cout << "relop" <<endl;
            }
            else if(match == "punctuations")
            {
                cout << token << endl;
            }
            else
            {
                cout << match << endl;
            }
        }
    }
    freopen("CON", "w", stdout);
}
