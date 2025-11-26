#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "tree.h"
#include "common_dump.h"

//————————————————————————————————————————————————————————————————————————————————

struct variable_t {
    char*  name;
    double value;
};

//————————————————————————————————————————————————————————————————————————————————

const int NumOfOper = 7;

//————————————————————————————————————————————————————————————————————————————————

const math_oper_cont_t operations[] = {
    {"ADD" , "+"   },
    {"DIF" , "-"   },
    {"MUL" , "*"   },
    {"DIV" , "/"   },
    {"EXP" , "^"   },
    {"SQRT", "sqrt"},
    {"Log" , "log" },
};

//————————————————————————————————————————————————————————————————————————————————

tree_node_t* 
NewNode (node_type_t type, tree_data_t data, tree_node_t* left, tree_node_t* right);
tree_node_t*
DerivativeNode (tree_node_t* ancestor);
tree_node_t* 
CopyTree (tree_node_t* ancestor);
tree_node_t* 
DerivativeConst (tree_node_t* ancestor);
tree_node_t* 
DerivativeVariable (tree_node_t* ancestor);
tree_node_t* 
DerivativeAddition (tree_node_t* ancestor);
tree_node_t* 
DerivativeDifference (tree_node_t* ancestor);
tree_node_t* 
DerivativeMultiplication (tree_node_t* ancestor);
tree_data_t
MakeOperData(math_oper_t oper);
tree_data_t
MakeDigitData(double value);
int 
CheckForVariables (tree_node_t* node);
//————————————————————————————————————————————————————————————————————————————————

#define ADD_(left, right)  NewNode (operation, MakeOperData (addition),       left, right)
#define DIF_(left, right)  NewNode (operation, MakeOperData (difference),     left, right)
#define MUL_(left, right)  NewNode (operation, MakeOperData (multiplication), left, right)
#define DIV_(left, right)  NewNode (operation, MakeOperData (division),       left, right)
#define EXP_(left, right)  NewNode (operation, MakeOperData (exponentiation), left, right)
#define ROOT_(left, right) NewNode (operation, MakeOperData (root),           left, right)
#define LOG_(left, right)  NewNode (operation, MakeOperData (logarithm),      left, right)
#define SIN_(arg)          NewNode (operation, MakeOperData (sin),    arg, nullptr)
#define COS_(arg)          NewNode (operation, MakeOperData (cos),    arg, nullptr)
#define TG_(arg)           NewNode (operation, MakeOperData (tg),     arg, nullptr)
#define CTG_(arg)          NewNode (operation, MakeOperData (ctg),    arg, nullptr)
#define ARCSIN_(arg)       NewNode (operation, MakeOperData (arcsin), arg, nullptr)
#define ARCCOS_(arg)       NewNode (operation, MakeOperData (arccos), arg, nullptr)
#define ARCTG_(arg)        NewNode (operation, MakeOperData (arctg),  arg, nullptr)
#define ARCCTG_(arg)       NewNode (operation, MakeOperData (arcctg), arg, nullptr)
#define SH_(arg)           NewNode (operation, MakeOperData (sh),     arg, nullptr)
#define CH_(arg)           NewNode (operation, MakeOperData (ch),     arg, nullptr)

#define d_(node) DerivativeNode (node)
#define c_(node) CopyTree       (node)

#define NEW_DATA_NODE(data) NewNode (value, MakeDigitData (data), nullptr, nullptr)


//————————————————————————————————————————————————————————————————————————————————

#endif //CALCULATOR_H