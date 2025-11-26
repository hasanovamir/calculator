#include "../calculator.h"
#include "common_dump.h"

//--------------------------------------------------------------------------------

tree_err_t 
FillDotFile (const char* file_name)
{
    DEBUG_ASSERT (file_name != NULL);

    FILE* dot_file = fopen (file_name, "w");
    
    if (dot_file == NULL)
    {
        PRINTERR (TREE_OPEN_FILE_ERR);
        return   (TREE_OPEN_FILE_ERR);
    }

    DotDumpTitle (dot_file);
    DotDumpNode  (dot_file, tree_context.head);
    DotDumpEdge  (dot_file, tree_context.head);
    
    fprintf (dot_file, "}\n");

    fclose (dot_file);

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

void 
DotDumpTitle (FILE* dot_file)
{
    DEBUG_ASSERT (dot_file != NULL);

    char title[] = 

R"(digraph ListDump {
    rankdir=TB;
    node [shape=Mrecord, color=black, fontname="Courier"];

    graph [splines=ortho]
    overlap=false;
    
)";

    fprintf (dot_file, "%s", title);
}

//--------------------------------------------------------------------------------

void
DotDumpNode (FILE* dot_file, tree_node_t* node)
{
    DEBUG_ASSERT (dot_file != NULL);

    fprintf (dot_file, "\tnode%d [label=\"{<type> type = ", node->idx);

    if (node->type == operation) {
        fprintf (dot_file, "OPER | VAL = %s |", operations[node->node_data.operation].oper_name);
    }
    else if (node->type == value) {
        fprintf (dot_file, "NUM | VAL = %lf |", node->node_data.data);
    }
    else if (node->type == var_num) {
        fprintf (dot_file, "VAR | VAL = %d", node->node_data.var_number);
    }
    else {
        PRINTERR (TREE_UNKNOWN_DATA_TYPE);
        return ;
    }

    fprintf (dot_file, " <ptr> ptr:%p | {<left_node> left_node=%p | <right_node> right_node=%p } }\", color=black, style=filled, fillcolor=lime];\n", 
        node, node->left_node, node->right_node);

    if (node->left_node)
        DotDumpNode (dot_file, node->left_node);
    if (node->right_node)
        DotDumpNode (dot_file, node->right_node);
}

//--------------------------------------------------------------------------------

void 
DotDumpEdge (FILE* dot_file, tree_node_t* node)
{
    DEBUG_ASSERT (dot_file != NULL);

    if (node == NULL)
        return;

    if (node->left_node) {
        fprintf (dot_file, "\tnode%d -> node%d [color=green];\n", node->idx, node->left_node->idx);
        DotDumpEdge (dot_file, node->left_node);
    }

    if (node->right_node) {
        fprintf (dot_file, "\tnode%d -> node%d [color=green];\n", node->idx, node->right_node->idx);
        DotDumpEdge (dot_file, node->right_node);
    }
}

//--------------------------------------------------------------------------------

tree_err_t 
TreeDump ()
{
    tree_context.num_calls++;

    char dot_file_name[MAXFILENAMESIZE];
    char svg_file_name[MAXFILENAMESIZE];
    char tex_file_name[MAXFILENAMESIZE];
    char command[MAXCOMMANDSIZE];
    
    snprintf (dot_file_name, MAXFILENAMESIZE, "dot/%s_%d.dot", "call", tree_context.num_calls);
    snprintf (svg_file_name, MAXFILENAMESIZE, "svg/%s_%d.svg", "call", tree_context.num_calls);
    snprintf (tex_file_name, MAXFILENAMESIZE, "TeX/%s_%d.tex", "call", tree_context.num_calls);
    
    if (FillDotFile (dot_file_name))
    {
        return TREE_OPEN_FILE_ERR;
    }

    printf ("\nGenerated DOT file:  %s\n", dot_file_name);
    
    snprintf (command, sizeof(command), "dot -Tsvg %s -o %s", dot_file_name, svg_file_name);
    
    if (system(command) == 0)
    {
        printf ("Generated SVG image: %s\n", svg_file_name);
    }
    else
    {
        PRINTERR (TREE_SYS_FUNC_ERR);
        return   (TREE_SYS_FUNC_ERR);
    }

    if (TreeFillHtml (svg_file_name))
        return TREE_OPEN_FILE_ERR;

    if (TexDump (tex_file_name)) {
        return TREE_SUCCESS;
    }

    snprintf (command, sizeof(command), "pdflatex -output-directory=pdf -interaction=nonstopmode %s > /dev/null 2>&1", tex_file_name);

    if (system (command) == 0) {
        printf ("Generated TEX image: %s\n\n", tex_file_name);
        snprintf (command, sizeof(command), "rm pdf/call_%d.aux pdf/call_%d.log", 
            tree_context.num_calls, tree_context.num_calls);
        system (command);
    }
    else {
        PRINTERR (TREE_SYS_FUNC_ERR);
        return   (TREE_SYS_FUNC_ERR);
    }

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
TreeStartFillHtml (void)
{
    FILE* html_file = fopen ("tree_dump.html", "w");

    if (html_file == NULL)
    {
        PRINTERR (TREE_OPEN_FILE_ERR);
        return   (TREE_OPEN_FILE_ERR);
    }

    fprintf (html_file, "<!DOCTYPE html>\n<html>\n<head>\n\t<title>LIST DUMP</title>\n</head>\n<body>\n");

    fclose (html_file);

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
TreeFillHtml (const char* file_name)
{
    FILE* html_file = fopen ("tree_dump.html", "a");

    if (html_file == NULL)
    {
        PRINTERR (TREE_OPEN_FILE_ERR);
        return   (TREE_OPEN_FILE_ERR);
    }

    char string[MAXSTRINGSIZE] = "";

    snprintf (string, MAXSTRINGSIZE, "\t<h2>CALL = #%d</h2>\n\t<p>HEAD = %p</p>\n\t<img src=\"%s\">\n\n", 
        tree_context.num_calls, tree_context.head, file_name);

    fprintf (html_file, "%s", string);

    fclose (html_file);

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
TreeEndFillHtml (void)
{
    FILE* html_file = fopen ("tree_dump.html", "a");

    if (html_file == NULL)
    {
        PRINTERR (TREE_OPEN_FILE_ERR);
        return   (TREE_OPEN_FILE_ERR);
    }

    fprintf (html_file, "</body>\n</html>");

    fclose (html_file);

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
TexDump (const char* file_name)
{
    FILE* tex_file = fopen (file_name, "w");

    if (tex_file == nullptr) {
        PRINTERR (TREE_OPEN_FILE_ERR);
        return TREE_OPEN_FILE_ERR;
    }

    fprintf (tex_file, "\\documentclass{article}\n\\begin{document}\n$");

    TexNode (tex_file, tree_context.head);

    fprintf (tex_file, "$\n\\end{document}");

    fclose (tex_file);

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

void 
TexNode (FILE* tex_file, tree_node_t* node)
{
    DEBUG_ASSERT (node     != nullptr);
    DEBUG_ASSERT (tex_file != nullptr);

    if (node->left_node) {
        TexNode (tex_file, node->left_node);
    }

    if (node->type == operation) {
        fprintf (tex_file, "%s", operations[node->node_data.operation].oper);
    }
    else if (node->type == value) {
        fprintf (tex_file, "%lf", node->node_data.data);
    }
    else {
        fprintf (tex_file, "x");
    }

    if (node->right_node) {
        TexNode (tex_file, node->right_node);
    }
}

//--------------------------------------------------------------------------------