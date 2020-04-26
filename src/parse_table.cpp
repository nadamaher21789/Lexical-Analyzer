#include "parse_table.h"
#include "First.h"

#include<vector>
#include<iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

vector<string> nonterminals;
vector<string> terminals;
string table [50][50];
First f;


void parse_table :: insert_data()
{
    vector<string> nonterm = f.non_terminal;
    for(int i = 0; i<nonterm.size(); i++)
    {
        nonterminals.push_back(nonterm.at(i));
    }
    vector<string> term = f.terminal;
    for(int i = 0; i<term.size(); i++)
    {
        terminals.push_back(term.at(i));
    }
    terminals.push_back("$");

}

void parse_table::build_table()
{
    f.calculate_follow_first();
    vector<set_object> grammer= f.grammar;
    vector<set_object> nonterminals2;
    nonterminals2 = f.follow_sets;
    vector<set_object> terminals2;
    terminals2 = f.first_sets_part2;
    insert_data();
    int n = nonterminals.size();
    int m = terminals.size();
    for(int i = 0; i<grammer.size(); ++i)
    {
        set_object gram = grammer.at(i);
        string left = gram.object_id;
        vector<string> objSet = gram.object_set;
        bool epsilon = false;
        bool endStr = false;
        vector<string> follow;
        vector<string> first;
        for(int j = 0; j<objSet.size(); ++j)
        {
            string cur = objSet.at(j);
            for(int k = 0; k<terminals2.size(); ++k)
            {
                set_object t = terminals2.at(k);
                if(t.object_id.compare(cur) == 0)
                {
                    first = t.object_set;
                    break;
                }
            }
            for(int k2 = 0; k2<nonterminals2.size(); ++k2)
            {
                set_object t = nonterminals2.at(k2);
                if(t.object_id.compare(left) == 0)
                {
                    follow = t.object_set;
                    break;
                }
            }
            for(int a = 0; a <first.size(); ++a)
            {
                string str = first.at(a);
                if (cur.compare("\\L") == 0)
                {
                    epsilon = true;
                }
                else
                {
                    //first case
                    vector<string>::iterator itr1 = std::find(terminals.begin(), terminals.end(),str);
                    int col = distance(terminals.begin(), itr1);
                    vector<string>::iterator itr2 = std::find(nonterminals.begin(), nonterminals.end(),left);
                    int row = distance(nonterminals.begin(), itr2);
                    table[row][col] = cur;
                }
                if(epsilon)
                {
                    for(int b = 0; b<follow.size(); ++b)
                    {
                        string s = follow.at(b);
                        //third case
                        if(s.compare("$") == 0)
                        {
                            endStr = true;
                            vector<string>::iterator itr1 = std::find(terminals.begin(), terminals.end(),s);
                            int col = distance(terminals.begin(), itr1);
                            vector<string>::iterator itr2 = std::find(nonterminals.begin(), nonterminals.end(),left);
                            int row = distance(nonterminals.begin(), itr2);
                            table[row][col] = cur;
                        }
                        else
                        {
                            //second case
                            vector<string>::iterator itr1 = std::find(terminals.begin(), terminals.end(),s);
                            int col = distance(terminals.begin(), itr1);
                            vector<string>::iterator itr2 = std::find(nonterminals.begin(), nonterminals.end(),left);
                            int row = distance(nonterminals.begin(), itr2);
                            table[row][col] = cur;
                        }

                    }
                }
                //for panic mode
                for(int c = 0; c<follow.size(); ++c)
                {
                    string ss = follow.at(c);
                    //add synch
                    vector<string>::iterator itr1 = std::find(terminals.begin(), terminals.end(),ss);
                    int col = distance(terminals.begin(), itr1);
                    vector<string>::iterator itr2 = std::find(nonterminals.begin(), nonterminals.end(),left);
                    int row = distance(nonterminals.begin(), itr2);
                    if(table[row][col].compare("") == 0)
                    {
                        table[row][col] = "synch";
                    }
                }
            }
        }
    }
    // open a file in write mode.
        ofstream outfile;
        outfile.open("final.txt");
        outfile<<"------------------------THE TABLE---------------------------------"<<endl;

    for(int i = 0; i <nonterminals.size(); i++)
    {
        for(int j = 0; j<terminals.size(); j++)
        {
            if(table[i][j].compare("") == 0)
            {
                table[i][j] = "X";
            }
            outfile <<"---"<< table[i][j] ;

        }
        outfile<<endl;
    }
        outfile<<"------------------End Of the table---------------------------------"<<endl;;

}
