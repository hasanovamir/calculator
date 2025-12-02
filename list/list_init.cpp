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

    MemSetList (list);

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t IncreaseList (list_t* list)
{
    int new_capacity = list->capacity * 2;

    list_data_t* data_temp = NULL;
    int*         next_temp = NULL;

    data_temp = (list_data_t*) realloc (list->data, new_capacity * sizeof (list_data_t));
    next_temp = (int*)         realloc (list->next, new_capacity * sizeof (    int    ));

    {
        PRINTERR (LIST_ALLOCATE_ERR);
        return   (LIST_ALLOCATE_ERR);
    }

    list->data = data_temp;
    list->next = next_temp;

    list->free = list->capacity;
    list->capacity = new_capacity;

    MemSetList (list);

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListDestroy (list_t* list)
{
    free (list->data);
    free (list->next);

    list->data = NULL;
    list->next = NULL;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

void MemSetList (list_t* list)
{
    DEBUG_ASSERT (list       != NULL);
    DEBUG_ASSERT (list->data != NULL);
    DEBUG_ASSERT (list->next != NULL);

    int*         next_arr = list->next;
    list_data_t* data_arr = list->data;

    int start_val = (list->size == 0) ? 0 : list->size + 1;

    for (int i = start_val; i < list->capacity; i++)
    {

        next_arr[i] = i + 1;
        
        data_arr[i].node_data.immediate = POISON;
    }

    if (list->size == 0) 
    {

        next_arr[0] = 0;
    }

    next_arr[list->capacity - 1] = 0;
}

//--------------------------------------------------------------------------------