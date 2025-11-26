#include "calculator.h"

//--------------------------------------------------------------------------------

tree_node_t*
DerivativeNode (tree_node_t* ancestor)
{
    DEBUG_ASSERT (ancestor     != nullptr);

    switch (ancestor->type) {
        case value:
            return DerivativeConst (ancestor);
        case var_num:
            return DerivativeVariable (ancestor);
        case operation:
            return NULL; //потом написать вызов функцуии из массива operations
    }
}

//--------------------------------------------------------------------------------

tree_node_t* 
CopyTree (tree_node_t* ancestor)
{
    DEBUG_ASSERT (ancestor     != nullptr);

    tree_node_t* new_node = nullptr;

    if (MakeNode (&new_node)) {
        return nullptr;
    }

    new_node->type                 = ancestor->type;
    new_node->node_data.data       = ancestor->node_data.data;
    new_node->node_data.operation  = ancestor->node_data.operation;
    new_node->node_data.var_number = ancestor->node_data.var_number;

    if (ancestor->left_node) {
        new_node->left_node = CopyTree (ancestor->left_node);
    }

    if (ancestor->right_node) {
        new_node->right_node = CopyTree (ancestor->right_node);
    }

    return new_node;
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeConst (tree_node_t* ancestor)
{
    tree_data_t data = {};
    data.data = 0;

    return NewNode (value, data, nullptr, nullptr);
}

//--------------------------------------------------------------------------------


tree_node_t* 
DerivativeVariable (tree_node_t* ancestor)
{
    return NEW_DATA_NODE (1);
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeAddition (tree_node_t* ancestor)
{
    return ADD_ (d_ (ancestor->left_node), d_ (ancestor->right_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeDifference (tree_node_t* ancestor)
{
    return DIF_ (d_ (ancestor->left_node), d_ (ancestor->right_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeMultiplication (tree_node_t* ancestor)
{
    return ADD_ (MUL_ (d_ (ancestor->left_node), c_ (ancestor->right_node)), 
                 MUL_ (c_ (ancestor->left_node), d_ (ancestor->right_node))); 
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeDivision (tree_node_t* ancestor)
{
    return DIV_ (DIF_ (MUL_ (d_ (ancestor->left_node), c_ (ancestor->right_node)), 
                       MUL_ (c_ (ancestor->left_node), d_ (ancestor->right_node))), 
                 EXP_ (c_ (ancestor->right_node), NEW_DATA_NODE (2)));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeExponential (tree_node_t* ancestor)
{
    // d(g^f) = f * g^(f-1) * dg + g^f * ln(g) * df
    tree_node_t* g = c_ (ancestor->left_node);
    tree_node_t* f = c_ (ancestor->right_node);
    
    tree_node_t* summand_1 = MUL_ (MUL_ (f, EXP_ (g,  DIF_ (f, NEW_DATA_NODE (1)  ))), d_ (ancestor->left_node ));
    tree_node_t* summand_2 = MUL_ (MUL_ (EXP_ (g, f), LOG_ (g, NEW_DATA_NODE (2.71))), d_ (ancestor->right_node));
    
    return ADD_(summand_1, summand_2);
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeRoot (tree_node_t* ancestor)
{
    // d(u^(1/n)) = (1/n) * u^((1/n)-1) * du
    // for n = f(x) need to use exponential
    tree_node_t* u = c_ (ancestor->left_node );
    tree_node_t* n = c_ (ancestor->right_node);

    tree_node_t* one_over_n = DIV_ (NEW_DATA_NODE (1), n);
    tree_node_t* degrees    = DIF_ (one_over_n, NEW_DATA_NODE (1));
    
    return MUL_ (MUL_ (one_over_n, EXP_ (u, degrees)), d_ (ancestor->left_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeLog (tree_node_t* ancestor)
{
    // d(log[a](u)) = du / (u * ln(a))
    tree_node_t* u  = c_ (ancestor->left_node );
    tree_node_t* a  = c_ (ancestor->right_node);
    tree_node_t* du = d_ (ancestor->left_node );
    
    return DIV_ (du, MUL_ (u, LOG_ (a, nullptr)));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeSin (tree_node_t* ancestor)
{    
    return MUL_ (COS_ (c_ (ancestor->left_node)), d_ (ancestor->left_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeCos (tree_node_t* ancestor)
{
    tree_node_t* u  = c_ (ancestor->left_node);
    tree_node_t* du = d_ (ancestor->left_node);
    
    return MUL_ (MUL_ (NEW_DATA_NODE (-1), SIN_ (u)), du);
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeTg (tree_node_t* ancestor)
{
    // d(tg u) = du / cos^2 u
    tree_node_t* u  = c_ (ancestor->left_node);
    tree_node_t* du = d_ (ancestor->left_node);
    
    return DIV_ (du, EXP_ (COS_ (u), NEW_DATA_NODE (2)));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeCtg (tree_node_t* ancestor)
{
    // d(ctg u) = -du / sin^2 u
    tree_node_t* u  = c_ (ancestor->left_node);
    tree_node_t* du = d_ (ancestor->left_node);
    
    return MUL_ (NEW_DATA_NODE (-1), DIV_ (du, EXP_ (SIN_ (u), NEW_DATA_NODE (1))));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeArcsin (tree_node_t* ancestor)
{
    // d(arcsin u) = du / sqrt(1 - u^2)
    tree_node_t* u  = c_ (ancestor->left_node);
    tree_node_t* du = d_ (ancestor->left_node);
    
    tree_node_t* one_minus_u2 = DIF_ (NEW_DATA_NODE (1), EXP_ (u, NEW_DATA_NODE (2)));
    
    return DIV_ (du, ROOT_ (one_minus_u2, NEW_DATA_NODE (2)));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeArccos (tree_node_t* ancestor)
{
    // d(arccos u) = -du / sqrt(1 - u^2)
    tree_node_t* u  = c_ (ancestor->left_node);
    tree_node_t* du = d_ (ancestor->left_node);
    
    tree_node_t* one_minus_u2 = DIF_ (NEW_DATA_NODE (1), EXP_ (u, NEW_DATA_NODE (2)));
    
    return MUL_ (NEW_DATA_NODE (-1), DIV_ (du, ROOT_ (one_minus_u2, NEW_DATA_NODE (2))));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeArctg (tree_node_t* ancestor)
{
    // d(arctg u) = du / (1 + u^2)
    tree_node_t* u  = c_ (ancestor->left_node);
    tree_node_t* du = d_ (ancestor->left_node);
    
    return DIV_ (du, ADD_ (NEW_DATA_NODE (1), EXP_ (u, NEW_DATA_NODE (2))));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeArcctg (tree_node_t* ancestor)
{
    // d(arcctg u) = -du / (1 + u^2)
    tree_node_t* u  = c_ (ancestor->left_node);
    tree_node_t* du = d_ (ancestor->left_node);
    
    tree_node_t* one_plus_u2 = ADD_ (NEW_DATA_NODE (1), EXP_ (u, NEW_DATA_NODE (2)));
    
    return MUL_ (NEW_DATA_NODE (-1), DIV_ (du, one_plus_u2));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeSh (tree_node_t* ancestor)
{
    return MUL_ (CH_ (c_ (ancestor->left_node)), d_ (ancestor->left_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DerivativeCh (tree_node_t* ancestor)
{
    return MUL_ (SH_ (c_ (ancestor->left_node)), d_ (ancestor->left_node));
}

//--------------------------------------------------------------------------------