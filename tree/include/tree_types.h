#ifndef TREE_TYPES_H
#define TREE_TYPES_H

//————————————————————————————————————————————————————————————————————————————————

typedef enum {
    addition_op       = 0,
    difference_op     = 1,
    multiplication_op = 2,
    division_op       = 3,
    exponentiation_op = 4,
    root_op           = 5,
    logarithm_op      = 6,
    sin_op            = 7,
    cos_op            = 8,
    tg_op             = 9,
    ctg_op            = 10,
    arcsin_op         = 11,
    arccos_op         = 12,
    arctg_op          = 13,
    arcctg_op         = 14,
    sh_op             = 15,
    ch_op             = 16,
} math_oper_t;

//————————————————————————————————————————————————————————————————————————————————

struct math_oper_cont_t {
    const char* oper_name;
    const char* oper;
};

//————————————————————————————————————————————————————————————————————————————————

typedef union {
    int         var_number;
    double      immediate;
    math_oper_t operation;
} tree_data_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    TREE_SUCCESS,
    TREE_OPEN_FILE_ERR,
    TREE_SYS_FUNC_ERR,
    TREE_ALLOC_ERR,
    TREE_INVALID_FILE_SIZE,
    TREE_NO_WORD_IN_SPACE,
    TREE_UNKNOWN_DATA_TYPE,
} tree_err_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    operation = 0,
    constant  = 1,
    var_num   = 2,
} node_type_t;

//————————————————————————————————————————————————————————————————————————————————

struct tree_node_t {
    int          idx;
    node_type_t  type;
    tree_data_t  node_data;
    tree_node_t* prev_node;
    tree_node_t* left_node;
    tree_node_t* right_node;
};

//————————————————————————————————————————————————————————————————————————————————

#endif //TREE_TYPES_H