#ifndef DFA_H
#define DFA_H
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
#include <string.h>
#include<stdio.h>
#include<stdlib.h>



class DFA
{
    public:
        DFA();
        vector <Minimization :: graph> getDfaStates();
        vector <char> getFinalStates();
        unordered_map<char, string> getNamesMap();
        void runDFA();
    protected:

    private:
};

#endif // DFA_H
