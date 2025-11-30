#include "calculator.h"

//--------------------------------------------------------------------------------
typedef tree_node_t* (*deriv_method_t)(tree_node_t *);
tree_node_t*
DifferentiateNode (tree_node_t* node)
{
    DEBUG_ASSERT (node != nullptr);

    switch (node->type) {
        case constant:
            return DifferentiateConst (node);
        case var_num:
            return DifferentiateVariable (node);
        case operation:
            switch (node->node_data.operation) {
                case addition_op:
                    return DifferentiateAddition (node);
                case difference_op:
                    return DifferentiateDifference (node);
                case multiplication_op:
                    return DifferentiateMultiplication (node);
                case division_op:
                    return DifferentiateDivision (node);
                case exponentiation_op:
                    return DifferentiateExponential (node);
                case root_op:
                    return DifferentiateRoot (node);
                case logarithm_op:
                    return DifferentiateLog (node);
                case sin_op:
                    return DifferentiateSin (node);
                case cos_op:
                    return DifferentiateCos (node);
                case tg_op:
                    return DifferentiateTg (node);
                case ctg_op:
                    return DifferentiateCtg (node);
                case arcsin_op:
                    return DifferentiateArcsin (node);
                case arccos_op:
                    return DifferentiateArccos (node);
                case arctg_op:
                    return DifferentiateArctg (node);
                case arcctg_op:
                    return DifferentiateArcctg (node);
                case sh_op:
                    return DifferentiateSh (node);
                case ch_op:
                    return DifferentiateCh (node);
                default:
                    return nullptr;
            }
        default:
            return nullptr;
    }
}

//--------------------------------------------------------------------------------

tree_node_t* 
CopyTree (tree_node_t* node)
{
    DEBUG_ASSERT (node     != nullptr);

    tree_node_t* new_node = nullptr;

    if (MakeNode (&new_node)) {
        return nullptr;
    }

    new_node->type                 = node->type;
    new_node->node_data.immediate  = node->node_data.immediate;
    new_node->node_data.operation  = node->node_data.operation;
    new_node->node_data.var_number = node->node_data.var_number;

    if (node->left_node) {
        new_node->left_node = CopyTree (node->left_node);
    }

    if (node->right_node) {
        new_node->right_node = CopyTree (node->right_node);
    }

    return new_node;
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateConst (tree_node_t* node)
{
    return NEW_DATA_NODE (0);
}

//--------------------------------------------------------------------------------


tree_node_t* 
DifferentiateVariable (tree_node_t* node)
{
    return NEW_DATA_NODE (1);
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateAddition (tree_node_t* node)
{
    return ADD_ (d_ (node->left_node), d_ (node->right_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateDifference (tree_node_t* node)
{
    return DIF_ (d_ (node->left_node), d_ (node->right_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateMultiplication (tree_node_t* node)
{
    return ADD_ (MUL_ (d_ (node->left_node), c_ (node->right_node)), 
                 MUL_ (c_ (node->left_node), d_ (node->right_node))); 
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateDivision (tree_node_t* node)
{
    return DIV_ (DIF_ (MUL_ (d_ (node->left_node), c_ (node->right_node)), 
                       MUL_ (c_ (node->left_node), d_ (node->right_node))), 
                 EXP_ (c_ (node->right_node), NEW_DATA_NODE (2)));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateExponential (tree_node_t* node)
{
    // d(g^f) = f * g^(f-1) * dg + g^f * ln(g) * df
    tree_node_t* g = c_ (node->left_node);
    tree_node_t* f = c_ (node->right_node);
    
    tree_node_t* summand_1 = MUL_ (MUL_ (c_ (node->right_node), EXP_ (c_ (node->left_node),  DIF_ (c_ (node->right_node), NEW_DATA_NODE (1)  ))), d_ (node->left_node ));
    tree_node_t* summand_2 = MUL_ (MUL_ (EXP_ (c_ (node->left_node), c_ (node->right_node)), LOG_ (c_ (node->left_node), NEW_DATA_NODE (2.71))), d_ (node->right_node));
    
    return ADD_(summand_1, summand_2);
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateRoot (tree_node_t* node)
{
    // d(u^(1/n)) = (1/n) * u^((1/n)-1) * du
    // for n = f(x) need to use exponential
    tree_node_t* u = c_ (node->left_node );
    tree_node_t* n = c_ (node->right_node);

    tree_node_t* one_over_n = DIV_ (NEW_DATA_NODE (1), n);
    tree_node_t* degrees    = DIF_ (one_over_n, NEW_DATA_NODE (1));
    
    return MUL_ (MUL_ (one_over_n, EXP_ (u, degrees)), d_ (node->left_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateLog (tree_node_t* node)
{
    // d(log[a](u)) = du / (u * ln(a))
    tree_node_t* u  = c_ (node->left_node );
    tree_node_t* a  = c_ (node->right_node);
    tree_node_t* du = d_ (node->left_node );
    
    return DIV_ (du, MUL_ (u, LOG_ (a, nullptr)));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateSin (tree_node_t* node)
{    
    return MUL_ (COS_ (c_ (node->left_node)), d_ (node->left_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateCos (tree_node_t* node)
{
    tree_node_t* u  = c_ (node->left_node);
    tree_node_t* du = d_ (node->left_node);
    
    return MUL_ (MUL_ (NEW_DATA_NODE (-1), SIN_ (u)), du);
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateTg (tree_node_t* node)
{
    // d(tg u) = du / cos^2 u
    tree_node_t* u  = c_ (node->left_node);
    tree_node_t* du = d_ (node->left_node);
    
    return DIV_ (du, EXP_ (COS_ (u), NEW_DATA_NODE (2)));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateCtg (tree_node_t* node)
{
    // d(ctg u) = -du / sin^2 u
    tree_node_t* u  = c_ (node->left_node);
    tree_node_t* du = d_ (node->left_node);
    
    return MUL_ (NEW_DATA_NODE (-1), DIV_ (du, EXP_ (SIN_ (u), NEW_DATA_NODE (2))));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateArcsin (tree_node_t* node)
{
    // d(arcsin u) = du / sqrt(1 - u^2)
    tree_node_t* u  = c_ (node->left_node);
    tree_node_t* du = d_ (node->left_node);
    
    tree_node_t* one_minus_u2 = DIF_ (NEW_DATA_NODE (1), EXP_ (u, NEW_DATA_NODE (2)));
    
    return DIV_ (du, ROOT_ (one_minus_u2, NEW_DATA_NODE (2)));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateArccos (tree_node_t* node)
{
    // d(arccos u) = -du / sqrt(1 - u^2)
    tree_node_t* u  = c_ (node->left_node);
    tree_node_t* du = d_ (node->left_node);
    
    tree_node_t* one_minus_u2 = DIF_ (NEW_DATA_NODE (1), EXP_ (u, NEW_DATA_NODE (2)));
    
    return MUL_ (NEW_DATA_NODE (-1), DIV_ (du, ROOT_ (one_minus_u2, NEW_DATA_NODE (2))));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateArctg (tree_node_t* node)
{
    // d(arctg u) = du / (1 + u^2)
    tree_node_t* u  = c_ (node->left_node);
    tree_node_t* du = d_ (node->left_node);
    
    return DIV_ (du, ADD_ (NEW_DATA_NODE (1), EXP_ (u, NEW_DATA_NODE (2))));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateArcctg (tree_node_t* node)
{
    // d(arcctg u) = -du / (1 + u^2)
    tree_node_t* u  = c_ (node->left_node);
    tree_node_t* du = d_ (node->left_node);
    
    tree_node_t* one_plus_u2 = ADD_ (NEW_DATA_NODE (1), EXP_ (u, NEW_DATA_NODE (2)));
    
    return MUL_ (NEW_DATA_NODE (-1), DIV_ (du, one_plus_u2));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateSh (tree_node_t* node)
{
    return MUL_ (CH_ (c_ (node->left_node)), d_ (node->left_node));
}

//--------------------------------------------------------------------------------

tree_node_t* 
DifferentiateCh (tree_node_t* node)
{
    return MUL_ (SH_ (c_ (node->left_node)), d_ (node->left_node));
}

//--------------------------------------------------------------------------------