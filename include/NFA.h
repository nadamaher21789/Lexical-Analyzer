#ifndef NFA_H
#define NFA_H
#include<vector>
using namespace std;
class NFA
{
public:
    int type;
    int no_states = 0;
    struct node
    {
        int state_num;
    };

    struct transition
    {
        int from_node;
        int to_node;
        char* arrow_state;
    };
    vector<transition> transitions;
    vector<int> acceptable_states;
    int final_node;
    node start_node;
    // vector<transition> total_trans;

    void insert_node(node new_node);
    void set_finalNode(int new_node);
    void set_startNode(node new_node);
    void add_transition(int from,int to,char* onArrow);
    void set_states(int n);
    int get_states();
//    string get_type();
};

#endif // NFA_H
