#ifndef LIST_H
#define LIST_H

//————————————————————————————————————————————————————————————————————————————————

#define POISON 0x66677752

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_types.h"
#include "macros.h"


//————————————————————————————————————————————————————————————————————————————————

typedef tree_node_t list_data_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{ 
    LIST_SUCCESS,
    LIST_INVALID_IDX,
    LIST_INVALID_CAPACITY_SIZE,
    LIST_ALLOCATE_ERR,
    LIST_UP_SIZE_ERR,
    LIST_GET_NEXT_FREE_ERR,
    LIST_CHANGE_NEXT_ERR,
    LIST_CHANGE_PREV_ERR,
    LIST_OPEN_FILE_ERR,
    LIST_SYS_FUNC_ERR,
    LIST_INVALID_FREE_VAL,
    LIST_INVALID_HEAD,
    LIST_INVALID_TAIL,
    LIST_INVALID_SIZE,
    LIST_INCORRECT_NUMBERING,
} list_err_t;

//————————————————————————————————————————————————————————————————————————————————

struct list_t
{
    list_data_t* data;
    int* next;
    int  head;
    int  tail;
    int  free;
    int  size;
    int  capacity;
    int  num_calls;
};

//————————————————————————————————————————————————————————————————————————————————

void       MemSetList        (list_t* list);
list_err_t ListDestroy       (list_t* list);
list_err_t ChangeFree        (list_t* list);
list_err_t IncreaseList      (list_t* list);
list_err_t ListInsertToStart (list_t* list, int val);
list_err_t ListInsertToEnd   (list_t* list, int val);
list_err_t ListInit          (list_t* list, int capacity          );
list_err_t ListInsertAfter   (list_t* list, int idx, int  val     );
list_err_t ListInsertBefore  (list_t* list, int idx, int  val     );
list_err_t ListGetNext       (list_t* list, int idx, int* next_idx);
list_err_t ListGetFree       (list_t* list, int idx, int* free_idx);
list_err_t ListPop           (list_t* list, int idx, int* ret_val );
list_err_t FillDotFile       (list_t* list, const char* file_name);
void       MakeDumpTitle     (FILE* dot_file);
void       MakeDumpNode      (FILE* dot_file, int node_number, int idx, list_data_t data, int next, const char* color);
void       MakeDumpEdge      (FILE* dot_file, int node_num_1, int node_num_2, const char* color);
list_err_t ListDump          (list_t* list);
list_err_t ListStartFillHtml (void);
list_err_t ListFillHtml      (list_t* list, const char* file_name);
list_err_t ListEndFillHtml   (void);

//————————————————————————————————————————————————————————————————————————————————

#endif //LISTH