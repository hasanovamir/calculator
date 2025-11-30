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

    VariablesInit ();

    tree_node_t* node = GetG (buffer, &pos);

    TreeDump (node);

    OptimizeTree (node);

    TreeDump (node);
}

//--------------------------------------------------------------------------------

tree_node_t*
GetP (const char* buffer, int* pos)
{
    if (buffer[*pos] == '(') {
        (*pos)++;

        tree_node_t* node = GetE (buffer, pos);

        pos++;

        return node;
    }
    else if (isdigit (buffer[*pos])) {
        return GetN (buffer, pos);
    }
    else {
        tree_node_t* node = GetF (buffer, pos);

        if (node == nullptr) node = GetV (buffer, pos);

        return node;
    }
}

//--------------------------------------------------------------------------------

tree_node_t*
GetT (const char* buffer, int* pos)
{
    tree_node_t* node_1 = GetExp (buffer, pos);

    while (buffer[*pos] == '*' || buffer[*pos] == '/') {
        int oper = buffer[*pos];

        (*pos)++;

        tree_node_t* node_2 = GetExp (buffer, pos);

        if (oper == '*') {
            node_1 = NewNode (operation, MakeOperData (multiplication_op), node_1, node_2); 
        }
        else {
            node_1 = NewNode (operation, MakeOperData (division_op), node_1, node_2);
        }
    }

    return node_1;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetE (const char* buffer, int* pos)
{
    tree_node_t* node_1 = GetT (buffer, pos);

    while (buffer[*pos] == '+' || buffer[*pos] == '-') {
        int oper = buffer[*pos];

        (*pos)++;

        tree_node_t* node_2 = GetT (buffer, pos);

        if (oper == '+') {
            node_1 = NewNode (operation, MakeOperData (addition_op), node_1, node_2);
        }
        else {
            node_1 = NewNode (operation, MakeOperData (difference_op), node_1, node_2);
        }
    }

    return node_1;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetN (const char* buffer, int* pos)
{
    tree_node_t* node = NEW_DATA_NODE (0);

    int prev_pos = *pos;

    while (buffer[*pos] >= '0' && buffer[*pos] <= '9') {
        node = NewNode (operation, MakeOperData (addition_op), 
            NewNode (operation, MakeOperData (multiplication_op), node, NEW_DATA_NODE (10)), 
            NEW_DATA_NODE (buffer[*pos] - '0'));

        (*pos)++;
    }

    if (*pos == prev_pos) {
        return nullptr;
    }

    return node;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetG (char* buffer, int* pos)
{
    tree_node_t* node = GetE (buffer, pos);

    if (buffer[*pos] != '$') {
        (*pos)++;

        return nullptr;
    }

    (*pos)++;
    
    return node;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetF (const char* buffer, int* pos)
{
    int oper_num = -1;

    for (int i = 0; i < NumOfOper; i++) {
        if (strncmp (buffer, operations[i].oper, 
            sizeof (operations[i].oper) / sizeof (operations[i].oper[0]))) {
            oper_num = i;
            break;
        }
    }

    if (oper_num != -1) {
        return NewNode (operation, MakeOperData ((math_oper_t) oper_num),
                        GetE (buffer, pos), nullptr);
    }

    return nullptr;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetV (const char* buffer, int* pos)
{
    if (isdigit (buffer[*pos])) {
        return nullptr;
    }

    int i = 0;

    char name[COMMONSTRINGSIZE] = "";

    while (isdigit (buffer[*pos]) || isalpha (buffer[*pos]) || buffer[*pos] == '_') {
        name[i] = buffer[*pos];

        i++;

        (*pos)++;
    }

    int number = GetVarNumber (name);

    if (number == -1) number = AddVar (name);

    return NewNode (var_num, MakeVarData (number), nullptr, nullptr);
}

//--------------------------------------------------------------------------------

tree_node_t*
GetA (const char* buffer, int* pos)
{
    return NewNode (operation, MakeOperData (equality_op), GetV (buffer, pos), 
                                                           GetE (buffer, pos));
}

//--------------------------------------------------------------------------------

tree_node_t*
GetExp (const char* buffer, int* pos)
{
    tree_node_t* node = GetP (buffer, pos);

    while (buffer[*pos] == '^') {
        (*pos)++;

        tree_node_t* node_2 = GetP (buffer, pos);

        node = NewNode (operation, MakeOperData (exponentiation_op), node, node_2);
    }

    return node;
}

//--------------------------------------------------------------------------------

tree_node_t*
GetRoot (const char* buffer, int* pos)
{
    if (strncmp (&buffer[*pos], "root", 4) == 0) {
        (*pos) += 4;
        
        if (buffer[*pos] == '(') {
            (*pos)++;

            tree_node_t* node_1 = GetE (buffer, pos);

            if (buffer[*pos] != ',') {
                return nullptr;
            }

            (*pos)++;

            tree_node_t* node_2 = GetE (buffer, pos);
            
            if (buffer[*pos] != ')') {
                return nullptr;
            }

            (*pos)++;
            
            return NewNode (operation, MakeOperData (root_op), node_1, node_2);
        }
    }

    return nullptr;
}

//--------------------------------------------------------------------------------

