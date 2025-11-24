#include "tree.h"


//-----------------------------------------------------------------------------

tree_err_t 
TreeInit (TreeContext_t* TreeContext)
{
    DEBUG_ASSERT (TreeContext != NULL);

    InitBigArray (TreeContext);

    TreeContext->head = MyAlloc (TreeContext, 1);

    if (TreeContext->head == NULL) {
        return TREE_ALLOC_ERR;
    }

    strcpy (TreeContext->head->node_data, "nothing");

    TreeContext->head->idx    = 0;
    TreeContext->head->status = WORD;

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

TreeNode_t*
MyAlloc (TreeContext_t* TreeContext, int count)
{
    DEBUG_ASSERT (TreeContext != NULL);

    MyAllocContext_t* AllocContext = TreeContext->MyAllocContext;

    if (AllocContext->pos_in_cur_arr + count >= ALLOCCAPACITY)
    {
        if (ChangeSrcArray (TreeContext)) {
            return NULL;
        }
    }

    list_t* list = &AllocContext->BigArray[AllocContext->cur_array].list_node_src; 

    int free_idx = list->free;

    TreeNode_t* new_node = &(list->data[free_idx]);

    for (int i = 0; i < count; i++) {
        free_idx = list->free;

        list->free = list->next[free_idx];

        AllocContext->src_size++;

        AllocContext->pos_in_cur_arr++;
    }

    return new_node;
}

//--------------------------------------------------------------------------------

void 
TreeDestroy (TreeContext_t* TreeContext)
{
    DEBUG_ASSERT (TreeContext != nullptr);

    for (int i = 0; i < TreeContext->MyAllocContext->num_src_arr; i++) {
        ListDestroy (&TreeContext->MyAllocContext->BigArray[i].list_node_src);

        free (TreeContext->MyAllocContext->BigArray[i].string_src);
    }

    free (TreeContext->MyAllocContext->BigArray);
}

//--------------------------------------------------------------------------------

tree_err_t 
AkinatorStart ( TreeContext_t* TreeContext )
{
    if ( TreeInit ( TreeContext ) ) {
        return TREE_ALLOC_ERR;
    }

    if ( TreeReadDataBase ( TreeContext, "tree_data_base.txt" ) ) {
        return TREE_ALLOC_ERR;
    }

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
InitBigArray (TreeContext_t* TreeContext)
{
    DEBUG_ASSERT (TreeContext != NULL);

    BigArrayData_t* BigArray_ptr = (BigArrayData_t*) calloc (STARTBIGARRAYCAP, sizeof (BigArrayData_t));

    if (BigArray_ptr == NULL) {
        PRINTERR (TREE_ALLOC_ERR);
        return TREE_ALLOC_ERR;
    }

    for (int i = 0; i < STARTBIGARRAYCAP; i++) {
        if (ListInit (&BigArray_ptr[i].list_node_src, ALLOCCAPACITY)) {
            return TREE_ALLOC_ERR;
        }

        BigArray_ptr[i].string_src = (char*) calloc (ALLOCCAPACITY * COMMONSTRINGSIZE, sizeof (char));

        if (BigArray_ptr[i].string_src == NULL) {
            PRINTERR (TREE_ALLOC_ERR);
            return TREE_ALLOC_ERR;
        }

        for (int j = 0; j < ALLOCCAPACITY; j++) {
            BigArray_ptr[i].list_node_src.data[j].node_data = 
                BigArray_ptr[i].string_src + j * COMMONSTRINGSIZE; //put ptr_to_str in node

            BigArray_ptr[i].list_node_src.data[j].idx = i * ALLOCCAPACITY + j;
        }
    }

    MyAllocContext_t* AllocContext = TreeContext->MyAllocContext;

    AllocContext->BigArray = BigArray_ptr;

    AllocContext->src_cap = STARTBIGARRAYCAP * ALLOCCAPACITY;

    AllocContext->num_src_arr = STARTBIGARRAYCAP;

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
ChangeSrcArray (TreeContext_t* TreeContext)
{
    DEBUG_ASSERT (TreeContext != NULL);

    int num_arr = TreeContext->MyAllocContext->num_src_arr;

    MyAllocContext_t* AllocContext = TreeContext->MyAllocContext;

    if (AllocContext->cur_array == num_arr - 1) {
        BigArrayData_t* tmp = (BigArrayData_t*) realloc (
            AllocContext->BigArray, 2 * num_arr * sizeof (BigArrayData_t));

        if (tmp == NULL) {
            PRINTERR (TREE_ALLOC_ERR);
            return TREE_ALLOC_ERR;
        }

        char*       char_tmp = NULL;
        TreeNode_t* node_tmp = NULL;

        for (int i = 0; i < num_arr; i++) {
            ListInit (&AllocContext->BigArray[i + num_arr].list_node_src, ALLOCCAPACITY);

            node_tmp = AllocContext->BigArray[i + num_arr].list_node_src.data;

            char_tmp = (char*) calloc (ALLOCCAPACITY * COMMONSTRINGSIZE, sizeof (char));

            if (char_tmp == NULL || node_tmp == NULL) {
                PRINTERR (TREE_ALLOC_ERR);
                return TREE_ALLOC_ERR;
            }

            for (int j = 0; j < ALLOCCAPACITY; j++) {
                node_tmp[j].node_data = char_tmp + j * COMMONSTRINGSIZE; //put ptr_to_str in node

                strcpy (node_tmp[j].node_data, STR_POISON);

                node_tmp[j].idx = (i + AllocContext->src_cap) * ALLOCCAPACITY + j;
            }

            AllocContext->BigArray[i + num_arr].list_node_src.data = node_tmp;
            
            AllocContext->BigArray[i + num_arr].string_src = char_tmp;
        }

        AllocContext->num_src_arr *= 2;

        AllocContext->src_cap *= 2;
    }

    AllocContext->cur_array++;

    AllocContext->pos_in_cur_arr = 0;

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------