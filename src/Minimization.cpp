#include "Minimization.h"
#include<iostream>
#include<iterator>
#include<vector>
#include<unordered_map>
#include<unordered_set>

Minimization::Minimization()
{
    //ctor
}

char Minimization :: goesTo(vector<graph> dfa, char input, char state)
{
    vector<graph>::iterator it = dfa.begin();
    while(it != dfa.end())
    {
        if((*it).from == state && (*it).input == input)
        {
            return ((*it).to);
        }
        it++;
    }
    return '#';
}
char Minimization :: phiState (vector<graph> dfa, vector<char> allStates, vector<char> inputs)
{
    vector<char> :: iterator it2 = allStates.begin();
    int c = inputs.size();
    while(it2 < allStates.end())
    {
        vector<char> :: iterator itIn = inputs.begin();
        int c2 = 0;
        while(itIn < inputs.end())
        {
            vector<graph> :: iterator it = dfa.begin();
            while(it < dfa.end())
            {
                if((*it).from == (*it2) && (*it).to == (*it2) && (*it).input == (*itIn))
                {
                    c2++;
                    break;
                }
                it++;
            }
            itIn++;
        }
        if(c2 == c)
        {
            return (*it2);
        }
        it2++;
    }
    return '#';
}
bool Minimization :: goToPhie(vector<graph> dfa, vector<char> inputs, char state, char phi)
{
    vector<char> :: iterator it = inputs.begin();
    int c = inputs.size();
    int c2 = 0;
    while(it < inputs.end())
    {
        vector<graph> :: iterator it2 = dfa.begin();
        while(it2 < dfa.end())
        {
            if((*it2).from == state && (*it2).to == phi && (*it2).input == (*it))
            {
                c2++;
            }
            it2++;
        }
        it++;
    }
    if(c == c2)
    {
        return true;
    }
    return false;
}
vector<vector<char>> Minimization :: minimize(vector<graph> dfa, vector<char> endStates,vector<char> normalStates, vector<char> inputs)
{

    vector<char>::iterator nIt = normalStates.begin();
    vector<char>::iterator eIt = endStates.begin();
    vector<char> endPh;
    vector<char> endNotPh;
    vector<vector<char>> allSet;
    vector<char> allStates;
    while(eIt != endStates.end())
    {
        allStates.push_back(*eIt);
        eIt++;
    }
    while(nIt != normalStates.end())
    {
        allStates.push_back(*nIt);
        nIt++;
    }

    char phi = phiState(dfa, allStates, inputs);
    nIt = normalStates.begin();
    eIt = endStates.begin();
    int countStates = 0;
    while(eIt != endStates.end())
    {
        if(!goToPhie(dfa, inputs, (*eIt), phi))
        {
            statesMap[*eIt] = countStates;
            endNotPh.push_back(*eIt);
        }
        else
        {
            endPh.push_back(*eIt);
        }
        eIt++;
    }
    countStates++;
    while(nIt != normalStates.end())
    {
        statesMap[*nIt] = countStates;
        nIt++;
    }
    countStates++;
    vector<char> :: iterator iPh = endPh.begin();
    while(iPh < endPh.end())
    {
        statesMap[*iPh] = countStates++;
        iPh++;
    }
    if(endNotPh.size() != 0){
     allSet.push_back(endNotPh);
    }
    allSet.push_back(normalStates);

    /*for(int i = 0; i < allSet.size(); i++){
            vector<char> t = allSet[i];
            for(int j = 0; j < t.size(); j++){
                cout << t[j];
            }
            cout <<endl;
    }*/
    /*unordered_map<char,int> :: iterator t = statesMap.begin();
    while(t != statesMap.end()){
        cout << t -> first << "  " << t -> second << endl;
        t++;
    }*/
    bool done = false;
    while(!done)
    {
        done = true;
        vector<vector<char>> allSetTemp;
        vector<vector<char>>::iterator itAll = allSet.begin();
        while(itAll < allSet.end())
        {
            vector<char> :: iterator itSet = (*itAll).begin();
            if((*itAll).size() != 1)
            {
                //cout<< "da5al set  " << *itSet <<endl;
                char prev = (*itSet);
                itSet++;
                while((itSet) < (*itAll).end())
                {
                    vector<char>::iterator inIt = inputs.begin();
                    bool doneInputs = false;
                    while(inIt < inputs.end() && !doneInputs)
                    {
                        //cout << " da5al input " << *inIt <<endl;
                        int int1 = statesMap.find(goesTo(dfa, (*inIt), (*itSet)))->second;
                        int int2 = statesMap.find(goesTo(dfa, (*inIt), prev))->second;
                        if(int1 != int2)
                        {
                            //cout<< "da5al" <<endl;
                            done = false;
                            vector<vector<char>>::iterator itAllTemp = allSet.begin();
                            bool doneReplacing = false;
                            while(itAllTemp < allSet.end() && !doneReplacing)
                            {
                                if(itAllTemp != itAll)
                                {
                                    vector<char>::iterator inItTemp = inputs.begin();
                                    vector<char>::iterator itSetTemp = (*itAllTemp).begin();
                                    bool found = true;
                                    while(inItTemp != inputs.end())
                                    {
                                        int arg1 = statesMap.find(goesTo(dfa,(*inItTemp), (*itSet)))->second;
                                        int arg2 = statesMap.find(goesTo(dfa,(*inItTemp), (*itSetTemp)))->second;
                                        if(arg1 != arg2)
                                        {
                                            found = false;
                                        }
                                        inItTemp++;
                                    }
                                    if(found)
                                    {
                                        statesMap[*itSet] = statesMap.find(*itSetTemp)-> second;
                                        (*itAllTemp).push_back(*itSet);
                                        int index = std::distance(allSet.begin(), itAll);
                                        (*itAll).erase(itSet);
                                        itAll = allSet.begin() + index;
                                        doneReplacing = true;
                                        itSet--;
                                    }
                                }
                                itAllTemp++;
                            }
                            if(!doneReplacing)
                            {
                                statesMap[*itSet] = countStates++;
                                vector<char> newSet;
                                int index = std::distance(allSet.begin(), itAll);
                                char newChar = *itSet;
                                (*itAll).erase(itSet);
                                newSet.push_back(newChar);
                                allSet.push_back(newSet);
                                itAll = allSet.begin() + index;
                                index = std::distance(allSet.begin(), itAll);
                                itSet--;
                            }
                            doneInputs = true;
                        }
                        inIt++;
                    }
                    itSet++;
                }
            }
            itAll++;
        }
    }
    iPh = endPh.begin();
    while(iPh != endPh.end())
    {
        vector<char> endState;
        endState.push_back(*iPh);
        allSet.push_back(endState);
        iPh++;
    }
    unordered_map<char, int> :: iterator itM = statesMap.begin();
    int maxElem = 0;
    while(itM != statesMap.end())
    {
        if((itM ->second) > maxElem)
        {
            maxElem = itM ->second;
        }
        itM++;
    }
    int i = 0;
    while(i < maxElem)
    {
        bool found = false;
        int j = i;
        while(!found)
        {
            itM = statesMap.begin();
            while(itM != statesMap.end())
            {
                if((itM ->second) == j)
                {
                    found = true;
                    break;
                }
                itM++;
            }
            j++;
        }
        j--;
        if(j != i)
        {
            itM = statesMap.begin();
            while(itM != statesMap.end())
            {
                if((itM -> second) == j)
                {
                    statesMap[itM -> first] = i;
                }
                itM++;
            }
        }
        i++;
    }
       /* for(int i = 0; i < allSet.size(); i++){
            vector<char> t = allSet[i];
            for(int j = 0; j < t.size(); j++){
                cout << t[j];
            }
            cout <<endl;
    }*/
    return allSet;
}
vector<Minimization :: graph> Minimization :: minTable(vector<vector<char>> allSet, vector<char> inputs, vector<graph> dfa)
{
    vector<vector<char>> :: iterator it = allSet.begin();
    vector<graph> output;
    int i = 0;
    while(it < allSet.end())
    {
        char newSet = 65 + statesMap.find(*((*it).begin())) -> second;
        i++;
        vector<char> :: iterator inputsIt = inputs.begin();
        char f = newSet;
        while(inputsIt < inputs.end())
        {
            graph g;
            char gT = goesTo(dfa, *inputsIt, *((*it).begin()));
            int arg = statesMap.find(gT) -> second;
            char t = 65 + arg;
            char in = *inputsIt;
            g.from = f;
            g.to = t;
            g.input = in;
            output.push_back(g);
            inputsIt++;
        }
        it++;
    }
    return output;
}
vector<char> Minimization :: newEndStates(vector<char> endStates, vector<vector<char>> allSet)
{
    vector<char> output;
    vector<char> :: iterator itEnd = endStates.begin();
    while(itEnd < endStates.end())
    {
        int i = statesMap.find(*itEnd) -> second;
        if(!(find(output.begin(), output.end(), i + 65) < output.end()))
        {
            output.push_back(i + 65);
        }

        itEnd++;
    }
    return output;
}

char Minimization :: newStart(char start, vector<vector<char>> allSet)
{
    char output;
    int i = statesMap.find(start) -> second;
    output = 65 + i;
    return output;
}
unordered_map<char, string> Minimization :: endNames(unordered_map<char, string> name, vector<char> endStates)
{
    vector<char> :: iterator it = endStates.begin();
    unordered_map<char, string> output;
    while(it < endStates.end())
    {
        char s = (statesMap.find(*it) -> second) + 65;
        output[s] = name.find(*it) -> second;
        it++;
    }
    return output;
}
