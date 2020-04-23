#include "ReadRules.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <map>
#include "Re_to_NFA.h"
using namespace std;

bool enter = false;
int priority = 0;
map<string,string > regularDefinition;
map<string, pair<int,string> > regularExpression;
vector<string> keyss;
string keywords = "";
string panc = "";
int flag_key1 = 0;
int flag_key2 = 0;
Re_to_NFA yy ;

void ReadRules::checkExist(std::string k,std::string str)
{
    std::map<std::string,string>::iterator it = regularDefinition.begin();
    while (it != regularDefinition.end())
    {
        std::string key = it->first;
        string::size_type found = str.find(key);
        if (found != std::string::npos)
        {
            enter = true;
            size_t pos = str.find(key);
            str = str.replace(pos, key.length(),it->second);
            regularDefinition.insert({k, str});
        }
        it++;
    }


}
vector<string> ReadRules :: returnKeyWords(){
    return keyss;
}
void ReadRules::readFile()
{
    std::ifstream file("rules.txt");
    string line;
    while (getline(file, line))
    {
        if (line.find('=') != std::string::npos)    /* Regular Definition */
        {
            if(line.find(':') == std::string::npos)
            {
                string key = strtok((char *)line.c_str(), "=");
                std::string value = strtok(NULL, "=");
                key.erase( remove( key.begin(), key.end(), ' ' ), key.end() );
                checkExist(key,value);
                if(!enter)
                {
                    regularDefinition.insert({key, value});
                }
                enter = false;
            }
        }
        if (line.find(':') != std::string::npos)   /* Regular expr */
        {
            string key = strtok((char *)line.c_str(), ":");
            string value = strtok(NULL, ":");
            key.erase( remove( key.begin(), key.end(), ' ' ), key.end() );
            pair <int, string> temp_pair ;
            temp_pair.first = ++priority;
            temp_pair.second = value;
            regularExpression.insert({key, temp_pair});
        }
        else if(line.find('{') != std::string::npos && line.at(0)=='{')     /*keywords*/
        {
            if(!flag_key1)
            {
                flag_key1 = 1;
                string kw = line.c_str();
                kw = kw.substr(1,kw.size()-2);
                keywords = kw;
                yy.return_keywords(keywords);
                //cout << " keyworddddddddd " << keywords << endl;
                vector<string> k = yy.returnKeyWords();
                for(int i = 0; i < k.size(); i++){
                        if(!(find(keyss.begin(), keyss.end(), k[i]) < keyss.end())){
                            keyss.push_back(k[i]);
                        }
                }
            }
        }
        else if (line.find('[') != std::string::npos && line.at(0)=='[')  /*punctuations*/
        {
            if(!flag_key2)
            {
                flag_key2 = 1;
                string kw = line.c_str();
                kw = kw.substr(1,kw.size()-2);
                panc = kw;
                pair <int, string> temp_pair;
                temp_pair.first = ++priority;
                temp_pair.second = panc;
                yy.return_panc(temp_pair);
            }

        }
        flag_key1 = 0;
    }

}
map<string,string> ReadRules::returnDef()
{
    return regularDefinition;
}
map<string,pair<int,string> > ReadRules::returnExpr()
{
    return regularExpression;
}
