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

    tree_node_t* ptr = DifferentiateNode (tree_context.head);

    TreeDump (tree_context.head);
    TreeDump (ptr);

    OptimizeTree (ptr);

    TreeDump (ptr);
    return 0;
}

//--------------------------------------------------------------------------------