#include "tree.h"

//--------------------------------------------------------------------------------

tree_err_t 
TreeInsert (tree_data_t val, tree_node_t* ParentNode, tree_data_t cond)
{
    DEBUG_ASSERT (ParentNode  != NULL);
    ParentNode->left_node  = MyAlloc ();
    ParentNode->right_node = MyAlloc ();

    if (ParentNode->right_node == NULL || ParentNode->left_node == NULL) {
        PRINTERR (TREE_ALLOC_ERR);
        return   (TREE_ALLOC_ERR);
    }

    ParentNode->right_node->prev_node = ParentNode;
    ParentNode->left_node ->prev_node = ParentNode; 
    
    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
MakeNode (tree_node_t** node)
{
    *node = MyAlloc ();

    if (*node == NULL) {
        return TREE_ALLOC_ERR;
    }

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

void 
TreeDeleteBranch (tree_node_t* node)
{
    if (node->left_node) {
        TreeDeleteBranch (node->left_node);
    }

    if (node->right_node) {
        TreeDeleteBranch (node->right_node);
    }

    MyFree (node);
}

//--------------------------------------------------------------------------------