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
#include <string.h>
#include<stdio.h>
#include<stdlib.h>


using namespace std;


int i, j, X, Y, A, F, d;
int N;
int M;
class nfa
{
public :
    int transitions[alpha_size][nfa_states_size];
    nfa()
    {

        for (int i=0; i<alpha_size; i++)
        {
            for (int j=0; j<nfa_states_size; j++)
            {
                transitions[i][j]=-1;
            }
        }
    }
} *nfa_states;

void epsilonClosure(int state, bitset <nfa_states_size> &eClosure)
{
    for (int i=0; i<N && nfa_states[state].transitions[0][i]!=-1; i++)
    {
        if (eClosure[nfa_states[state].transitions[0][i]]==0)
        {
            eClosure[nfa_states[state].transitions[0][i]]=1;
            epsilonClosure(nfa_states[state].transitions[0][i],eClosure);
        }
    }
}

void epsilonClosure(bitset <nfa_states_size> state,bitset<nfa_states_size> &eclosure)
{
    for(int i=0; i<N; i++)
    {
        if(state[i]==1)
        {
            epsilonClosure(i,eclosure);
        }
    }
}

void moves(int state,int symbol, bitset<nfa_states_size> &toStates)
{
    for(int i=0; i<N&& nfa_states[state].transitions[symbol][i]!=-1; i++)
    {
        toStates[ nfa_states[state].transitions[symbol][i]]=1;
    }
}

void moves(bitset<nfa_states_size> states,int symbol, bitset<nfa_states_size> &toStates)
{
    for(int i=0; i<N; i++)
    {
        if (states[i]==1)
        {
            moves(i,symbol,toStates);
        }
    }
}
struct dfa
{
    bool finalState;
    bitset<nfa_states_size> transitions[alpha_size];
    bitset<nfa_states_size> nfaStates;
    int ip[alpha_size];
};
vector <dfa*> dfaStates;
vector <int> dfaFstate;
vector <char> normalState;
vector <char> newDFaFinal;
set <int> nfaFstate;
queue <int> s;
bitset<alpha_size> check;
int startState;
unordered_map<int, string> stateName;
unordered_map<char, string> name;
void buildDfa()
{
    d=1;
    dfaStates.push_back(new dfa);
    dfaStates[0]->nfaStates[0]=1;
    epsilonClosure(0,dfaStates[0]->nfaStates);
    for (j=0; j<N; j++)
        if(dfaStates[0]->nfaStates[j]==1 &&nfaFstate.find(j)!=nfaFstate.end())
        {
            dfaStates[0]->finalState=true;
            dfaFstate.push_back(0);
            break;
        }
    int t=0;
    s.push(0);
    while(!s.empty())
    {
        t=s.front();
        s.pop();
        for (int i=1; i<=M ; i++)
        {
            moves(dfaStates[t]-> nfaStates,i,dfaStates[t]->transitions[i]);
            epsilonClosure(dfaStates[t]->transitions[i],dfaStates[t]->transitions[i]);
            for ( j=0; j<d; j++)
                if (dfaStates[t]->transitions[i]== dfaStates[j]->nfaStates)
                {
                    dfaStates[t]->ip[i]=j;
                    break;
                }

            if (j == d)
            {
                dfaStates[t]->ip[i]=d;
                dfaStates.push_back(new dfa);
                dfaStates[d]->nfaStates=dfaStates[t]->transitions[i];
                for (j=0; j<N; j++)
                    if(dfaStates[d]->nfaStates[j]==1 && nfaFstate.find(j)!= nfaFstate.end())
                    {
                        dfaStates[d]->finalState=true;
                        if(!(find(dfaFstate.begin(), dfaFstate.end(), d) < dfaFstate.end())){
                            dfaFstate.push_back(d);
                        }
                            if(stateName.find(j) != stateName.end()){
                                string n = stateName.find(j) -> second;
                                char  c='0'+ d;
                                char b=c+49;
                                char z=(char)b;
                                name[z] = n;
                            }
                    }
                s.push(d);
                d++;
            }
        }
    }
}
struct newDfa
{
    char from;
    char to;
    char ip;
};
vector <Minimization :: graph> newDfaStates;
char startDfastate;
void getNewDfa()
{
    int k=0;
    char c;
    int b;
    char z;
    char from;
    char to;
    char ip;
    Minimization ::graph nDfa;

    for(i=0; i<d; i++)
    {
        for(j=1; j<=M ; j++)
        {
            if (check[j]){
                c='0'+i;
            b=c+49;
            z=(char)b;
            nDfa.from=z;

            c='0'+dfaStates[i]->ip[j];
            b=c+49;
            z=(char)b;
            nDfa.to=z;
            int ipNo=j;
            c='!'+ipNo - 1;
            nDfa.input=c;

            newDfaStates.push_back(nDfa);
            k++;
            }

        }
    }

}
void getNFa()
{

    NFA temp;
    Re_to_NFA reToNfa;
    reToNfa.convertDef_NFA();
    reToNfa.convertNFA();
    NFA keyWords = reToNfa.get_keywordsNFA();

    if(keyWords.transitions.size() != 0){
     reToNfa.tokens.push_back("keyword");
    reToNfa.finalVect.push_back(keyWords);
    }
    NFA punc = reToNfa.get_pancNFA();
    if(punc.transitions.size() != 0){
     reToNfa.tokens.push_back("punctuations");
    reToNfa.finalVect.push_back(punc);
    }
    NFA oldNfa=reToNfa.finalNFA(reToNfa.finalVect);
   N= oldNfa.transitions.at(oldNfa.transitions.size()-1).to_node+1;
    nfa_states = new nfa[N];
    int t;
    for (int i=0; i<oldNfa.acceptable_states.size(); i++)
    {
        t=oldNfa.acceptable_states[i];
        nfaFstate.insert(t);

        stateName[t] = reToNfa.tokens[i];
    }
// find f
    F=nfaFstate.size();
    int transSiza=oldNfa.transitions.size();
    int j;
    int from;
    int to;
    int arrow;
    int maxip=0;
   for (int i=0; i<transSiza; i++)
    {
        from=oldNfa.transitions[i].from_node;
        to = oldNfa.transitions[i].to_node;
        if (strcmp(oldNfa.transitions[i].arrow_state,"\\l") == 0)
        {
            arrow=0;
        }
        else
        {
            arrow=strcmp(oldNfa.transitions[i].arrow_state,"");
            arrow= arrow-32;
        }

        j=0;
        if (arrow>maxip){
            maxip=arrow;
        }
         M=maxip;
        if (i>0 && nfa_states[from].transitions[arrow][0]!= -1)
        {
            for(int k=1; k<M; k++)
            {
                if(nfa_states[from].transitions[arrow][k]==-1)
                {
                    j=k;
                    break;
                }
            }
        }
        check[arrow]=1;
        nfa_states[from].transitions[arrow][j] = to;
    }
}

void getNewStates()
{

   for (int i=0; i<dfaFstate.size(); i++){
         int element= dfaFstate[i];
        char  c='0'+ element;
        char b=c+49;
        char z=(char)b;
        newDFaFinal.push_back(z);
        }

}
void run()
{
    getNFa();
    cout<<"nfa done"<< endl;
    buildDfa();
    cout << "dfa done" << endl;
    getNewDfa();
    cout << "get new dfa done" << endl;
    getNewStates();
    cout << "get new states done" << endl;
}

int main()
{
    run();

    vector<char> normalStates;
    vector<char> inputs;
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

    /* start minimization */

    Minimization dfaMin;

    vector<vector<char>> minim = dfaMin.minimize(newDfaStates, newDFaFinal, normalStates, inputs);
    vector<Minimization :: graph> output = dfaMin.minTable(minim, inputs,newDfaStates);

    /*vector<Minimization :: graph> :: iterator outIt = output.begin();

    cout << "New transitions : "<< endl;
    while(outIt < output.end())
    {
        cout << "FROM:  " << (*outIt).from << "  INPUT:  "<<(*outIt).input << "   TO:  " << (*outIt).to << endl;
        outIt++;
    }*/

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
    return 0;
}
