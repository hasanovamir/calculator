#ifndef COMMON_DUMP_h
#define COMMON_DUMP_h

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//————————————————————————————————————————————————————————————————————————————————

void        DotDumpTitle        (FILE* dot_file);
void        DotDumpNode         (FILE* dot_file, tree_node_t* node);
void        DotDumpEdge         (FILE* dot_file, tree_node_t* node);
void        TexNodeWithPriority (FILE* tex_file, tree_node_t* node , 
                                               int parent_priority);
tree_err_t  TreeDump            (tree_node_t* tree_node);
tree_err_t  TexDump             (tree_node_t* node, const char* file_name);
tree_err_t  FillDotFile         (tree_node_t* tree_node , const char* file_name);
tree_err_t  TreeFillHtml        (const char* file_name );
tree_err_t  TreeStartFillHtml   (void);
tree_err_t  TreeEndFillHtml     (void);

//————————————————————————————————————————————————————————————————————————————————

#endif //COMMON_DUMP