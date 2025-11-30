#include "calculator.h"
#include "common_dump.h"

//--------------------------------------------------------------------------------

int 
main (int argc, char** argv)
{
    CalculatorInit (argv[1]);

    tree_node_t* ptr = DifferentiateNode (tree_context.head); 

    TreeDump (tree_context.head);
    TreeDump (ptr);

    OptimizeTree (ptr);

    TreeDump (ptr);
    return 0;
}

//--------------------------------------------------------------------------------