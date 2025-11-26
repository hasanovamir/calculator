#ifndef COMMON_DUMP_h
#define COMMON_DUMP_h

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//————————————————————————————————————————————————————————————————————————————————

void        DotDumpTitle      (FILE* dot_file);
void        DotDumpNode       (FILE* dot_file, tree_node_t* node);
void        DotDumpEdge       (FILE* dot_file, tree_node_t* node);
void        TexNode           (FILE* tex_file, tree_node_t* node);
tree_err_t  TreeDump          ();
tree_err_t  TexDump           (const char* file_name);
tree_err_t  FillDotFile       (const char* file_name);
tree_err_t  TreeFillHtml      (const char* file_name);
tree_err_t  TreeStartFillHtml (void);
tree_err_t  TreeEndFillHtml   (void);

//————————————————————————————————————————————————————————————————————————————————

#endif //COMMON_DUMP