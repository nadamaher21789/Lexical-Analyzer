#ifndef FIRST_H
#define FIRST_H
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <map>

using namespace std;

class First
{
    public:
    struct set_object{
        string object_id;
        vector<string> object_set;
        int has_epsilon = 0;
    };
    vector<set_object> grammar;
    vector<string> get_first(set_object item);
    vector<string> get_follow(set_object item);
    void readFile();
    vector<string>terminal;
    vector<string>non_terminal;
    vector<set_object> first_sets;
    vector<set_object> first_sets_part2;
    vector<set_object> follow_sets;
    vector<set_object> prev_first_sets;
    void calculate_follow_first();
    void first_of_next();
    vector<set_object> return_g();

    protected:

    private:

};

#endif // FIRST_H
