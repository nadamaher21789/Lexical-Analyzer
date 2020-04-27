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
    // take input from file
    ifstream myfile;
    myfile.open("output.txt");
    vector<string> tokens;

    string line;
    while(getline(myfile, line))
    {
        tokens.push_back(line);
    }
    tokens.push_back("$");
    stack<string> s;
    s.push("$");
    //push start symbol
    s.push(ptable.nonterminals.at(0));

    // open a file in write mode.
    std::fstream fs;
    fs.open ("final.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    int i=0;
    while (i<tokens.size())
    {

        string top = s.top();
        string current ;
        current =tokens.at(i);

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
            cout << top<<" MATCH==>>"<< current<<endl;

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
            if (tokens1.size()==0)
            {
                i++;
            }
            else
            {
                s.pop();
                for (int j=tokens1.size()-1; j>=0; j--)
                {
                    string str1 ;
                    str1=tokens1.at(j);
                    char z=str1.at(0);
                    if (  z == '\''){
                    str1.erase(std::remove(str1.begin(), str1.end(), '\''), str1.end());
                    }
                    if ((str1.compare("X")) == 0)
                    {
                        i++;
                        cout<<"**EMPTY**"<<endl;

                    }
                    else if ((str1.compare("\\L")) == 0)
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

                fs<<productionRule<<endl;


            }


        }
        else
        {
            //errorrr
        }

    }

    fs.close();
}
