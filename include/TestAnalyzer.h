#ifndef TESTANALYZER_H
#define TESTANALYZER_H
#include "Minimization.h"

class TestAnalyzer
{
public:
    TestAnalyzer();
    void analyze(vector<Minimization::graph> minimDfa, char startState, vector<char> endStates, unordered_map <char, string> name, unordered_map <string, int> priority, vector<string> keys);
    string isEnd(vector<char> endStates, char state, unordered_map <char, string> name, string token);
protected:

private:
};

#endif // TESTANALYZER_H
