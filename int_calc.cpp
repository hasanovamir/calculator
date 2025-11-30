#include "calculator.h"

//--------------------------------------------------------------------------------

tree_node_t*
GetP (char* buffer, int* pos);
tree_node_t*
GetT (char* buffer, int* pos);
tree_node_t*
GetE (char* buffer, int* pos);
tree_node_t*
GetN (char* buffer, int* pos);
tree_node_t*
GetG (char* buffer, int* pos);

//--------------------------------------------------------------------------------

int 
main ()
{
    char buffer[100] = "";

    int pos = 0;

    scanf ("%s", buffer);

    ClearDump ();

    if (TreeInit ()) {
        return TREE_ALLOC_ERR;
    }

    tree_node_t* node = GetG (buffer, &pos);

    TreeDump (node);

    OptimizeTree (node);

    TreeDump (node);
}

//--------------------------------------------------------------------------------

tree_node_t*
GetP (char* buffer, int* pos)
{
    if (buffer[*pos] == '(') {
        (*pos)++;

        tree_node_t* val = GetE (buffer, pos);

        pos++;

        return val;
    }
    else {
        return GetN (buffer, pos);
    }
}

//--------------------------------------------------------------------------------

tree_node_t*
GetT (char* buffer, int* pos)
{
    tree_node_t* val_1 = GetP (buffer, pos);

    while (buffer[*pos] == '*' || buffer[*pos] == '/') {
        int oper = buffer[*pos];

        (*pos)++;

        tree_node_t* val_2 = GetP (buffer, pos);

        if (oper == '*') {
            val_1 = NewNode (operation, MakeOperData (multiplication_op), val_1, val_2); 
        }
        else {
            val_1 = NewNode (operation, MakeOperData (division_op), val_1, val_2);
        }
    }

    return val_1;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetE (char* buffer, int* pos)
{
    tree_node_t* val_1 = GetT (buffer, pos);

    while (buffer[*pos] == '+' || buffer[*pos] == '-') {
        int oper = buffer[*pos];

        (*pos)++;

        tree_node_t* val_2 = GetT (buffer, pos);

        if (oper == '+') {
            val_1 = NewNode (operation, MakeOperData (addition_op), val_1, val_2);
        }
        else {
            val_1 = NewNode (operation, MakeOperData (difference_op), val_1, val_2);
        }
    }

    return val_1;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetN (char* buffer, int* pos)
{
    tree_node_t* val = NEW_DATA_NODE (0);

    int prev_pos = *pos;

    while (buffer[*pos] >= '0' && buffer[*pos] <= '9') {
        val = NewNode (operation, MakeOperData (addition_op), 
            NewNode (operation, MakeOperData (multiplication_op), val, NEW_DATA_NODE (10)), 
            NEW_DATA_NODE (buffer[*pos] - '0'));

        (*pos)++;
    }

    if (*pos == prev_pos) {
        return nullptr;
    }

    return val;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetG (char* buffer, int* pos)
{
    tree_node_t* val = GetE (buffer, pos);

    if (buffer[*pos] != '$') {
        (*pos)++;

        return nullptr;
    }

    (*pos)++;
    
    return val;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetF (const char* buffer, int* pos)
{
    int oper_num = -1;

    for (int i = 0; i < NumOfOper; i++) {
        if (strncmp (buffer, operations[i].oper, sizeof (operations[i].oper) / sizeof (operations[i].oper[0]))) {
            oper_num = i;
            break;
        }
    }

    if (oper_num != -1) {
        return NewNode (operation, MakeOperData (oper_num), GetE (buffer, pos), nullptr);
    }

    return nullptr;
}

//--------------------------------------------------------------------------------