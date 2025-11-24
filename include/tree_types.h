#ifndef TREE_TYPES_H
#define TREE_TYPES_H

//————————————————————————————————————————————————————————————————————————————————

typedef char* tree_data_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    TREE_SUCCESS,
    TREE_OPEN_FILE_ERR,
    TREE_SYS_FUNC_ERR,
    TREE_ALLOC_ERR,
    TREE_INVALID_FILE_SIZE,
    TREE_NO_WORD_IN_SPACE,
} tree_err_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    COND = 0,
    WORD = 1,
} tree_data_val_t;

//————————————————————————————————————————————————————————————————————————————————

struct TreeNode_t {
    int         idx;
    tree_data_t node_data;
    TreeNode_t* prev_node;
    TreeNode_t* cond_true;
    TreeNode_t* cond_false;
    tree_data_val_t status;
};

//————————————————————————————————————————————————————————————————————————————————

#endif //TREE_TYPES_H