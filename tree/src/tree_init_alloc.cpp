#include "tree.h"

//--------------------------------------------------------------------------------

my_alloc_context_t my_alloc_context = {};
tree_context_t tree_context = {0, 0, nullptr, &my_alloc_context};

//-----------------------------------------------------------------------------

tree_err_t 
TreeInit ()
{
    InitBigArray ();

    tree_context.head = MyAlloc ();

    if (tree_context.head == NULL) {
        return TREE_ALLOC_ERR;
    }

    tree_context.head->idx = 0;

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_node_t*
MyAlloc ()
{
    my_alloc_context_t* alloc_context = tree_context.my_alloc_context;

    while  (alloc_context->big_array[alloc_context->cur_array].list.free == 0)
    {
        if (ChangeSrcArray ()) {
            return NULL;
        }
    }

    list_t* list = &alloc_context->big_array[alloc_context->cur_array].list; 

    tree_node_t* new_node = list->data + list->free;

    list->free = list->next[list->free];

    tree_context.size++;
    tree_context.my_alloc_context->src_size++;

    return new_node;
}

//--------------------------------------------------------------------------------

void 
TreeDestroy ()
{
    DEBUG_ASSERT (&tree_context != nullptr);

    for (int i = 0; i < tree_context.my_alloc_context->num_src_arr; i++) {
        ListDestroy (&tree_context.my_alloc_context->big_array[i].list);
    }

    free (tree_context.my_alloc_context->big_array);
}

//--------------------------------------------------------------------------------

tree_err_t 
InitBigArray ()
{
    big_array_data_t* big_array_ptr = nullptr;

    big_array_ptr = (big_array_data_t*) calloc (StartBigArrayCap, sizeof (big_array_data_t));

    if (big_array_ptr == NULL) {
        PRINTERR (TREE_ALLOC_ERR);
        return TREE_ALLOC_ERR;
    }

    for (int i = 0; i < StartBigArrayCap; i++) {
        if (ListInit (&big_array_ptr[i].list, AllocCapacity)) {
            return TREE_ALLOC_ERR;
        }

        for (int j = 0; j < AllocCapacity; j++) {
            big_array_ptr[i].list.data[j].idx = i * AllocCapacity + j;
        }
    }

    tree_context.my_alloc_context->big_array = big_array_ptr;

    tree_context.my_alloc_context->src_cap = StartBigArrayCap * AllocCapacity;

    tree_context.my_alloc_context->num_src_arr = StartBigArrayCap;

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
ChangeSrcArray ()
{
    int num_arr = tree_context.my_alloc_context->num_src_arr;

    my_alloc_context_t* alloc_context = tree_context.my_alloc_context;

    if  (alloc_context->cur_array == num_arr - 1) {
        big_array_data_t* tmp = (big_array_data_t*) realloc (
            alloc_context->big_array, 2 * num_arr * sizeof (big_array_data_t));

        if (tmp == NULL) {
            PRINTERR (TREE_ALLOC_ERR);
            return TREE_ALLOC_ERR;
        }

        for (int i = 0; i < num_arr; i++) {
            if (ListInit (&tmp[i + num_arr].list, AllocCapacity)) {
                return TREE_ALLOC_ERR;
            }
        }

        alloc_context->big_array = tmp;

        alloc_context->num_src_arr *= 2;

        alloc_context->src_cap *= 2;
    }

    alloc_context->cur_array++; 

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

void 
MyFree (tree_node_t* node)
{
    int cur_arr = 0;

    for (int i = 0; i < tree_context.my_alloc_context->num_src_arr; i++) {
        if (node > tree_context.my_alloc_context->big_array[i].list.data &&
            node < tree_context.my_alloc_context->big_array[i].list.data + AllocCapacity) {
                cur_arr = i;
                break;
            }
    }

    int pos = node - tree_context.my_alloc_context->big_array[cur_arr].list.data;

    int prev_free = tree_context.my_alloc_context->big_array[cur_arr].list.free;

    tree_context.my_alloc_context->big_array[cur_arr].list.next[pos] = prev_free;

    tree_context.my_alloc_context->big_array[cur_arr].list.free = pos;
 
    tree_context.my_alloc_context->cur_array = cur_arr;

    tree_context.my_alloc_context->src_size--;
}

//--------------------------------------------------------------------------------

void 
FreeSideNodes (tree_node_t* parent_node)
{
    if (parent_node->left_node) {
        MyFree (parent_node->left_node );
    }
    if (parent_node->right_node) {
        MyFree (parent_node->right_node);
    }

    parent_node->left_node  = nullptr;
    parent_node->right_node = nullptr;
}

//--------------------------------------------------------------------------------