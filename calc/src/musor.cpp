#include "calculator.h"

//--------------------------------------------------------------------------------

tree_data_t
MakeOperData(math_oper_t oper)
{
    tree_data_t data;
    
    data.operation = oper;
    
    return data;
}

//--------------------------------------------------------------------------------

tree_data_t
MakeDigitData(double value)
{
    tree_data_t data;

    data.immediate = value;

    return data;
}

//--------------------------------------------------------------------------------

int 
CheckForVariables (tree_node_t* node)
{
    if (node->node_data.var_number != 0) {
        return 1;
    }

    if (node->left_node) {
        if (CheckForVariables (node->left_node)) {
            return 1;
        }
    }

    if (node->right_node) {
        if (CheckForVariables (node->right_node)) {
            return 1;
        }
    }

    return 0;
}

//--------------------------------------------------------------------------------