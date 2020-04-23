#ifndef RE_TO_NFA_H
#define RE_TO_NFA_H
#include "NFA.h"
#include <iostream>
#include<string>

using namespace std;

class Re_to_NFA
{
public:
    // Re_to_NFA();
    vector<NFA> finalVect;
    void convertNFA();
    void convertDef_NFA();
    NFA concatenate(NFA nfa1, NFA nfa2);
    NFA unionNFA(NFA nfa1, NFA nfa2);
    NFA unionNFAvec(vector<NFA> vec);
    NFA kleeneClosure(NFA nfa1);
    NFA positiveClosure(NFA nfa1);
    NFA finalNFA(vector<NFA> vec);

    vector<NFA> return_keywords(string keywords);
    vector<NFA> return_panc(pair <int, string > temp_pair);
    vector<string> returnKeyWords();
    vector<string>tokens;
    NFA get_keywordsNFA();
    NFA get_pancNFA();
protected:

private:
};

#endif // RE_TO_NFA_H
