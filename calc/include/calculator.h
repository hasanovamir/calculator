#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "tree.h"
#include "common_dump.h"

//————————————————————————————————————————————————————————————————————————————————

struct variable_t {
    char*  name;
    int    number;
    double value;
};

//————————————————————————————————————————————————————————————————————————————————



//————————————————————————————————————————————————————————————————————————————————

const math_oper_cont_t operations[] = {
    {"ADD"   , "+"     },
    {"DIF"   , "-"     },
    {"MUL"   , "*"     },
    {"DIV"   , "/"     },
    {"EXP"   , "^"     },
    {"SQRT"  , "sqrt"  },
    {"Log"   , "log"   },
    {"SIN"   , "sin"   },
    {"COS"   , "cos"   },
    {"TG"    , "tg"    },
    {"CTG"   , "ctg"   },
    {"ARCSIN", "arcsin"},
    {"ARCCOS", "arccos"},
    {"ARCTG" , "arctg" },
    {"ARCCTG", "arcctg"},
    {"SH"    , "sh"    },
    {"CH"    , "ch"    },
};

//————————————————————————————————————————————————————————————————————————————————

const int NumOfOper = sizeof (operations) / sizeof (*operations);

//————————————————————————————————————————————————————————————————————————————————

int         CheckForVariables        (tree_node_t* node);
int         OptimizeTree             (tree_node_t* node);
int         OptimizeTreeIfBothConst  (tree_node_t* node);
int         OptimizeTreeIfRightConst (tree_node_t* node);
int         OptimizeTreeIfLeftConst  (tree_node_t* node);
int         CheckToEquality          (double value_1, double value_2);
void        ClearDump                (void);
void        CopyNodeData             (tree_node_t* destination_node, tree_node_t* source_node);
tree_data_t MakeDigitData            (double value);
tree_data_t MakeOperData             (math_oper_t oper);
tree_err_t  CalculatorInit           (const char* file_name);

//————————————————————————————————————————————————————————————————————————————————

tree_node_t* DifferentiateNode           (tree_node_t* node);
tree_node_t* CopyTree                    (tree_node_t* node);
tree_node_t* DifferentiateConst          (tree_node_t* node);
tree_node_t* DifferentiateVariable       (tree_node_t* node);
tree_node_t* DifferentiateAddition       (tree_node_t* node);
tree_node_t* DifferentiateDifference     (tree_node_t* node);
tree_node_t* DifferentiateMultiplication (tree_node_t* node);
tree_node_t* DifferentiateDivision       (tree_node_t* node);
tree_node_t* DifferentiateExponential    (tree_node_t* node);
tree_node_t* DifferentiateRoot           (tree_node_t* node);
tree_node_t* DifferentiateLog            (tree_node_t* node);
tree_node_t* DifferentiateSin            (tree_node_t* node);
tree_node_t* DifferentiateCos            (tree_node_t* node);
tree_node_t* DifferentiateTg             (tree_node_t* node);
tree_node_t* DifferentiateCtg            (tree_node_t* node);
tree_node_t* DifferentiateArcsin         (tree_node_t* node);
tree_node_t* DifferentiateArccos         (tree_node_t* node);
tree_node_t* DifferentiateArctg          (tree_node_t* node);
tree_node_t* DifferentiateArcctg         (tree_node_t* node);
tree_node_t* DifferentiateSh             (tree_node_t* node);
tree_node_t* DifferentiateCh             (tree_node_t* node);
tree_node_t* NewNode                     (node_type_t type, tree_data_t data, 
                                          tree_node_t* left, tree_node_t* right);

//————————————————————————————————————————————————————————————————————————————————

#define d_(node) DifferentiateNode (node)
#define c_(node) CopyTree          (node)

#define ADD_(left, right)   NewNode (operation, MakeOperData (addition_op),       left, right)
#define DIF_(left, right)   NewNode (operation, MakeOperData (difference_op),     left, right)
#define MUL_(left, right)   NewNode (operation, MakeOperData (multiplication_op), left, right)
#define DIV_(left, right)   NewNode (operation, MakeOperData (division_op),       left, right)
#define EXP_(left, right)   NewNode (operation, MakeOperData (exponentiation_op), left, right)
#define ROOT_(left, right)  NewNode (operation, MakeOperData (root_op),           left, right)
#define LOG_(left, right)   NewNode (operation, MakeOperData (logarithm_op),      left, right)
#define SIN_(node)          NewNode (operation, MakeOperData (sin_op),    node, nullptr)
#define COS_(node)          NewNode (operation, MakeOperData (cos_op),    node, nullptr)
#define TG_(node)           NewNode (operation, MakeOperData (tg_op),     node, nullptr)
#define CTG_(node)          NewNode (operation, MakeOperData (ctg_op),    node, nullptr)
#define ARCSIN_(node)       NewNode (operation, MakeOperData (arcsin_op), node, nullptr)
#define ARCCOS_(node)       NewNode (operation, MakeOperData (arccos_op), node, nullptr)
#define ARCTG_(node)        NewNode (operation, MakeOperData (arctg_op),  node, nullptr)
#define ARCCTG_(node)       NewNode (operation, MakeOperData (arcctg_op), node, nullptr)
#define SH_(node)           NewNode (operation, MakeOperData (sh_op),     node, nullptr)
#define CH_(node)           NewNode (operation, MakeOperData (ch_op),     node, nullptr)

#define NEW_DATA_NODE(data) NewNode (constant, MakeDigitData (data),   nullptr, nullptr)


//————————————————————————————————————————————————————————————————————————————————

#endif //CALCULATOR_H