#include "calculator.h"

//--------------------------------------------------------------------------------

tree_data_t
MakeOperData (math_oper_t oper)
{
    tree_data_t data;
    
    data.operation = oper;
    
    return data;
}

//--------------------------------------------------------------------------------

tree_data_t
MakeDigitData (double value)
{
    tree_data_t data;

    data.immediate = value;

    return data;
}

//--------------------------------------------------------------------------------

tree_data_t
MakeVarData (int oper_num)
{
    tree_data_t data;
    
    data.var_number = oper_num;
    
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

int
GetVarNumber (const char* name)
{
    for (int i = 0; i < variables_ctx.count; i++) {
        if (strcmp (name, variables_ctx.variable_arr[i].name) == 0) {
            return i;
        }
    }

    return -1;
}

//--------------------------------------------------------------------------------

int
AddVar (const char* name)
{
    strcpy (variables_ctx.variable_arr[variables_ctx.count].name, name);
printf ("ADDVAR count = %d, name = %s\n", variables_ctx.count, name);
    variables_ctx.count++;

    return variables_ctx.count - 1;
}

//--------------------------------------------------------------------------------