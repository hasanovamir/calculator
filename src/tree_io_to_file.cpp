#include "tree.h"
#include "general_io_file_func.h"
#include "list.h"

//--------------------------------------------------------------------------------

tree_err_t 
WriteTreeToFile (TreeContext_t* TreeContext)
{
    DEBUG_ASSERT (TreeContext != NULL);

    FILE* file = fopen ("tree_data_base_1.txt", "w");

    if (file == NULL) {
        PRINTERR (TREE_OPEN_FILE_ERR);
        return   (TREE_OPEN_FILE_ERR);
    }

    char* NodeBuffer = (char*) calloc (TreeContext->size * MAXSTRINGSIZE, sizeof (char));

    if (NodeBuffer == NULL) {
        PRINTERR (TREE_ALLOC_ERR);
        return   (TREE_ALLOC_ERR);
    }

    PrintNode (TreeContext->head, NodeBuffer);

    fwrite (NodeBuffer, sizeof (char), strlen (NodeBuffer), file);

    fclose (file);

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

void 
PrintNode (const TreeNode_t* Node, char* NodeBuffer)
{
    DEBUG_ASSERT (Node != NULL);

    strcat (NodeBuffer, "(");

    char string[MAXSTRINGSIZE] = "";
    sprintf (string, "\"%s\"", Node->node_data);

    strcat (NodeBuffer, string);

    if (Node->cond_true) {
        PrintNode (Node->cond_true, NodeBuffer);
    }
    else {
        strcat (NodeBuffer, "nil");
    }

    if (Node->cond_false) {
        PrintNode (Node->cond_false, NodeBuffer);
    }
    else {
        strcat (NodeBuffer, "nil");
    }

    strcat (NodeBuffer, ")");
}

//--------------------------------------------------------------------------------

TreeNode_t* 
ReadNode (TreeContext_t* TreeContext, TreeNode_t* ParentNode, char* buffer, int* pos)
{
    DEBUG_ASSERT (TreeContext != NULL);
    DEBUG_ASSERT (buffer      != NULL);

    if (buffer[*pos] == '(') {
        (*pos) += 2;

        TreeNode_t* NewNode = NULL;
        long long offset = strchr (buffer + *pos, '\"') - buffer - *pos;

        MakeNode (TreeContext, &NewNode);

        strncpy (NewNode->node_data, buffer + *pos, offset);

        NewNode->prev_node = ParentNode;

        *pos += offset + 1;

        NewNode->cond_true  = ReadNode (TreeContext, NewNode, buffer, pos);
        NewNode->cond_false = ReadNode (TreeContext, NewNode, buffer, pos);

        if (NewNode->cond_true == NULL && NewNode->cond_false == NULL) {
            NewNode->status = WORD;
        }

        (*pos)++;

        TreeContext->size += 1;

        return NewNode;
    }
    else if (strncmp (buffer + *pos, "nil", 3) == 0) {
        (*pos) += 3;
        return NULL;
    }

    return NULL;
}

//--------------------------------------------------------------------------------

tree_err_t 
TreeReadDataBase (TreeContext_t* TreeContext, const char* FileName)
{
    DEBUG_ASSERT (TreeContext != NULL);
    DEBUG_ASSERT (FileName    != NULL);

    int size = get_file_size (FileName);

    if (size == -1) {
        PRINTERR (TREE_SYS_FUNC_ERR);
        return   TREE_SYS_FUNC_ERR;
    }

    char* buffer = ( char* ) calloc ( size, sizeof (char) );
    int pos = 0;

    if (ReadBuffer (buffer, FileName, size)) {
        return TREE_OPEN_FILE_ERR;
    }

    TreeContext->head = ReadNode (TreeContext, nullptr, buffer, &pos);

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------

tree_err_t 
ReadBuffer (char* buffer, const char* FileName, int size)
{
    DEBUG_ASSERT (buffer   != NULL);
    DEBUG_ASSERT (FileName != NULL);

    FILE* file = fopen (FileName, "r");

    if (file == NULL) {
        PRINTERR (TREE_OPEN_FILE_ERR);
        return   TREE_OPEN_FILE_ERR;
    }

    int read_size = ( int ) fread ( buffer, sizeof ( char ), size, file );

    if ( read_size != size ) {
        PRINTERR (TREE_INVALID_FILE_SIZE);
        return TREE_INVALID_FILE_SIZE;
    }

    fclose (file);

    return TREE_SUCCESS;
}

//--------------------------------------------------------------------------------