#include "Parsing.h"
#include <iostream>
#include<stack>
#include<string>
#include <string.h>
#include<queue>
#include "parse_table.h"
#include "First.h"
#include<vector>
 #include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

Parsing::Parsing()
{
    //ctor
}

parse_table ptable ;


bool isTerminal(string x)
{
    std::vector<string>::iterator itr1 = std::find(ptable.terminals.begin(), ptable.terminals.end(),x);
    if (itr1 != ptable.terminals.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isnonTerminal(string x)
{
    std::vector<string>::iterator it = std::find(ptable.nonterminals.begin(), ptable.nonterminals.end(),x);
    if (it != ptable.nonterminals.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}
queue<string> op;

void Parsing::parse ()
{

    ptable.build_table();
  /*  string str="a a b $";//input string
    stringstream ss(str);
    string item;
    while (getline(ss, item, ' '))
    {
        tokens.push_back(item);
    }
*/
// take input from file
    ifstream myfile;
    myfile.open("output.txt");
    vector<string> tokens;

    string line;
     while(getline(myfile, line)) {
     tokens.push_back(line);
    }
    tokens.push_back("$");
    stack<string> s;
    s.push("$");
//push start symbol
    s.push(ptable.nonterminals.at(0));

    int i=0;
    while (i<tokens.size())
    {

        string top = s.top();
        string current ;
        current =tokens.at(i);

     //   cout << "top========="<<top<<endl;
       // cout << "curr========="<<current<<endl;
        //   i++;

        //successful completion
        if ((top.compare(current)) == 0 &&  ((top.compare("$")) == 0))
        {
            cout << "***ACCEPT**"<<endl;
            break;
        }
        //not matching
        if (tokens.size()>1 &&  ((top.compare("$")) == 0))
        {
            cout << "***NOT ACCEPT**"<<endl;
            break;
        }
        // top is terminal and equal current
        if (isTerminal(top) && ((top.compare(current)) == 0))
        {
           // cout << "d5l========="<<endl;

            s.pop();
            i++;

        }
        else if(isTerminal(top) && ((top.compare(current)) != 0))
        {
            cout << "***ERROR**"<<endl;
            s.pop();
        }
        else if(isnonTerminal(top))
        {
            //look at parser table table[top][current]
            vector<string>::iterator itr1 = std::find(ptable.terminals.begin(),ptable.terminals.end(),current);
            int col = distance(ptable.terminals.begin(), itr1);
            vector<string>::iterator itr2 = std::find(ptable.nonterminals.begin(),ptable.nonterminals.end(),top);
            int row = distance(ptable.nonterminals.begin(), itr2);

            string productionRule = ptable.table[row][col];
            stringstream ss1(productionRule);
            string item1;
            vector<string> tokens1;
            while (getline(ss1, item1, ' '))
            {
                tokens1.push_back(item1);
            }
            bool flag= false;
            //cout << "drow==="<<row<<"=col="<<col<<endl;
            //cout << "d5al in 3==="<<productionRule<<endl;
            if (tokens1.size()==0)
            {
                i++;
            }
            else
            {
                s.pop();
                for (int i=tokens1.size()-1; i>=0; i--)
                {
                    string str1 ;
                    str1=tokens1.at(i);
                    str1.erase(std::remove(str1.begin(), str1.end(), '\''), str1.end());
                    //cout << "**str1=******"<<str1<<endl;
                    if ((str1.compare("\\L")) == 0)
                    {
                        flag=true;
                        cout << "*****epsilon******"<<endl;
                    }
                    else if (str1.compare("synch")==0)
                    {
                        cout << "*****SYNCH******"<<endl;
                    }
                    else
                    {

                        //cout << "dstriiiii==="<<str1<<endl;

                        s.push(str1);
                        flag=true;
                    }
                }
            }
            //push prod rule in output
            if (flag == true)
            {
                op.push(productionRule);
            cout << "production rule : "<< top<< " -> "<<productionRule<<endl;

            }


        }
        else
        {
            //errorrr
        }

        /*   if (top.compare("EDASH")==0){
               top = s.top();

           current =tokens.at(i);
                                                               cout << "top**==="<<top<<endl;
                                                       cout << "tcurr**=="<<current<<endl;

                   break;
                  }*/
    }
}
