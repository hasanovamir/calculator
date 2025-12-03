#include "list.h"

//--------------------------------------------------------------------------------

list_err_t FillDotFile (list_t* list, const char* file_name)
{
    DEBUG_ASSERT (file_name != NULL);

    FILE* dot_file = fopen (file_name, "w");
    
    if (dot_file == NULL)
    {
        PRINTERR (LIST_OPEN_FILE_ERR);
        return   (LIST_OPEN_FILE_ERR);
    }

    MakeDumpTitle (dot_file);

    for (int i = 0; i < 40; i++)
    {
        if (list->data[i].node_data.immediate == POISON)
        {
            MakeDumpNode (dot_file, i, i, list->data[i], list->next[i], "yellow");
        }
        else
        {
            MakeDumpNode (dot_file, i, i, list->data[i], list->next[i], "lime");
        }
    }

    for (int i = 0; i < 40; i++)
    {
        if (i < 35) // i + 1 < cap // invis node connection
        {
            MakeDumpEdge (dot_file, i, i + 1, "invis");
        }

        if (list->data[i].node_data.immediate == POISON && i != 0) // free connection
        {
            MakeDumpEdge (dot_file, i, list->next[i], "purple");
        }
        else // connection of ordinary members
        {
            MakeDumpEdge (dot_file, i, list->next[i], "blue" );
        }
    }
    
    fprintf(dot_file, "}\n");

    fclose(dot_file);

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

void MakeDumpTitle (FILE* dot_file)
{
    DEBUG_ASSERT (dot_file != NULL);

    char title[] = 

R"(digraph ListDump {
    rankdir=LR;
    node [shape=Mrecord, fontname="Courier"];

    graph [splines=ortho]
    overlap=false;
    
)";

    fprintf(dot_file, "%s", title);
}

//--------------------------------------------------------------------------------

void MakeDumpNode (FILE* dot_file, int node_number, int idx, list_data_t data, int next, const char* color)
{
    DEBUG_ASSERT (dot_file != NULL);
    DEBUG_ASSERT (color    != NULL);

    if (data.node_data.immediate == POISON)
        fprintf(dot_file, "\tnode%d [label=\"<idx> idx:%d | <data> data:PSN | <next> next:%d\", style=filled, color=%s];\n", 
            node_number, idx, next, color);
    else
            fprintf(dot_file, "\tnode%d [label=\"<idx> idx:%d | <data> data:%lf | <next> next:%d\", style=filled, color=%s];\n", 
            node_number, idx, data.node_data.immediate, next, color);
}

//--------------------------------------------------------------------------------

void MakeDumpEdge (FILE* dot_file, int node_num_1, int node_num_2, const char* color)
{
    DEBUG_ASSERT (dot_file != NULL);
    DEBUG_ASSERT (color    != NULL);

    if (strcmp (color, "invis") == 0)
        fprintf (dot_file, "\tnode%d -> node%d [style = invis, weight = 100];\n", node_num_1, node_num_2);
    else
        fprintf(dot_file, "\tnode%d -> node%d [color=%s, constraint=false];\n", node_num_1, node_num_2, color);
}

//--------------------------------------------------------------------------------

list_err_t ListDump (list_t* list)
{
    list->num_calls++;

    char dot_file_name[MaxFileNameSize];
    char png_file_name[MaxFileNameSize];
    char command[MaxCommandSize];
    
    snprintf(dot_file_name, MaxFileNameSize, "dump/dot/%s_%d.dot", "call", list->num_calls);
    snprintf(png_file_name, MaxFileNameSize, "dump/png/%s_%d.svg", "call", list->num_calls);
    
    if (FillDotFile (list, dot_file_name))
    {
        return LIST_OPEN_FILE_ERR;
    }

    printf("Generated DOT file:  %s\n", dot_file_name);
    
    snprintf(command, MaxCommandSize, "dot -Tsvg %s -o %s", dot_file_name, png_file_name);

    int result = system(command);
    
    if (result == 0)
    {
        printf("Generated PNG image: %s\n", png_file_name);
    }
    else
    {
        PRINTERR (LIST_SYS_FUNC_ERR);
        return   (LIST_SYS_FUNC_ERR);
    }

    if (ListFillHtml (list, png_file_name))
        return LIST_OPEN_FILE_ERR;

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListStartFillHtml (void)
{
    FILE* html_file = fopen ("dump/list_dump.html", "w");

    if (html_file == NULL)
    {
        PRINTERR (LIST_OPEN_FILE_ERR);
        return   (LIST_OPEN_FILE_ERR);
    }

    fprintf (html_file, "<!DOCTYPE html>\n<html>\n<head>\n\t<title>LIST DUMP</title>\n</head>\n<body>\n");

    fclose (html_file);

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListFillHtml (list_t* list, const char* file_name)
{
    FILE* html_file = fopen ("dump/list_dump.html", "a");

    if (html_file == NULL)
    {
        PRINTERR (LIST_OPEN_FILE_ERR);
        return   (LIST_OPEN_FILE_ERR);
    }

    char string[MaxStringSize] = "";

    snprintf (string, MaxStringSize, "\t<h2>CALL = #%d</h2>\n\t<p>HEAD = %d</p>\n\t<p>TAIL = %d</p>\n\t<p>FREE = %d</p>\n\t<p>CAPACITY = %d</p>\n\t<img src=\"%s\">\n\n", 
        list->num_calls, list->head, list->tail, list->free, list->capacity, file_name);

    fprintf (html_file, "%s", string);

    fclose (html_file);

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------

list_err_t ListEndFillHtml (void)
{
    FILE* html_file = fopen ("dump/list_dump.html", "a");

    if (html_file == NULL)
    {
        PRINTERR (LIST_OPEN_FILE_ERR);
        return   (LIST_OPEN_FILE_ERR);
    }

    fprintf (html_file, "</body>\n</html>");

    fclose (html_file);

    return LIST_SUCCESS;
}

//--------------------------------------------------------------------------------