#include "calculator.h"

//--------------------------------------------------------------------------------

static tree_node_t*
MakeNewTaylorSeriesElem (tree_node_t* node, int degree, int main_var);

//--------------------------------------------------------------------------------

tree_node_t*
DecomposeFunction (tree_node_t* node, int value, int var_to_dif, int num_elem)
{
    tree_node_t* cur_dif_node  = c_ (node);
    tree_node_t* prev_dif_node = nullptr;
    tree_node_t* new_node = NEW_DATA_NODE (CountTree (node).immediate);

    for (int i = 1; i < 6; i++) {
        prev_dif_node = cur_dif_node;
        cur_dif_node = d_ (cur_dif_node, var_to_dif);
        TreeDeleteBranch (prev_dif_node);

        tree_node_t* new_el = MakeNewTaylorSeriesElem (cur_dif_node, i, var_to_dif);

        OptimizeTree (new_node);
        OptimizeTree (cur_dif_node);

        new_node = ADD_ (new_node, new_el);

        OptimizeTree (new_node);
    }

    return new_node;
}

//--------------------------------------------------------------------------------

tree_data_t
CountTree (tree_node_t* node)
{
    tree_node_t* new_node = c_ (node);

    ChangeVarToData (new_node);

    OptimizeTree (new_node);

    tree_data_t data = new_node->node_data;

    MyFree (new_node);

    return data;
}

//--------------------------------------------------------------------------------

void
ChangeVarToData (tree_node_t* node)
{
    if (node->left_node) {
        ChangeVarToData (node->left_node);
    }
    if (node->right_node) {
        ChangeVarToData (node->right_node);
    }

    if (node->type == var_num) {
        node->type = constant;
        node->node_data.immediate = variables_ctx.variable_arr[node->node_data.var_number].value;
        node->deriv_method = DifferentiateConst;
    }
}

//--------------------------------------------------------------------------------

int
Factorial (int num)
{
    if (num == 0) return 1;

    return num * Factorial (num - 1);
}

//--------------------------------------------------------------------------------

static tree_node_t*
MakeNewTaylorSeriesElem (tree_node_t* node, int degree, int main_var)
{
    tree_node_t* exp = EXP_ (DIF_ (NEW_VAR_NODE  (main_var), 
                                   NEW_DATA_NODE (variables_ctx.variable_arr[main_var].value)), 
                             NEW_DATA_NODE (degree));

    tree_node_t* ret_val =  DIV_ (MUL_ (NEW_DATA_NODE (CountTree (node).immediate), exp), NEW_DATA_NODE (Factorial (degree)));

    // OptimizeTree (ret_val);

    return ret_val;
}

//--------------------------------------------------------------------------------