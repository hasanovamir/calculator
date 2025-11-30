#include "calculator.h"

//--------------------------------------------------------------------------------

tree_err_t 
CalculatorInit (const char* file_name)
{
    ClearDump ();

    if (TreeInit ()) {
        return TREE_ALLOC_ERR;
    }

    if (TreeReadDataBase (file_name)) {
        return TREE_INVALID_FILE_SIZE;
    }

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