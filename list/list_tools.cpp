#include "list.h"

//--------------------------------------------------------------------------------

list_err_t ListGetNext (list_t* list, int idx, int* next_idx)
{
    LIST_VERIFY (list);

    *next_idx = list->next[idx];
    
    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListGetPrev (list_t* list, int idx, int* prev_idx)
{
    LIST_VERIFY (list);

    *prev_idx = list->prev[idx];
    
    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListGetFree (list_t* list, int idx, int* free_idx)
{
    LIST_VERIFY (list);

    *free_idx = list->free;
    
    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListPop (list_t* list, int idx, int* ret_val)
{
    LIST_VERIFY (list);

    *ret_val = list->data[idx];
    
    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListInsertAfter (list_t* list, int idx, int val)
{
    LIST_VERIFY (list);

    if (idx < 0 || idx >= list->capacity)
    {
        PRINTERR (LIST_INVALID_IDX);
        return   (LIST_INVALID_IDX);
    }

    if (list->data[idx] == POISON && idx != 0)
    {
        PRINTERR (LIST_INVALID_IDX);
        return   (LIST_INVALID_IDX);
    }

    int free = list->free;

    if (ChangeFree (list))
    {
        return LIST_UP_SIZE_ERR;
    }

    list->data[free] = val;

    list->prev[free] = idx;
    list->prev[list->next[idx]] = free;

    list->next[free] = list->next[idx];
    list->next[idx]  = free;

    list->head = list->next[0];
    list->tail = list->prev[0];

    list->size++;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListInsertBefore (list_t* list, int idx, int val)
{
    LIST_VERIFY (list);

    if (idx < 0 || idx >= list->capacity)
    {
        PRINTERR (LIST_INVALID_IDX);
        return   (LIST_INVALID_IDX);
    }

    if (list->data[idx] == POISON && idx != 0)
    {
        PRINTERR (LIST_INVALID_IDX);
        return   (LIST_INVALID_IDX);
    }

    int free = list->free;

    if (ChangeFree (list))
    {
        return LIST_UP_SIZE_ERR;
    }

    list->data[free] = val;

    list->next[free] = idx;
    list->next[list->prev[idx]] = free;

    list->prev[free] = list->prev[idx];
    list->prev[idx] = free;

    list->head = list->next[0];
    list->tail = list->prev[0];

    list->size++;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListInsertToStart (list_t* list, int val)
{
    LIST_VERIFY (list);

    int free = list->free;

    if (ChangeFree (list))
    {
        return LIST_UP_SIZE_ERR;
    }

    list->data[free] = val;

    list->next[0] = free;
    list->next[free] = list->head;

    list->prev[free] = 0;
    list->prev[list->head] = free;

    list->head = list->next[0];
    list->tail = list->prev[0];

    list->size++;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListInsertToEnd (list_t* list, int val)
{
    LIST_VERIFY (list);

    int free = list->free;

    if (ChangeFree (list))
    {
        return LIST_UP_SIZE_ERR;
    }

    list->data[free] = val;

    list->next[list->tail] = free;
    list->next[free] = 0;

    list->prev[free] = list->tail;
    list->prev[0] = free;

    list->head = list->next[0];
    list->tail = list->prev[0];

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

list_err_t ListDelete (list_t* list, int idx)
{
    LIST_VERIFY (list);

    list->data[idx] = POISON;
    
    list->next[list->prev[idx]] = list->next[idx];
    list->prev[list->next[idx]] = list->prev[idx];
    list->next[idx] = list->free;
    list->prev[idx] = -1;
    list->free = idx;
    list->size--;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------