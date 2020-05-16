#include <iostream>
#define alpha_size 255
#define  nfa_states_size 1000
#include <bitset>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <cstdio>
#include <fstream>
#include "NFA.h"
#include "Re_to_NFA.h"
#include "ReadRules.h"
#include "Minimization.h"
#include "TestAnalyzer.h"
#include "DFA.h"
#include "Parsing.h"
#include <string.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
    /* START PHASE ONE */

    /* start NFA and DFA */
    DFA dfa;
    dfa.runDFA();
    vector<char> normalStates;
    vector<char> inputs;
    vector <Minimization :: graph> newDfaStates = dfa.getDfaStates();
    vector<char> newDFaFinal = dfa.getFinalStates();
    unordered_map<char, string> name = dfa.getNamesMap();
    vector<Minimization::graph> :: iterator it = newDfaStates.begin();
    while(it < newDfaStates.end())
    {
        char state = (*it).from;
        char state2 = (*it).to;
        if(!(find(newDFaFinal.begin(), newDFaFinal.end(), state) < newDFaFinal.end()) && !(find(normalStates.begin(), normalStates.end(), state) < normalStates.end()))
        {
            normalStates.push_back(state);
        }
        if(!(find(newDFaFinal.begin(), newDFaFinal.end(), state2) < newDFaFinal.end()) && !(find(normalStates.begin(), normalStates.end(), state2) < normalStates.end()))
        {
            normalStates.push_back(state2);
        }
        if(!(find(inputs.begin(), inputs.end(), (*it).input) < inputs.end()))
        {
            inputs.push_back((*it).input);
        }
        it++;
    }

    /*end NFA and DFA */

    /* start minimization */

    Minimization dfaMin;

    vector<vector<char>> minim = dfaMin.minimize(newDfaStates, newDFaFinal, normalStates, inputs);
    vector<Minimization :: graph> output = dfaMin.minTable(minim, inputs,newDfaStates);

    vector<Minimization :: graph> :: iterator outIt = output.begin();

    cout << "New transitions : "<< endl;
    while(outIt < output.end())
    {
        cout << "FROM:  " << (*outIt).from << "  INPUT:  "<<(*outIt).input << "   TO:  " << (*outIt).to << endl;
        outIt++;
    }

    vector<char> ends = dfaMin.newEndStates(newDFaFinal, minim);
    char newStart = dfaMin.newStart('a', minim);
    unordered_map<char, string> m = dfaMin.endNames(name, newDFaFinal);

    cout << "Minimization done" << endl;

    /* end minimization */

    /*  start test analyzer */

    ReadRules r;
    map<string,pair<int,string>> regExp = r.returnExpr();
    vector<string> keys = r.returnKeyWords();
    unordered_map<string, int> priority;
    map<string,pair<int,string>> :: iterator itExp = regExp.begin();
    priority["keyword"] = 0;
    while(itExp  != regExp.end())
    {
        priority[itExp  -> first] = (itExp -> second).first;
        itExp ++;
    }
    TestAnalyzer t;
    t.analyze(output, newStart, ends, m, priority, keys);

    cout << "Lexical analyzer done" << endl;
    /*  end test analyzer */

    /* END PHASE ONE */

    /*START PHASE TWO */

    Parsing p;
    p.parse();
    cout << "Parsing done" << endl;

    /*END PHASE TWO */
    return 0;
}
