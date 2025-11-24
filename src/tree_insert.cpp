#include "tree.h"
#include "list.h"

//--------------------------------------------------------------------------------

tree_err_t 
TreeInsert (TreeContext_t* TreeContext, tree_data_t val, TreeNode_t* ParentNode, tree_data_t cond)
{
    DEBUG_ASSERT (ParentNode  != NULL);
    DEBUG_ASSERT (TreeContext != NULL);

    ParentNode->cond_true  = MyAlloc (TreeContext, 1);
    ParentNode->cond_false = MyAlloc (TreeContext, 1);

    if (ParentNode->cond_false == NULL || ParentNode->cond_true == NULL) {
        PRINTERR (TREE_ALLOC_ERR);
        return   (TREE_ALLOC_ERR);
    }

    ParentNode->cond_false->prev_node = ParentNode;
    ParentNode->cond_true ->prev_node = ParentNode; 

    ParentNode->cond_false->status = WORD;
    ParentNode->cond_true ->status = WORD;
    
    strcpy (ParentNode->cond_false->node_data, ParentNode->node_data);

    strcpy (ParentNode->cond_true ->node_data, val);
    
    strcpy (ParentNode->node_data, cond);
    
    ParentNode->status = COND;
    
    TreeContext->size += 2;
    
    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
MakeNode (TreeContext_t* TreeContext, TreeNode_t** Node)
{
    DEBUG_ASSERT (TreeContext != NULL);

    *Node = MyAlloc (TreeContext, 1);

    if (*Node == NULL) {
        return TREE_ALLOC_ERR;
    }

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------