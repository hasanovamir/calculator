#include "list.h"

//--------------------------------------------------------------------------------

list_err_t ListInit (list_t* list, int capacity)
{
    DEBUG_ASSERT (list != NULL);

    list->free = 1;
    list->head = 0;
    list->tail = 0;
    list->capacity = capacity;

    if (capacity < 0)
    {
        PRINTERR (LIST_INVALID_CAPACITY_SIZE);
        return   (LIST_INVALID_CAPACITY_SIZE);
    }

    list->data = (list_data_t*) calloc (capacity, sizeof (list_data_t));

    if (list->data == NULL)
    {
        PRINTERR (LIST_ALLOCATE_ERR);
        return   (LIST_ALLOCATE_ERR);
    }

    list->next = (int*) calloc (capacity, sizeof (int));

    if (list->next == NULL)
    {
        PRINTERR (LIST_ALLOCATE_ERR);
        return   (LIST_ALLOCATE_ERR);
    }

    list->prev = (int*) calloc (capacity, sizeof (int));

    if (list->prev == NULL)
    {
        PRINTERR (LIST_ALLOCATE_ERR);
        return   (LIST_ALLOCATE_ERR);
    }

    MemSetList (list);

    ListStartFillHtml ();

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t IncreaseList (list_t* list)
{
    LIST_VERIFY (list);

    int new_capacity = list->capacity * 2;

    list_data_t* data_temp = NULL;
    int*         next_temp = NULL;
    int*         prev_temp = NULL;

    data_temp = (list_data_t*) realloc (list->data, new_capacity * sizeof (list_data_t));
    next_temp = (int*)         realloc (list->next, new_capacity * sizeof (    int    ));
    prev_temp = (int*)         realloc (list->prev, new_capacity * sizeof (    int    ));

    if (data_temp == NULL || next_temp == NULL || prev_temp == NULL)
    {
        PRINTERR (LIST_ALLOCATE_ERR);
        return   (LIST_ALLOCATE_ERR);
    }

    list->data = data_temp;
    list->next = next_temp;
    list->prev = prev_temp;

    list->free = list->capacity;
    list->capacity = new_capacity;

    MemSetList (list);

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListDestroy (list_t* list)
{
    LIST_VERIFY (list);

    free (list->data);
    free (list->next);
    free (list->prev);

    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;

    ListEndFillHtml ();

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

void MemSetList (list_t* list)
{
    DEBUG_ASSERT (list != NULL);
    DEBUG_ASSERT (list->data != NULL);
    DEBUG_ASSERT (list->next != NULL);
    DEBUG_ASSERT (list->prev != NULL);

    int*         next_arr = list->next;
    int*         prev_arr = list->prev;
    list_data_t* data_arr = list->data;

    int start_val = 0;

    if (list->size == 0)
        start_val = 0;
    else
        start_val = list->size + 1;

    // int start_val = (list->size == 0) ? 0 : list->size + 1;

    for (int i = start_val; i < list->capacity; i++)
    {
        prev_arr[i]   = -1;
        next_arr[i]   = i + 1;
        data_arr[i]   = POISON;
    }

    if (list->size == 0) 
    {
        prev_arr[0] = 0;
        next_arr[0] = 0;
    }

    next_arr[list->capacity - 1] = 0;
}

//--------------------------------------------------------------------------------