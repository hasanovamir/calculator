#ifndef COMMON_DUMP_h
#define COMMON_DUMP_h

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//————————————————————————————————————————————————————————————————————————————————

void        MakeDumpTitle     (FILE* dot_file);
void        MakeDumpNode      (FILE* dot_file, TreeNode_t* Node);
void        MakeDumpEdge      (FILE* dot_file, TreeNode_t* Node);
tree_err_t  TreeStartFillHtml (void);
tree_err_t  TreeEndFillHtml   (void);
tree_err_t  TreeDump          (TreeContext_t* TreeContext);
tree_err_t  FillDotFile       (TreeContext_t* TreeContext, const char* file_name);
tree_err_t  TreeFillHtml      (TreeContext_t* TreeContext, const char* file_name);

//————————————————————————————————————————————————————————————————————————————————

#endif //COMMON_DUMP_h