#include "First.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <map>
/*
First::First()
{
    //ctor
}
*/
using namespace std;

vector<string> terminal;
vector<string> non_terminal;
vector<string> file_grammar;
vector<string> production_rules;
int start_symbol = 1;



void First::readFile() {
    std::ifstream file("CFG.txt");


    string line;;
    string add_line = "";
    int flag = 0;
    while (getline(file, line)) {

        if(line.at(0)=='#') {
            if(!flag) {
                add_line = line;
                flag = 1;
            } else {
                file_grammar.push_back(add_line);
                add_line=line;
            }

        } else {
            add_line = add_line.substr(0, add_line.size()) +" "+ line;
        }

    }
    file_grammar.push_back(add_line);

    for(vector <string> :: iterator it = file_grammar.begin(); it != file_grammar.end(); ++it) {
        string str = *it;
        string temp_nonTerminal = "";
        int word_end = 0;
        int nonTerminal = 1;
        for(int i = 0; i <= str.size(); i++) {

            if(i == str.size()) {
                set_object obj = grammar.at(grammar.size()-1);
                obj.object_set.push_back(temp_nonTerminal.substr(0, temp_nonTerminal.size()));
                if(temp_nonTerminal.at(0)=='\\' && temp_nonTerminal.at(1)=='L') {
                    if(production_rules.size()==0) {
                        production_rules.push_back("\\L");
                        set_object objk;
                        objk.object_id = "\\L";
                        objk.object_set.push_back("\\L");
                        first_sets_part2.push_back(objk);
                    }
                    obj.has_epsilon = 1;
                }
                grammar.pop_back();
                grammar.push_back(obj);
                if (temp_nonTerminal.find('\'') != std::string::npos) {
                    int end_flag = 0;
                    int start_flag = 0;
                    int start = 0;
                    int ter_size = 0;
                    for(int j = 0; j < temp_nonTerminal.size(); j++) {
                        ter_size++;
                        if(temp_nonTerminal.at(j)=='\'') {
                            if(!start_flag) {
                                start_flag = 1;
                                start = j;
                                ter_size = 0;
                            }
                        } else if(j < temp_nonTerminal.size()-1) {
                            if(start_flag && temp_nonTerminal.at(j+1) =='\'') {
                                string temp_str = temp_nonTerminal.substr(start+1, ter_size);
                                if (std::find(terminal.begin(), terminal.end(), temp_str) != terminal.end()) {

                                } else {
                                    terminal.push_back(temp_str);
                                }
                                start_flag = 0;
                                start = 0;
                                j++;
                            }
                        }
                    }
                }
            } else {
                if(str.at(i) == '#'  || (str.at(i) == '=' && str.at(i+1)==' ') || str.at(i) == '|') {
                    if(str.at(i)=='=' || str.at(i)=='|') {
                        i++;
                        temp_nonTerminal = "";
                    }
                    continue;
                } else if(i>1 && str.at(i) == ' ' && (str.at(i+1)== '|' || nonTerminal)) {

                    if(nonTerminal) {
                        nonTerminal = 0;
                        set_object obj;
                        obj.object_id = temp_nonTerminal;
                        non_terminal.push_back(temp_nonTerminal);
                        grammar.push_back(obj);
                    } else {
                        set_object obj = grammar.at(grammar.size()-1);
                        obj.object_set.push_back(temp_nonTerminal);
                        if(temp_nonTerminal == "\\L") {
                            obj.has_epsilon = 1;
                        }
                        grammar.pop_back();
                        grammar.push_back(obj);
                        if (temp_nonTerminal.find('\'') != std::string::npos) {
                            int end_flag = 0;
                            int start_flag = 0;
                            int start = 0;
                            int ter_size = 0;
                            for(int j = 0; j < temp_nonTerminal.size(); j++) {
                                ter_size++;
                                if(temp_nonTerminal.at(j)=='\'') {
                                    if(!start_flag) {
                                        start_flag = 1;
                                        start = j;
                                        ter_size = 0;
                                    }
                                } else if(j < temp_nonTerminal.size()-1) {
                                    if(start_flag && temp_nonTerminal.at(j+1) =='\'') {
                                        string temp_str = temp_nonTerminal.substr(start+1, ter_size);
                                        if (std::find(terminal.begin(), terminal.end(), temp_str) != terminal.end()) {
                                        } else {
                                            terminal.push_back(temp_str);
                                        }
                                        start_flag = 0;
                                        start = 0;
                                        j++;
                                    }
                                }
                            }
                        }
                    }
                    temp_nonTerminal = "";
                } else if(i > 1) {
                    temp_nonTerminal.push_back(str.at(i));
                }
            }
        }
    }

}

vector<string> First::get_first(set_object item) {

    for(vector <set_object> :: iterator it = first_sets.begin(); it != first_sets.end(); ++it) {
        set_object obj = *it;
        if(obj.object_id == item.object_id) {
            return obj.object_set;
        }
    }
    set_object obj;
    obj.object_id = item.object_id;

    if(item.has_epsilon) {
        obj.has_epsilon = 1;
        obj.object_set.push_back("\\L");
    }
    int if_part2 = 0;
    for(int i = 0; i < item.object_set.size(); i++) {
        string str = item.object_set.at(i);
        set_object obj2;
        if(!(std::find(production_rules.begin(), production_rules.end(), str) != production_rules.end())) {
            production_rules.push_back(str);
            obj2.object_id = str;
            if_part2 = 1;
        }
        if(str.at(0)=='\'') {
            int end_flag = 0;
            for(int j = 1; j < str.size(); j++) {
                if(str.at(j)=='\'') {
                    end_flag = j;
                    str = str.substr(1,end_flag-1);
                    break;
                }
            }
            if(!(std::find(obj.object_set.begin(), obj.object_set.end(), str) != obj.object_set.end())) {
                obj.object_set.push_back(str);
            }
            if(if_part2) {
                if(!(std::find(obj2.object_set.begin(), obj2.object_set.end(), str) != obj2.object_set.end())) {
                    obj2.object_set.push_back(str);
                }
            }
        } else if (str.at(0) == '\'' && str.at(1) == 'L') {
            continue;
        } else {
            vector<string> str_vec;
            int end_flag = 0;

            for(int k=0; k< str.size(); k++) {
                if(str.at(k)==' ') {
                    str_vec.push_back(str.substr(end_flag, k-end_flag));
                    k++;
                    end_flag = k;
                } else if (k == str.size()-1) {
                    str_vec.push_back(str.substr(end_flag, k-end_flag+1));
                }
            }
            int if_epsilon = 0;
            int if_all_epsilon = 1;
            int indexes = 0;
            for(int k=0; k< str_vec.size(); k++) {
                indexes++;
                if(str_vec.at(k).at(0) == '\'') {
                    if_all_epsilon = 0;
                }
                if(k>0 && !if_epsilon) {
                    break;
                } else if (k == 0 || if_epsilon) {

                    if(str_vec.at(k).at(0) == '\'') {
                        if_all_epsilon = 0;
                        string s = str_vec.at(k).substr(1, str_vec.at(k).size()-2);

                        if(!(std::find(obj.object_set.begin(), obj.object_set.end(), s) != obj.object_set.end())) {
                            obj.object_set.push_back(s);

                        }
                        if(if_part2) {
                            if(!(std::find(obj2.object_set.begin(), obj2.object_set.end(), s) != obj2.object_set.end())) {
                                obj2.object_set.push_back(s);
                            }
                        }
                        if_epsilon = 0;
                    } else {
                        vector<string> returned_vec;
                        for(vector <set_object> :: iterator it = grammar.begin(); it != grammar.end(); ++it) {
                            set_object temp_obj = *it;

                            if(temp_obj.object_id == str_vec.at(k)) {

                                if(obj.object_id != temp_obj.object_id) {
                                    returned_vec = get_first(temp_obj);
                                    if(!(std::find(returned_vec.begin(), returned_vec.end(), "\\L") != returned_vec.end())) {
                                        if_all_epsilon = 0;
                                    }
                                    for(int y = 0 ; y< returned_vec.size(); y++) {
                                        if(!(std::find(obj.object_set.begin(), obj.object_set.end(), returned_vec.at(y)) != obj.object_set.end())) {
                                            if(returned_vec.at(y) != "\\L") {
                                                obj.object_set.push_back(returned_vec.at(y));
                                            }
                                        }
                                        if(if_part2) {
                                            if(!(std::find(obj2.object_set.begin(), obj2.object_set.end(), returned_vec.at(y)) != obj2.object_set.end())) {
                                                if(returned_vec.at(y) != "\\L") {
                                                    obj2.object_set.push_back(returned_vec.at(y));
                                                }

                                            }
                                        }
                                    }

                                } else if(if_part2) {
                                    for(int y = 0 ; y< obj.object_set.size(); y++) {
                                        if(!(std::find(obj2.object_set.begin(), obj2.object_set.end(), obj.object_set.at(y)) != obj2.object_set.end())) {
                                            obj2.object_set.push_back(obj.object_set.at(y));
                                        }
                                    }
                                }
                                if(obj.object_id == temp_obj.object_id) {
                                    if(!obj.has_epsilon) {
                                        if_all_epsilon = 0;
                                    }
                                }
                                if((std::find(returned_vec.begin(), returned_vec.end(), "\\L") != returned_vec.end())) {
                                    if_epsilon = 1;
                                } else {
                                    if(obj.object_id == temp_obj.object_id) {
                                        if(obj.has_epsilon) {
                                            if_epsilon = 1;
                                        } else {
                                            if_epsilon = 0;
                                        }
                                    } else {
                                        if_epsilon = 0;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }

            }
            if(if_all_epsilon && indexes == str_vec.size()) {
                if(!(std::find(obj.object_set.begin(), obj.object_set.end(), "\\L") != obj.object_set.end())) {
                    obj.object_set.push_back("\\L");
                    obj.has_epsilon = 1;
                }
            }
            if(if_part2&&if_all_epsilon&& indexes == str_vec.size()) {
                if(!(std::find(obj2.object_set.begin(), obj2.object_set.end(), "\\L") != obj2.object_set.end())) {
                    obj2.object_set.push_back("\\L");
                    obj2.has_epsilon = 1;
                }
            }
        }
        if(if_part2) {
            first_sets_part2.push_back(obj2);
            if_part2 = 0;
        }
    }
    first_sets.push_back(obj);

    return obj.object_set;
}

vector<string> First::get_follow(set_object item) {
    set_object obj;
    int if_found = 0;
    for(vector <set_object> :: iterator it = follow_sets.begin(); it != follow_sets.end(); ++it) {
        set_object temp_obj = *it;
        if(temp_obj.object_id == item.object_id) {
            return temp_obj.object_set;
        }
    }

    obj.object_id = item.object_id;
    set_object start_obj = *grammar.begin();
    if(item.object_id == start_obj.object_id && start_symbol) {
        obj.object_set.push_back("$");
        start_symbol=0;
    }

    for(vector <set_object> :: iterator it = grammar.begin(); it != grammar.end(); ++it) {
        set_object temp_obj = *it;
        for(int y = 0 ; y< temp_obj.object_set.size(); y++) {
            string str = temp_obj.object_set.at(y);
            vector<string> str_vec;
            int end_flag = 0;
            for(int k=0; k< str.size(); k++) {
                if(str.at(k)==' ') {
                    // EXTRA SPACE
                    str_vec.push_back(str.substr(end_flag, k-end_flag));
                    k++;
                    end_flag = k;
                } else if (k == str.size()-1) {
                    str_vec.push_back(str.substr(end_flag, k-end_flag+1));
                }
            }
            for(int i = 0 ; i< str_vec.size(); i++) {
                if(str_vec.at(i) == item.object_id) {
                    if(i < str_vec.size()-1) {
                        string nxt_str = str_vec.at(i+1);
                        if(nxt_str.at(0) == '\'') {
                            string s = nxt_str.substr(1, nxt_str.size()-2);
                            if(!(std::find(obj.object_set.begin(), obj.object_set.end(), s) != obj.object_set.end())) {
                                obj.object_set.push_back(s);
                            }
                        } else if(nxt_str != "\\L") {
                            set_object first_item;
                            int iterr = i+1;
                            for(vector <set_object> :: iterator it2 = first_sets.begin(); it2 != first_sets.end(); ++it2) {
                                set_object temp_first = *it2;
                                if( iterr < str_vec.size()) {
                                    string ss = str_vec.at(iterr);
                                    if(ss.at(0)=='\''){
                                        if(!(std::find(obj.object_set.begin(), obj.object_set.end(), ss.substr(1, ss.size()-2)) != obj.object_set.end())) {
                                            obj.object_set.push_back(ss.substr(1, ss.size()-2));
                                            break;
                                        }
                                    }
                                    if( temp_first.object_id == ss ) {
                                        iterr++;
                                        for(int l = 0 ; l< temp_first.object_set.size(); l++) {
                                            if(temp_first.object_set.at(l) != "\\L") {
                                                if(!(std::find(obj.object_set.begin(), obj.object_set.end(), temp_first.object_set.at(l)) != obj.object_set.end())) {
                                                    obj.object_set.push_back(temp_first.object_set.at(l));
                                                }
                                            }
                                        }


                                        first_item = temp_first;

                                        if(!temp_first.has_epsilon) {
                                            break;
                                        }
                                    }
                                }

                            }
                            if(item.object_id != temp_obj.object_id && first_item.has_epsilon) {
                                int iter = i+2;
                                for(; iter<str_vec.size(); iter++) {
                                    int if_break = 0;
                                    string current_str = str_vec.at(iter);
                                    if(current_str.at(0)=='\'') {
                                        break;
                                    }
                                    for(vector <set_object> :: iterator it3 = first_sets.begin(); it3 != first_sets.end(); ++it3) {
                                        set_object temp_first2 = *it3;
                                        if(temp_first2.object_id == current_str) {
                                            if(!temp_first2.has_epsilon) {
                                                if_break = 1;
                                            }
                                            break;
                                        }
                                    }
                                    if(if_break) {
                                        break;
                                    }

                                }
                                if(iter == str_vec.size()) {
                                    vector<string> returned_vec;
                                    for(vector <set_object> :: iterator it2 = grammar.begin(); it2 != grammar.end(); ++it2) {
                                        set_object temp_grammar = *it2;
                                        if(temp_grammar.object_id == temp_obj.object_id) {
                                            returned_vec = get_follow(temp_grammar);
                                            break;
                                        }
                                    }
                                    for(int m = 0 ; m< returned_vec.size(); m++) {
                                        if(!(std::find(obj.object_set.begin(), obj.object_set.end(), returned_vec.at(m)) != obj.object_set.end())) {
                                            obj.object_set.push_back(returned_vec.at(m));
                                        }
                                    }
                                }

                            }
                        }
                    }
                    if(i == str_vec.size()-1 && str_vec.at(i) != "\\L" && item.object_id != temp_obj.object_id) {
                        vector<string> returned_vec;
                        for(vector <set_object> :: iterator it2 = grammar.begin(); it2 != grammar.end(); ++it2) {
                            set_object temp_grammar = *it2;
                            if(temp_grammar.object_id == temp_obj.object_id) {
                                returned_vec = get_follow(temp_grammar);
                                break;
                            }
                        }
                        for(int m = 0 ; m< returned_vec.size(); m++) {
                            if(!(std::find(obj.object_set.begin(), obj.object_set.end(), returned_vec.at(m)) != obj.object_set.end())) {
                                obj.object_set.push_back(returned_vec.at(m));
                            }
                        }
                    }

                }
            }
        }

    }
    follow_sets.push_back(obj);
    return obj.object_set;
}
void First::calculate_follow_first() {
    readFile();
    for(vector <set_object> :: iterator it = grammar.begin(); it != grammar.end(); ++it) {
        set_object obj_pp = *it;
        get_first(obj_pp);
    }

    for(vector <set_object> :: iterator it = grammar.begin(); it != grammar.end(); ++it) {
        set_object obj_pp = *it;
        get_follow(obj_pp);
    }
}

