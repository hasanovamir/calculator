#ifndef TREE_H
#define TREE_H

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "colors.h"
#include "stack.h"
#include "list.h"
#include "tree_types.h"

//————————————————————————————————————————————————————————————————————————————————

struct BigArrayData_t
{
    char*  string_src;
    list_t list_node_src;
};

//————————————————————————————————————————————————————————————————————————————————

struct MyAllocContext_t
{
    int src_cap;
    int src_size;
    int cur_array;
    int num_src_arr;
    int pos_in_cur_arr;
    BigArrayData_t* BigArray;
};

//————————————————————————————————————————————————————————————————————————————————

struct TreeContext_t {
    int size;
    int num_calls;
    TreeNode_t* head;
    MyAllocContext_t* MyAllocContext;
};

//————————————————————————————————————————————————————————————————————————————————

int         GetAnswer           (void);
int         GetOperationIdx     (void);
void        CleanBuffer         (void);
void        PrintNode           (const TreeNode_t* Node, char* NodeBuffer);
void        TreeDestroy         (TreeContext_t* TreeContext);
tree_err_t  TreeInit            (TreeContext_t* TreeContext);
tree_err_t  TreeInit            (TreeContext_t* TreeContext);
tree_err_t  WriteTreeToFile     (TreeContext_t* TreeContext);
tree_err_t  InitBigArray        (TreeContext_t* TreeContext);
tree_err_t  ChangeSrcArray      (TreeContext_t* TreeContext);
TreeNode_t* MyAlloc             (TreeContext_t* TreeContext, int count);
TreeNode_t* TreeSearchNode      (TreeContext_t* TreeContext, const char* word);
tree_err_t  MakeNode            (TreeContext_t* TreeContext, TreeNode_t** Node);
tree_err_t  ReadBuffer          (char* buffer, const char* FileName, int size);
tree_err_t  TreeReadDataBase    (TreeContext_t* TreeContext, const char* FileName);
TreeNode_t* ReadNode            (TreeContext_t* TreeContext, TreeNode_t* ParentNode, char* buffer, int* pos);
tree_err_t  TreeSearchIteration (TreeNode_t** CurNode, stack_t* stack, int* status, char* letter);
tree_err_t  TreeInsert          (TreeContext_t* TreeContext, tree_data_t val, TreeNode_t* ParentNode, tree_data_t cond);

//————————————————————————————————————————————————————————————————————————————————

#endif // TREE_H