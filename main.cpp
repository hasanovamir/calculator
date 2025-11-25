#include "tree.h"
#include "common_dump.h"

//--------------------------------------------------------------------------------

int 
main ()
{
    my_alloc_context_t my_alloc_context = {};
    tree_context_t tree_context = {};
    tree_context.my_alloc_context = &my_alloc_context;

    TreeInit (&tree_context);

    TreeReadDataBase (&tree_context, "test.txt");

    TreeDump (&tree_context);

    return 0;
}

//--------------------------------------------------------------------------------