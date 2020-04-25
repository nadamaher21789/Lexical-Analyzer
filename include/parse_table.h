#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

#include <iostream>
#include<vector>
#include<string>
//#include<First.h>
#include "First.h"
using namespace std;
class parse_table
{
    public:
        typedef First::set_object set_object ;
        string table[50][50];
        vector<string> nonterminals;
        vector<string> terminals;
        void build_table();
        void insert_data();

    protected:

    private:
};

#endif // PARSE_TABLE_H
