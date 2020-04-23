#include "Re_to_NFA.h"
#include"NFA.h"
#include "ReadRules.h"
#include<iostream>
#include<stack>
#include <string>
#include <cstring>

ReadRules x;
map<string,NFA> defToNfa;
map<int,pair<string,NFA> > final_map;
//////////////////////////////
vector<NFA> keywords_vec;
vector<NFA> panc_vec;
vector<NFA> finalVect;
vector<string>tokens;
NFA keywords_nfa;
NFA panc_nfa;
///////////////////////////////
char* appendCharToCharArray(char* array, char a)
{
    size_t len = strlen(array);
    char* ret = new char[len+2];
    strcpy(ret, array);
    ret[len] = a;
    ret[len+1] = '\0';
    return ret;
}
char* appendCharToChar(char* array, char a)
{
    size_t len = strlen(array);

    char* ret = new char[len+2];

    strcpy(ret, array);
    ret[len] = a;
    ret[len+1] = '\0';

    return ret;

}
void Re_to_NFA::convertNFA()
{
    x.readFile();
    map<string,pair<int,string> > expr = x.returnExpr();
    std::map<std::string, pair<int,string> >::iterator it = expr.begin();
    std::stack<char> operators;
    std::stack<NFA> operands;
    string op_sym;
    int op_count;
    char cur;
    int j = 0;
    bool inside = false;
    while (it != expr.end())
    {
        pair <int, string > tempPair1 = it->second;
        string re = tempPair1.second;
        tokens.push_back(it->first);
        for(int i = 0; i <re.size(); i++)
        {
            cur = re.at(i);
            if(cur == ' ')
            {
                continue;
            }
            char y;
            string str;
            bool start = false;
            if(i>0)
            {
                y = re.at(i-1);
                if(y == '(')
                {
                    start = true;
                }
            }
            int flag = 0;
            bool test = false;
            while((cur>='a'&& cur<='z') || (cur>='A' && cur<='Z'))
            {
                //alphabet
                test = true;
                flag = 1;
                str += cur;
                i++;
                if(i != re.size())
                {
                    cur = re.at(i);
                }
                else
                {
                    break;
                }
            }
            bool def = false;
            std::map<std::string, NFA>::iterator it;
            it = defToNfa.find(str);
            if (it != defToNfa.end())
            {
                def = true;
                string name = it->first;
                NFA sec_nfaa = it->second;
                operands.push(it->second);
                i--;
                cur = re.at(i);
            }

            if(test && !def)
            {
                cur = re.at(i-1);
            }
            bool epsilon = false;
            if(cur != '(' && cur != ')' && cur != '*' && cur != '|' && cur != '+')
            {
                if(cur == '\\')
                {
                    cur = re.at(i+1);
                    if(cur == 'l' || cur == 'L')
                    {
                        cur = '\\l';
                        epsilon = true;
                        NFA *nfa = new NFA();
                        char* temp = "\\l";
                        nfa->add_transition(0,1,temp);
                        nfa->set_finalNode(1);
                        nfa->set_states(2);
                        j++;
                        operands.push(*nfa);
                    }
                    i++;
                }
                if(!def && !epsilon)
                {
                    char *temp = "";
                    NFA *nfa = new NFA();
                    temp = appendCharToCharArray(temp,cur);
                    nfa->add_transition(0,1,temp);
                    nfa->set_finalNode(1);
                    nfa->set_states(2);
                    j++;
                    operands.push(*nfa);
                }
                def = false;
                if(i > 0)
                {
                    y = re.at(i-1);
                    if(y == ' ')
                    {
                        if(i-2 >= 0)
                        {
                            y = re.at(i-2);
                        }
                    }
                }
                if (i>0 && !start && inside && !operators.empty()&& operators.top() == '#' && y != '(' && y != '|')
                {
                    operators.pop();
                    NFA nfa2 = operands.top();
                    operands.pop();
                    NFA nfa1 = operands.top();
                    operands.pop();
                    operands.push(concatenate(nfa1, nfa2));

                }
                if(i != re.size()-1)
                {
                    y = re.at(i+1);
                    if(y == ' ')
                    {
                        y = re.at(i+2);
                    }
                    //concatenation
                    if(y != '+' && y != '|' && y != ')' && y != '*')
                    {
                        operators.push('#');
                    }
                    else if(y == '+')
                    {
                        NFA nfa1 = operands.top();
                        operands.pop();
                        operands.push(positiveClosure(nfa1));
                        i++;
                        if(i != re.size()-1)
                        {
                            y = re.at(i+1);
                            if(y == ' ')
                            {
                                y = re.at(i+2);
                            }
                            //concatenation
                            if(y != '+' && y != '|' && y != ')' && y != '*')
                            {
                                operators.push('#');
                            }
                        }
                    }
                    else if(y == '*')
                    {
                        NFA nfa1 = operands.top();
                        operands.pop();
                        operands.push(kleeneClosure(nfa1));
                        i++;
                        if(i != re.size()-1)
                        {
                            operators.push('#');
                        }
                    }
                }


            }
            else if (cur == '(')
            {
                operators.push('(');
                inside = true;

            }
            else if(cur == '|')
            {
                y = re.at(i-1);
                if(y == '+')
                {
                    operators.pop();
                }
                operators.push('|');
            }
            else if (cur == ')')
            {
                vector<NFA> orVector2;
                char a = operators.top();

                int count = 0;
                while(a!='(')
                {
                    if(a == '|')
                    {
                        count++;
                    }
                    operators.pop();
                    a = operators.top();
                }

                operators.pop();
                int c = count;
                if(count != 0)
                {
                    for(int k = 0; k<=count; k++)
                    {
                        NFA nfa = operands.top();
                        orVector2.push_back(nfa);
                        operands.pop();
                        c--;
                    }
                }
                if(!orVector2.empty())
                {
                    operands.push(unionNFAvec(orVector2));
                }

                while (!orVector2.empty())
                {
                    orVector2.pop_back();
                }
                inside = false;
                char x;
                if(i != re.size()-1)
                {
                    x = re.at(i+1);
                    bool inc = false;
                    if(x == ' ')
                    {
                        inc = true;
                        if(i+2 < re.size())
                        {
                            x = re.at(i+2);
                        }

                    }
                    if(x == '*')
                    {
                        NFA nfa = operands.top();
                        operands.pop();
                        operands.push(kleeneClosure(nfa));
                        i++;
                        if(inc)
                        {
                            i++;
                        }
                    }
                    else if (x == '+')
                    {
                        NFA nfa = operands.top();
                        operands.pop();
                        operands.push(positiveClosure(nfa));
                        i++;
                        if(inc)
                        {
                            i++;
                        }
                    }
                }
                //concatenation before bracket with the bracket
                while (!operators.empty() && (operators.top() == '|' || operators.top() == '#'))
                {
                    if (operators.top() == '#')
                    {
                        operators.pop();
                        NFA nfa2 = operands.top();
                        operands.pop();
                        NFA nfa1 = operands.top();
                        operands.pop();
                        operands.push(concatenate(nfa1, nfa2));
                    }
                    else if (operators.top() == '|')
                    {
                        operators.pop();
                        NFA nfa2 = operands.top();
                        operands.pop();
                        NFA nfa1 = operands.top();
                        operands.pop();
                        vector<NFA>orVector2;
                        orVector2.push_back(nfa1);
                        orVector2.push_back(nfa2);
                        operands.push(unionNFAvec(orVector2));
                        while (!orVector2.empty())
                        {
                            orVector2.pop_back();
                        }
                    }
                }
            }
            if(i == re.size()-1)
            {
                while (!operators.empty() && (operators.top() == '|' || operators.top() == '#'))
                {
                    if (operators.top() == '#')
                    {
                        operators.pop();
                        NFA nfa2 = operands.top();
                        operands.pop();
                        NFA nfa1 = operands.top();
                        operands.pop();
                        operands.push(concatenate(nfa1, nfa2));
                    }
                    else if (operators.top() == '|')
                    {
                        operators.pop();
                        NFA nfa2 = operands.top();
                        operands.pop();
                        NFA nfa1 = operands.top();
                        operands.pop();
                        vector<NFA>orVector2;
                        orVector2.push_back(nfa1);
                        orVector2.push_back(nfa2);
                        operands.push(unionNFAvec(orVector2));
                        while (!orVector2.empty())
                        {
                            orVector2.pop_back();
                        }
                    }
                }
            }
        }
        /////////
        pair<int,string> temp_pair1 =it->second;
        pair<string,NFA> temp_pair2 ;
        temp_pair2.first = it->first;
        temp_pair2.second = operands.top();
        final_map.insert({temp_pair1.first, temp_pair2});
        finalVect.push_back(operands.top());
        ///////////
        while(!operands.empty())
        {
            operands.pop();
        }
        while(!operators.empty())
        {
            operators.pop();
        }
        it++;
    }
    while (!operators.empty() && (operators.top() == '|' || operators.top() == '#'))
    {
        if (operators.top() == '#')
        {
            operators.pop();
            NFA nfa2 = operands.top();
            operands.pop();
            NFA nfa1 = operands.top();
            operands.pop();
            operands.push(concatenate(nfa1, nfa2));
        }
        if (operators.top() == '|')
        {
            operators.pop();
            NFA nfa2 = operands.top();
            operands.pop();
            NFA nfa1 = operands.top();
            operands.pop();
        }
    }
}

NFA Re_to_NFA::concatenate(NFA nfa1, NFA nfa2)
{
    NFA nfa = nfa1;
    int num_states = nfa1.final_node+nfa2.final_node-1;
    int to_add = nfa1.final_node;
    for(int i = 0; i < nfa2.transitions.size(); i++)
    {
        nfa.add_transition( nfa2.transitions.at(i).from_node+to_add,
                            nfa2.transitions.at(i).to_node+to_add,
                            nfa2.transitions.at(i).arrow_state);
    }
    nfa.set_finalNode(num_states-1);
    nfa.set_states(num_states);
    return nfa;
}

NFA Re_to_NFA::kleeneClosure(NFA nfa1)
{
    NFA nfa;
    int to_int;
    nfa.add_transition(nfa1.transitions.at(0).from_node,
                       nfa1.transitions.at(0).from_node + 1,
                       "\\l");
    for(int i = 0; i < nfa1.transitions.size(); i++)
    {
        nfa.add_transition( nfa1.transitions.at(i).from_node +1,
                            nfa1.transitions.at(i).to_node +1,
                            nfa1.transitions.at(i).arrow_state);
        to_int = nfa1.transitions.at(i).to_node +1;
    }
    nfa.add_transition(to_int,
                       to_int+1,
                       "\\l");
    nfa.add_transition(to_int,
                       nfa1.transitions.at(0).from_node+1,
                       "\\l");
    nfa.add_transition(nfa1.transitions.at(0).from_node,
                       to_int+1,
                       "\\l");
    nfa.set_states(nfa1.get_states()+2);
    nfa.set_finalNode(nfa.get_states()-1);
    return nfa;
}

NFA Re_to_NFA::positiveClosure(NFA nfa1)
{
    NFA nfa;
    int to_int;
    nfa.add_transition(nfa1.transitions.at(0).from_node,
                       nfa1.transitions.at(0).from_node + 1,
                       "\\l");
    for(int i = 0; i < nfa1.transitions.size(); i++)
    {
        nfa.add_transition( nfa1.transitions.at(i).from_node +1,
                            nfa1.transitions.at(i).to_node +1,
                            nfa1.transitions.at(i).arrow_state);
        to_int = nfa1.transitions.at(i).to_node +1;
    }

    nfa.add_transition(to_int,
                       nfa1.transitions.at(0).from_node+1,
                       "\\l");
    nfa.add_transition(to_int,
                       to_int+1,
                       "\\l");
    nfa.set_states(nfa1.get_states()+2);


    nfa.set_finalNode(nfa.get_states()-1);
    return nfa;
}



//////// hnaaaaaaa
NFA Re_to_NFA::finalNFA(vector<NFA> vec)
{
    NFA nfa;
    int to_add = 1;
    int start = 0;
    int flag = 0;
    int last_int=0;
    int num_states = 0;
    for(vector <NFA> :: iterator it = vec.begin(); it != vec.end(); ++it)
    {
        NFA nfa1 = *it;
        num_states += nfa1.get_states();
        if(!start)
        {
            to_add = 1;

        }
        else
        {
            to_add = (nfa.transitions.at(nfa.transitions.size()-1).to_node)+1;
        }
        nfa.add_transition(nfa1.transitions.at(0).from_node,
                           nfa1.transitions.at(0).from_node + to_add,
                           "\\l");
        nfa.set_finalNode(nfa1.transitions.at(0).from_node + to_add);
        for(int i = 0; i < nfa1.transitions.size(); i++)
        {
            nfa.add_transition( nfa1.transitions.at(i).from_node +to_add,
                                nfa1.transitions.at(i).to_node +to_add,
                                nfa1.transitions.at(i).arrow_state);
            last_int = nfa1.transitions.at(i).to_node + to_add;
            nfa.set_finalNode(nfa1.transitions.at(i).from_node + to_add);
        }
        start = 1;
        nfa.acceptable_states.push_back((nfa.transitions.at(nfa.transitions.size()-1)).to_node);
    }
    num_states++;
    nfa.set_states(num_states);

    vector<int> temp_vec = nfa.acceptable_states;
    return nfa;
}

void Re_to_NFA:: convertDef_NFA()
{
    x.readFile();
    map<string,string> def = x.returnDef();
    std::map<std::string, string>::iterator it = def.begin();
    std::stack<char> operators;
    std::stack<NFA> operands;
    while (it != def.end())
    {
        string d = it->second;
        for(int i = 0; i<d.size(); i++)
        {
            char cur = d.at(i);
            if(cur == ' ')
            {
                continue;
            }
            char y;
            int j = 0;
            bool start = false;
            bool inside = false;
            if(i>0)
            {
                y = d.at(i-1);
                if(y == '(')
                {
                    start = true;
                }
            }
            if(cur != '(' && cur != ')' && cur != '*' && cur != '|' && cur != '+')
            {
                y = d.at(i+1);
                if(y == ' ')
                {
                    if(i+2 < d.size())
                    {
                        y = d.at(i+2);
                    }
                }
                if(y != '-')
                {
                    NFA *nfa = new NFA();
                    char *temp = "";
                    temp = appendCharToCharArray(temp,cur);
                    nfa->add_transition(0,1,temp);
                    nfa->set_finalNode(1);
                    nfa->set_states(2);
                    j++;
                    operands.push(*nfa);
                }
                if(i > 0)
                {
                    y = d.at(i-1);
                    if(y == ' ')
                    {
                        if(i-2 >= 0)
                        {
                            y = d.at(i-2);
                        }
                    }
                }
                if (i>0 && !start && inside && !operators.empty()&& operators.top() == '#' && y != '(' && y != '|')
                {
                    operators.pop();
                    NFA nfa2 = operands.top();
                    operands.pop();
                    NFA nfa1 = operands.top();
                    operands.pop();
                    operands.push(concatenate(nfa1, nfa2));

                }
                int check = 0;
                if(i != d.size()-1)
                {
                    y = d.at(i+1);
                    if(y == ' ')
                    {
                        check++;
                        y = d.at(i+2);
                    }
                    if(y == '-')
                    {
                        vector<NFA> orVector;
                        char letter = cur;
                        if(check==0)
                        {
                            y = d.at(i+2);
                        }
                        else if (check == 1)
                        {
                            y = d.at(i+3);
                        }
                        if(y == ' ')
                        {
                            y = d.at(i+3+check);
                            check++;
                        }
                        int s = 0;
                        while (letter >= cur && letter <= y)
                        {
                            NFA *nfa = new NFA();
                            char *temp = "";
                            temp = appendCharToCharArray(temp,letter);
                            nfa->add_transition(0,1,temp);
                            nfa->set_finalNode(1);
                            nfa->set_states(2);
                            j+=2;
                            letter++;
                            orVector.push_back(*nfa);
                        }
                        s = 0;
                        operands.push(unionNFAvec(orVector));
                        i = i+2+check;
                    }
                    y = d.at(i+1);
                    if(y == ' ')
                    {
                        y = d.at(i+2);
                    }
                    //concatenation
                    if(y != '+' && y != '|' && y != ')' && y != '*' && y != '-')
                    {
                        operators.push('#');
                    }
                    else if(y == '+')
                    {
                        NFA nfa1 = operands.top();
                        operands.pop();
                        operands.push(positiveClosure(nfa1));
                        i++;
                        if(i != d.size()-1)
                        {
                            y = d.at(i+1);
                            if(y == ' ')
                            {
                                y = d.at(i+2);
                            }
                            //concatenation
                            if(y != '+' && y != '|' && y != ')' && y != '*')
                            {
                                operators.push('#');
                            }
                        }
                    }
                    else if(y == '*')
                    {
                        NFA nfa1 = operands.top();
                        operands.pop();
                        operands.push(kleeneClosure(nfa1));
                        i++;
                        if(i != d.size()-1)
                        {
                            operators.push('#');
                        }
                    }
                }
            }
            else if (cur == '|')
            {
                operators.push('|');
            }
            else if (cur == ')')
            {
                vector<NFA> orVector2;
                char a = operators.top();
                int count = 0;
                while(a!='(')
                {
                    if(a == '|')
                    {
                        count++;
                    }
                    operators.pop();
                    a = operators.top();
                }

                operators.pop();
                int c = count;
                if(count != 0)
                {
                    for(int k = 0; k<=count; k++)
                    {
                        NFA nfa = operands.top();
                        orVector2.push_back(nfa);
                        operands.pop();
                        c--;
                    }
                }
                if(!orVector2.empty())
                {
                    operands.push(unionNFAvec(orVector2));
                }
                while (!orVector2.empty())
                {
                    orVector2.pop_back();
                }
                inside = false;
                char x;
                if(i != d.size()-1)
                {
                    x = d.at(i+1);
                    bool inc = false;
                    if(x == ' ')
                    {
                        inc = true;
                        if(i+2 < d.size())
                        {
                            x = d.at(i+2);
                        }

                    }
                    if(x == '*')
                    {
                        NFA nfa = operands.top();
                        operands.pop();
                        operands.push(kleeneClosure(nfa));
                        i++;
                        if(inc)
                        {
                            i++;
                        }
                    }
                    else if (x == '+')
                    {
                        NFA nfa = operands.top();
                        operands.pop();
                        operands.push(positiveClosure(nfa));
                        i++;
                        if(inc)
                        {
                            i++;
                        }
                    }
                }
                //concatenation before bracket with the bracket
                while (!operators.empty() && (operators.top() == '|' || operators.top() == '#'))
                {
                    if (operators.top() == '#')
                    {
                        operators.pop();
                        NFA nfa2 = operands.top();
                        operands.pop();
                        NFA nfa1 = operands.top();
                        operands.pop();
                        operands.push(concatenate(nfa1, nfa2));
                    }
                    else if (operators.top() == '|')
                    {
                        operators.pop();
                        NFA nfa2 = operands.top();
                        operands.pop();
                        NFA nfa1 = operands.top();
                        operands.pop();
                        vector<NFA>orVector2;
                        orVector2.push_back(nfa1);
                        orVector2.push_back(nfa2);
                        operands.push(unionNFAvec(orVector2));
                        while (!orVector2.empty())
                        {
                            orVector2.pop_back();
                        }
                    }
                }
            }
            else if (cur == '(')
            {
                operators.push('(');
                inside = true;
            }
            else if (cur == '+')
            {
                //call positive closure
                NFA nfa1 = operands.top();
                operands.pop();
                operands.push(positiveClosure(nfa1));

            }
            else if (cur == '*')
            {
                //kleene closure
                NFA nfa1 = operands.top();
                operands.pop();
                operands.push(kleeneClosure(nfa1));
            }
            if(i == d.size()-1)
            {
                while (!operators.empty() && (operators.top() == '|' || operators.top() == '#'))
                {
                    if (operators.top() == '#')
                    {
                        operators.pop();
                        NFA nfa2 = operands.top();
                        operands.pop();
                        NFA nfa1 = operands.top();
                        operands.pop();
                        operands.push(concatenate(nfa1, nfa2));
                    }
                    else if (operators.top() == '|')
                    {
                        operators.pop();
                        NFA nfa2 = operands.top();
                        operands.pop();
                        NFA nfa1 = operands.top();
                        operands.pop();
                        vector<NFA>orVector2;
                        orVector2.push_back(nfa1);
                        orVector2.push_back(nfa2);
                        operands.push(unionNFAvec(orVector2));
                        while (!orVector2.empty())
                        {
                            orVector2.pop_back();
                        }
                    }
                }
            }
        }
        defToNfa.insert({it->first,operands.top()});
        while(!operands.empty())
        {
            operands.pop();
        }
        while(!operators.empty())
        {
            operators.pop();
        }
        it++;
    }
}

//////////////////   hnaaaaa

NFA Re_to_NFA::unionNFAvec(vector<NFA> vec)
{
    NFA nfa;
    int to_add = 1;
    int start = 0;
    int flag = 0;
    int last_node = 0;
    int last_int=0;
    vector <int> from_vec;
    int num_states = 0;
    for(vector <NFA> :: iterator it = vec.begin(); it != vec.end(); ++it)
    {
        NFA nfa1 = *it;
        num_states += nfa1.get_states();

        if(!flag)
        {
            for(vector <NFA> :: iterator itt = vec.begin(); itt != vec.end(); ++itt)
            {
                NFA temp = *itt;
                last_node += temp.transitions.size()+1;
            }
            last_node++;
            flag = 1;
        }
        if(!start)
        {
            to_add = 1;

        }
        else
        {
            to_add = nfa.final_node+2;
        }
        nfa.add_transition(nfa1.transitions.at(0).from_node,
                           nfa1.transitions.at(0).from_node + to_add,
                           "\\l");
        nfa.set_finalNode(nfa1.transitions.at(0).from_node + to_add);
        for(int i = 0; i < nfa1.transitions.size(); i++)
        {
            nfa.add_transition( nfa1.transitions.at(i).from_node +to_add,
                                nfa1.transitions.at(i).to_node +to_add,
                                nfa1.transitions.at(i).arrow_state);
            last_int = nfa1.transitions.at(i).to_node + to_add;
            nfa.set_finalNode(nfa1.transitions.at(i).from_node + to_add);
        }
        from_vec.push_back(last_int);
        start = 1;
    }
    num_states+=2;
    for(vector <int> :: iterator it = from_vec.begin(); it != from_vec.end(); ++it)
    {
        int j = *it;
        nfa.add_transition(j,
                           last_int+1,"\\l");

    }
    nfa.set_states(num_states);
    nfa.set_finalNode(num_states-1);
    return nfa;
}
vector<string> keys;
vector<NFA> Re_to_NFA::return_keywords(string keywords)
{
    string word = "";
    int from = 0;
    NFA* nfa = new NFA();
    for(int i = 0; i<keywords.size(); i++)
    {
        char temp_char = keywords.at(i);
        if(i==0 && temp_char == ' ')
        {
            continue;
        }
        if(temp_char != ' ')
        {
            char* str = "";
            word += temp_char;
            str = appendCharToChar(str, temp_char);
            nfa->add_transition(from,from+1,str);
            nfa->set_finalNode(from+1);
            from++;

        }
        if(temp_char == ' ' || i == keywords.size()-1)
        {
            keys.push_back(word);
            keywords_vec.push_back(*nfa);
            nfa = new NFA();
            word = "";
            from = 0;
        }

    }
    if(keywords_vec.size() > 1)
    {
        keywords_nfa = unionNFAvec(keywords_vec);
    }
    else
    {
        keywords_nfa = keywords_vec.at(0);
    }
    pair <string,NFA> temp2;
    temp2.first = "keywords";
    temp2.second = keywords_nfa;
    final_map.insert({0, temp2});
    return keywords_vec;
}
vector<string> Re_to_NFA :: returnKeyWords(){
    return keys;
}

vector<NFA> Re_to_NFA::return_panc(pair <int, string >temp_pair)
{
    string panc = temp_pair.second;
    int from = 0;
    NFA* nfa = new NFA();
    for(int i = 0; i<panc.size(); i++)
    {
        char temp_char = panc.at(i);
        if(temp_char != ' ')
        {
            if(temp_char == '\\')
            {
                i++;
                temp_char = panc.at(i);
            }
            char* str = "";
            str = appendCharToChar(str, temp_char);
            nfa->add_transition(from,from+1,str);
            nfa->set_finalNode(from+1);
            from++;
        }
        if(temp_char == ' ' || i == panc.size()-1)
        {
            panc_vec.push_back(*nfa);
            nfa = new NFA();
            from = 0;
        }

    }
    if(panc_vec.size() >1)
    {
        panc_nfa = unionNFAvec(panc_vec);
    }
    else
    {
        panc_nfa = panc_vec.at(0);
    }
    pair <string,NFA> temp2;
    temp2.first = "panctuations";
    temp2.second = panc_nfa;
    final_map.insert({temp_pair.first, temp2});
    return panc_vec;
}
NFA Re_to_NFA::get_keywordsNFA()
{
    return keywords_nfa;
}
NFA Re_to_NFA::get_pancNFA()
{
    return panc_nfa;
}
