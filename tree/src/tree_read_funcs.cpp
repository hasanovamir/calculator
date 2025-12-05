#include "calculator.h"

//--------------------------------------------------------------------------------

tree_node_t*
ReadTree (const char* buffer)
{
    DEBUG_ASSERT (buffer != nullptr);

    int pos = 0;

    return GetG (buffer, &pos);
}

//--------------------------------------------------------------------------------

tree_node_t*
GetP (const char* buffer, int* pos)
{
    if (buffer[*pos] == '(') {
        (*pos)++;

        tree_node_t* node = GetE (buffer, pos);

        if (buffer[*pos] != ')') {
            
            return nullptr;
        }

        (*pos)++;

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
    int prev_pos = *pos;

    int start_pos = *pos;

    while (isdigit (buffer[*pos]) || buffer[*pos] == '.') {
        (*pos)++;
    }

    char* str_end = (char*) buffer + *pos;

    double val = strtod (buffer + start_pos, &str_end);

    if (*pos == prev_pos) {
        return nullptr;
    }

    return NEW_DATA_NODE (val);
}

//--------------------------------------------------------------------------------

tree_node_t*
GetG (const char* buffer, int* pos)
{
    tree_node_t* node = GetE (buffer, pos);

    if (buffer[*pos] != '$') {
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

    for (int i = root_op; i < NumOfOper; i++) {
        if (strncmp (&buffer[*pos], operations[i].oper, 
            strlen (operations[i].oper)) == 0) {
            oper_num = i;
            break;
        }
    }

    if (oper_num != -1) {
        *pos += strlen (operations[oper_num].oper);

        if (buffer[*pos] == '(') {
            (*pos)++;

            if (oper_num == root_op || oper_num == logarithm_op) {
                tree_node_t* left  = GetE (buffer, pos);
                if (buffer[*pos] != ',') return nullptr;
                (*pos)++;
                tree_node_t* right = GetE (buffer, pos);
                if (buffer[*pos] != ')') return nullptr;
                (*pos)++;
                return NewNode (operation, MakeOperData ((math_oper_t) oper_num), left, right);
            }

            tree_node_t* left  = GetE (buffer, pos);

            if (buffer[*pos] != ')') return nullptr;

            (*pos)++;

            return NewNode (operation, MakeOperData ((math_oper_t) oper_num), left, nullptr);
        }
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

    char name[CommonStringSize] = "";

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