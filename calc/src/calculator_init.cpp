#include "calculator.h"

//--------------------------------------------------------------------------------

variable_ctx variables_ctx = {};

//--------------------------------------------------------------------------------

tree_err_t 
CalculatorInit (const char* file_name)
{
    ClearDump ();

    if (TreeInit ()) {
        return TREE_ALLOC_ERR;
    }

    VariablesInit ();

    return TREE_SUCCESS;
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

void 
ClearDump (void)
{
    char command_1[COMMONSTRINGSIZE] = "";
    char command_2[COMMONSTRINGSIZE] = "";
    char command_3[COMMONSTRINGSIZE] = "";
    char command_4[COMMONSTRINGSIZE] = "";

    snprintf (command_1, COMMONSTRINGSIZE, "rm svg/*.svg");
    snprintf (command_2, COMMONSTRINGSIZE, "rm dot/*.dot");
    snprintf (command_3, COMMONSTRINGSIZE, "rm TeX/*.tex");
    snprintf (command_4, COMMONSTRINGSIZE, "rm pdf/*.pdf");

    system (command_1);
    system (command_2);
    system (command_3);
    system (command_4);
}

//--------------------------------------------------------------------------------

tree_err_t
VariablesInit (void)
{
    variables_ctx.variable_arr = (variable_t*) calloc (StartVarCap, sizeof (variable_t));

    if (variables_ctx.variable_arr == nullptr) {
        PRINTERR (TREE_ALLOC_ERR);
        return TREE_ALLOC_ERR;
    }

    for (int i = 0; i < StartVarCap; i++) {
        variables_ctx.variable_arr[i].name = (char*) calloc (COMMONSTRINGSIZE, sizeof (char));

        if (variables_ctx.variable_arr[i].name == nullptr) {
            PRINTERR (TREE_ALLOC_ERR);
        return TREE_ALLOC_ERR;
        }
    }

    variables_ctx.capacity = StartVarCap;

    return TREE_SUCCESS;
}

//————————————————————————————————————————————————————————————————————————————————