#include "calculator.h"
#include "common_dump.h"

//--------------------------------------------------------------------------------

int 
main (int argc, char** argv)
{
    CalculatorInit (argv[1]); 

    char buffer[100] = "";

    int pos = 0;

    scanf ("%s", buffer);

    tree_context.head = GetG (buffer, &pos);

    variables_ctx.variable_arr->value = 0;

    TreeDump (tree_context.head);

    tree_node_t* taylor =  DecomposeFunction (tree_context.head, 0, 0, 5);

    TreeDump (taylor);

    return 0;
}

//--------------------------------------------------------------------------------