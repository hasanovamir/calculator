#ifndef CALCULATOR_H
#define CALCULATOR_H

//————————————————————————————————————————————————————————————————————————————————

#include "tree.h"
#include "common_dump.h"

//————————————————————————————————————————————————————————————————————————————————

const int StartVarCap = 10;

//————————————————————————————————————————————————————————————————————————————————

struct variable_t {
    char*  name;
    int    number;
    double value;
};

//————————————————————————————————————————————————————————————————————————————————

struct variable_ctx {
    variable_t* variable_arr;
    int         count;
    int         capacity;
};

//————————————————————————————————————————————————————————————————————————————————

extern variable_ctx variables_ctx;

//————————————————————————————POSHEL_NAHUI————————————————————————————————————————————————————

void         ClearDump                  (void);
void         OptimizeTree               (tree_node_t* node);
void         OptimizeTreeIfBothConst    (tree_node_t* node);
void         OptimizeTreeIfRightConst   (tree_node_t* node);
void         OptimizeTreeIfLeftConst    (tree_node_t* node);
void         OptimizeFunctionOfConstant (tree_node_t* node);
void         CopyNodeData               (tree_node_t* destination_node, 
                                         tree_node_t* source_node    );
int          AddVar                     (const char*  name);
int          GetVarNumber               (const char*  name);
int          CheckForVariables          (tree_node_t* node);
int          CheckToEquality            (double value_1, double value_2);
tree_err_t   VariablesInit              (void);
tree_data_t  MakeVarData                (int         oper_num );
tree_data_t  MakeDigitData              (double      value    );
tree_data_t  MakeOperData               (math_oper_t oper     );
tree_err_t   CalculatorInit             (const char* file_name);

//——————————————————————————Differentiate funcs——————————————————————————————————————————————————————

tree_node_t* CopyTree                    (tree_node_t* node);
tree_node_t* DifferentiateConst          (tree_node_t* node, int arg);
tree_node_t* DifferentiateAddition       (tree_node_t* node, int arg);
tree_node_t* DifferentiateDifference     (tree_node_t* node, int arg);
tree_node_t* DifferentiateMultiplication (tree_node_t* node, int arg);
tree_node_t* DifferentiateDivision       (tree_node_t* node, int arg);
tree_node_t* DifferentiateExponential    (tree_node_t* node, int arg);
tree_node_t* DifferentiateRoot           (tree_node_t* node, int arg);
tree_node_t* DifferentiateLog            (tree_node_t* node, int arg);
tree_node_t* DifferentiateSin            (tree_node_t* node, int arg);
tree_node_t* DifferentiateCos            (tree_node_t* node, int arg);
tree_node_t* DifferentiateTg             (tree_node_t* node, int arg);
tree_node_t* DifferentiateCtg            (tree_node_t* node, int arg);
tree_node_t* DifferentiateArcsin         (tree_node_t* node, int arg);
tree_node_t* DifferentiateArccos         (tree_node_t* node, int arg);
tree_node_t* DifferentiateArctg          (tree_node_t* node, int arg);
tree_node_t* DifferentiateArcctg         (tree_node_t* node, int arg);
tree_node_t* DifferentiateSh             (tree_node_t* node, int arg);
tree_node_t* DifferentiateCh             (tree_node_t* node, int arg);
tree_node_t* DifferentiateNode           (tree_node_t* node, int arg);
tree_node_t* DifferentiateVariable       (tree_node_t* node, int arg);
tree_node_t* NewNode                     (node_type_t type, tree_data_t data, 
                                          tree_node_t* left, tree_node_t* right);

//——————————————————————————macros to Differentiate——————————————————————————————————————————————————————

#define d_(node, argument)  DifferentiateNode (node, argument)
#define c_(node)            CopyTree          (node)

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


const math_oper_cont_t operations[] = {
    {"ADD"   , "+"     , &DifferentiateAddition      },
    {"DIF"   , "-"     , &DifferentiateDifference    },
    {"MUL"   , "*"     , &DifferentiateMultiplication},
    {"DIV"   , "/"     , &DifferentiateDivision      },
    {"EXP"   , "^"     , &DifferentiateExponential   },
    {"SQRT"  , "sqrt"  , &DifferentiateRoot          },
    {"Log"   , "log"   , &DifferentiateLog           },
    {"SIN"   , "sin"   , &DifferentiateSin           },
    {"COS"   , "cos"   , &DifferentiateCos           },
    {"TG"    , "tg"    , &DifferentiateTg            },
    {"CTG"   , "ctg"   , &DifferentiateCtg           },
    {"ARCSIN", "arcsin", &DifferentiateArcsin        },
    {"ARCCOS", "arccos", &DifferentiateArccos        },
    {"ARCTG" , "arctg" , &DifferentiateArctg         },
    {"ARCCTG", "arcctg", &DifferentiateArcctg        },
    {"SH"    , "sh"    , &DifferentiateSh            },
    {"CH"    , "ch"    , &DifferentiateCh            },
};

//————————————————————————————————————————————————————————————————————————————————

const int NumOfOper = sizeof (operations) / sizeof (*operations);

//————————————————————————————————————————————————————————————————————————————————

#endif //CALCULATOR_H