#ifndef TREE_TYPES_H
#define TREE_TYPES_H

//————————————————————————————————————————————————————————————————————————————————

typedef enum {
    addition       = 1,
    difference     = 2,
    multiplication = 3,
    division       = 4,
    exponentiation = 5,
    root           = 6,
    logarithm      = 7,
    sin            = 8,
    cos            = 9,
    tg             = 10,
    ctg            = 11,
    arcsin         = 12,
    arccos         = 13,
    arctg          = 14,
    arcctg         = 15,
    sh             = 16,
    ch             = 17,
} math_oper_t;

//————————————————————————————————————————————————————————————————————————————————

struct math_oper_cont_t {
    const char* oper_name;
    const char* oper;
};

//————————————————————————————————————————————————————————————————————————————————

typedef union {
    int         var_number;
    double      data;
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
    value     = 1,
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