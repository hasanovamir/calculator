#include "list.h"

//--------------------------------------------------------------------------------

list_err_t ListGetNext (list_t* list, int idx, int* next_idx)
{
    *next_idx = list->next[idx];
    
    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListGetFree (list_t* list, int idx, int* free_idx)
{
    *free_idx = list->free;
    
    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListPop (list_t* list, int idx, int* ret_val)
{
    *ret_val = list->data[idx].node_data.immediate;
    
    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListInsertAfter (list_t* list, int idx, int val)
{
    if (idx < 0 || idx >= list->capacity)
    {
        PRINTERR (LIST_INVALID_IDX);
        return   (LIST_INVALID_IDX);
    }

    if (list->data[idx].node_data.immediate == POISON && idx != 0)
    {
        PRINTERR (LIST_INVALID_IDX);
        return   (LIST_INVALID_IDX);
    }

    int free = list->free;

    if (ChangeFree (list))
    {
        return LIST_UP_SIZE_ERR;
    }

    list->data[free].node_data.immediate = val;

    list->next[free] = list->next[idx];
    list->next[idx]  = free;

    list->head = list->next[0];

    list->size++;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListInsertBefore (list_t* list, int idx, int val)
{
    if (idx < 0 || idx >= list->capacity)
    {
        PRINTERR (LIST_INVALID_IDX);
        return   (LIST_INVALID_IDX);
    }

    if (list->data[idx].node_data.immediate == POISON && idx != 0)
    {
        PRINTERR (LIST_INVALID_IDX);
        return   (LIST_INVALID_IDX);
    }

    int free = list->free;

    if (ChangeFree (list))
    {
        return LIST_UP_SIZE_ERR;
    }

    list->data[free].node_data.immediate = val;

    list->next[free] = idx;


    list->head = list->next[0];

    list->size++;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListInsertToStart (list_t* list, int val)
{
    int free = list->free;

    if (ChangeFree (list))
    {
        return LIST_UP_SIZE_ERR;
    }

    list->data[free].node_data.immediate = val;

    list->next[0] = free;
    list->next[free] = list->head;

    list->head = list->next[0];

    list->size++;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListInsertToEnd (list_t* list, int val)
{
    int free = list->free;

    if (ChangeFree (list))
    {
        return LIST_UP_SIZE_ERR;
    }

    list->data[free].node_data.immediate = val;

    list->next[list->tail] = free;
    list->next[free] = 0;


    list->head = list->next[0];

    list->size++;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ChangeFree (list_t* list)
{
    DEBUG_ASSERT (list != NULL);

    list->free = list->next[list->free];

    if (list->free == 0)
    {
        if (IncreaseList (list))
        {
            return LIST_UP_SIZE_ERR;
        }
    }

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------