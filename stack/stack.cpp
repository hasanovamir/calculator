#include "stack.h"

//--------------------------------------------------------------------------------

stack_err_t stack_init (stack_t* stk, int init_capacity)
{
    DEBUG_ASSERT (stk != NULL);

    if (init_capacity < 1)
    {
        return STACK_INVALID_VALUE;
    }
    
    int array_size = (init_capacity > array_min_size) ? init_capacity + 2: array_min_size;

    stk->data = (stack_data_t*)calloc (array_size, sizeof (stack_data_t));

    if (stk->data == NULL)
    {
        return STACK_ALLOCATION_ERR;
    }

    stk->capacity = array_size - 2;
    stk->size = 0;

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t stack_push (stack_t* stk, stack_data_t value)
{

    RETURN_IF_STACK_ERR (upsize_array_if_need (stk));

    stk->data[++stk->size] = value;

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t stack_pop (stack_t* stk, stack_data_t* val_pointer)
{
    if (stk->size == 0)
    {
        val_pointer->prev_node = NULL;
        return STACK_NO_EL_TO_POP;
    }

    RETURN_IF_STACK_ERR (downsize_array_if_need (stk));

    *val_pointer = stk->data[stk->size--];
    
    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t stack_destroy (stack_t* stk)
{

    free (stk->data);

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

const char* error_stack_code_to_string (stack_err_t status)
{
    switch (status)
    {
        ERRCASE (STACK_DATA_RUINED   );
        ERRCASE (STACK_ALLOCATION_ERR);
        ERRCASE (STACK_NO_EL_TO_POP  );
        ERRCASE (STACK_SUCCESS       );
        ERRCASE (STACK_INVALID_VALUE );
        ERRCASE (STACK_SCAN_ERR      );
        ERRCASE (STACK_UNKNOWN_ERR   );
        default:
            return "SUCCESS";
    }
}

//--------------------------------------------------------------------------------


stack_err_t upsize_array_if_need (stack_t* stk)
{
    DEBUG_ASSERT (stk       != NULL);
    DEBUG_ASSERT (stk->data != NULL);

    if (stk->size == stk->capacity - 1)
    {
        stack_data_t* old_data = stk->data;

        stk->data = (stack_data_t*)realloc (stk->data, stk->capacity * 2);

        if (stk->data == NULL)
        {
            stk->data = old_data;

            return STACK_ALLOCATION_ERR;
        }

        stk->capacity = 2 * stk->capacity - 2;
    }

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t downsize_array_if_need (stack_t* stk)
{
    DEBUG_ASSERT (stk       != NULL);
    DEBUG_ASSERT (stk->data != NULL);

    if (stk->size <= stk->capacity / 4 && stk->capacity > array_min_size * 2)
    {
        stack_data_t* old_data = stk->data;

        stk->data = (stack_data_t*)realloc (stk->data, stk->capacity / 2);

        if (stk->data == NULL)
        {
            stk->data = old_data;

            return STACK_SUCCESS;
        }

        stk->capacity = stk->capacity / 2 - 2;
    }

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------