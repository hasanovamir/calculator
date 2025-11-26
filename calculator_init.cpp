#include "calculator.h"

//--------------------------------------------------------------------------------

tree_err_t 
CalculatorInit ()
{

}

//--------------------------------------------------------------------------------

tree_node_t* 
NewNode (node_type_t type, tree_data_t data, tree_node_t* left, tree_node_t* right)
{
    tree_node_t* new_node = nullptr;

    MakeNode (&new_node);

    new_node->type = type;

    new_node->node_data = data;

    new_node->left_node  = left;
    new_node->right_node = right;

    return new_node;
}

//--------------------------------------------------------------------------------