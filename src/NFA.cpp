#include "NFA.h"
#include "vector"


void NFA::add_transition(int from,int to,char* onArrow)
{
    transition t ;
    t.from_node = from;
    t.to_node = to;
    t.arrow_state = onArrow;
    transitions.push_back(t);
    // total_trans.push_back(t);
}
void NFA:: set_states(int n)
{
    no_states = n;
}
int NFA:: get_states()
{
    return no_states ;
}

void NFA ::set_finalNode(int new_node)
{
    final_node = new_node;
}
