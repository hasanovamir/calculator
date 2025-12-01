#include "calculator.h"

//--------------------------------------------------------------------------------

int 
OptimizeTree (tree_node_t* node)
{
    if (node->left_node) {
        OptimizeTree (node->left_node);  
    }

    if (node->right_node) {
        OptimizeTree (node->right_node);  
    }

    if (node->type == var_num) {
        return 0;
    }

    if (node->type == constant) {
        return 0;
    }

    if (node->type == operation) {
        if (node->left_node  && node->left_node->type  == constant && 
            node->right_node && node->right_node->type == constant) {
            return OptimizeTreeIfBothConst (node);
        }

        else if (node->right_node && node->right_node->type == constant) {
           return OptimizeTreeIfRightConst (node);
        }
        
        else if (node->left_node && node->left_node->type == constant) {
            return OptimizeTreeIfLeftConst (node);
        }
            
    }

    return 0;
}

//--------------------------------------------------------------------------------

int 
OptimizeTreeIfBothConst (tree_node_t* node)
{
    double left_val  = node->left_node ->node_data.immediate;
    double right_val = node->right_node->node_data.immediate;
    double result    = 0;

    switch (node->node_data.operation) {
        case addition_op:
            result = left_val + right_val;
            break;
        case difference_op:
            result = left_val - right_val;
            break;
        case multiplication_op:
            result = left_val * right_val;
            break;
        case division_op:
            if (right_val != 0) {
                result = left_val / right_val;
            } else {
                return 0;
            }
            break;
        case exponentiation_op:
            result = pow(left_val, right_val);
            break;
        default:
            return 0; 
    }

    node->type = constant;

    node->node_data.immediate = result;
    
    FreeSideNodes (node);

    return 1;
}

//--------------------------------------------------------------------------------

int 
OptimizeTreeIfRightConst (tree_node_t* node)
{
    double right_val = node->right_node->node_data.immediate;

    tree_node_t* left  = node->left_node ;
    tree_node_t* right = node->right_node;
            
    switch (node->node_data.operation) {
        case addition_op:
            if (CheckToEquality (right_val, 0)) {
                CopyNodeData (node, node->left_node);

                MyFree (left );
                MyFree (right);

                return 1;
            }
            break;
        case multiplication_op:
            if (CheckToEquality (right_val, 1)) {
                CopyNodeData (node, node->left_node);

                MyFree (left );
                MyFree (right);

                return 1;
            } 
            else if (CheckToEquality (right_val, 0)) {
                node->type = constant;

                node->node_data.immediate = 0;

                TreeDeleteBranch (node->left_node );
                TreeDeleteBranch (node->right_node);

                return 1;
            }
            break;
        case exponentiation_op:
            if (CheckToEquality (right_val, 0)) {
                node->type = constant;

                node->node_data.immediate = 1;

                TreeDeleteBranch (node->left_node );
                TreeDeleteBranch (node->right_node);

                return 1;
            }
            if (right_val == 1) {
                CopyNodeData (node, node->left_node);

                MyFree (left );
                MyFree (right);

                return 1;
            }
            break;
        default:
            return 0;
    }

    return 0;
}

//--------------------------------------------------------------------------------

int 
OptimizeTreeIfLeftConst (tree_node_t* node)
{
    double left_val = node->left_node->node_data.immediate;

    tree_node_t* left  = node->left_node ;
    tree_node_t* right = node->right_node;
            
    switch (node->node_data.operation) {
        case addition_op:
            if (CheckToEquality (left_val, 0)) {
                CopyNodeData (node, node->right_node);

                MyFree (left );
                MyFree (right);

                return 1;
            }
            break;
        case multiplication_op:
            if (left_val == 1) {
                CopyNodeData (node, node->right_node);

                MyFree (left );
                MyFree (right);

                return 1;
            } 
            else if (left_val == 0) {
                node->type = constant;

                node->node_data.immediate = 0;

                TreeDeleteBranch (node->left_node );
                TreeDeleteBranch (node->right_node);

                return 1;
            }
        case exponentiation_op:
            if (left_val == 0) {
                node->type = constant;

                node->node_data.immediate = 0;

                TreeDeleteBranch (node->left_node );
                TreeDeleteBranch (node->right_node);

                return 1;
            }
            if (left_val == 1) {
                node->type = constant;

                node->node_data.immediate = 1;

                TreeDeleteBranch (node->left_node );
                TreeDeleteBranch (node->right_node);

                return 1;
            }
            break;
        default :
            return 0;
    }

    return 0;
}

//--------------------------------------------------------------------------------

int 
CheckToEquality (double value_1, double value_2)
{
    if (fabs (value_1 - value_2) < 0.001) {
        return 1;
    }
    else {
        return 0;
    }
}

//--------------------------------------------------------------------------------

void 
CopyNodeData (tree_node_t* destination_node, tree_node_t* source_node)
{
    destination_node->type = source_node->type;

    switch (source_node->type) {
        case constant:
            destination_node->node_data.immediate = source_node->node_data.immediate;
            destination_node->left_node  = NULL;
            destination_node->right_node = NULL;
            return ;
        case operation:
            destination_node->node_data.operation = source_node->node_data.operation;//нужно сына сохранить...
            destination_node->left_node  = source_node->left_node ;
            destination_node->right_node = source_node->right_node;
            return ;
        case var_num:
            destination_node->node_data.var_number = source_node->node_data.var_number;
            destination_node->left_node  = NULL;
            destination_node->right_node = NULL;
            return ;
        default:
            PRINTERR (TREE_UNKNOWN_DATA_TYPE);
    }
}

//--------------------------------------------------------------------------------