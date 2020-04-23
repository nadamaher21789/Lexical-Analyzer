#ifndef MINIMIZATION_H
#define MINIMIZATION_H
#include <bits/stdc++.h>
using namespace std;

class Minimization
{
public:
    Minimization();
    struct graph
    {
        char from;
        char to;
        char input;
    };
    char phiState(vector<graph> dfa, vector<char> allStates, vector<char> inputs);
    bool goToPhie(vector<graph> dfa, vector<char> inputs, char state, char phi);
    vector<vector<char>> minimize(vector<graph> dfa, vector<char> endStates, vector<char> normalStates, vector<char> inputs);
    char goesTo(vector<graph> dfa, char input, char state);
    vector<graph> minTable(vector<vector<char>> allSet, vector<char> inputs, vector<graph> dfa);
    vector<char> newEndStates(vector<char> endStates, vector<vector<char>> allSet);
    unordered_map<char, string> endNames(unordered_map<char, string> name, vector<char> endStates);
    char newStart(char start, vector<vector<char>> allSet);
    unordered_map<char, int> statesMap;
protected:

private:
};

#endif // MINIMIZATION_H
