#ifndef MACROS_H
#define MACROS_H

//————————————————————————————————————————————————————————————————————————————————

#include "colors.h"

//————————————————————————————————————————————————————————————————————————————————

const int MAXSTRINGSIZE    = 256;
const int MAXCOMMANDSIZE   = 128;
const int MAXFILENAMESIZE  = 32;
const int AllocCapacity    = 512;
const int COMMONSTACKSIZE  = 128;
const int COMMONSTRINGSIZE = 100;
const int StartBigArrayCap = 8;

//————————————————————————————————————————————————————————————————————————————————

#ifndef N_DEBUG
#define DEBUG_ASSERT(cond)\
if (!(cond))\
{\
    fprintf(stderr, "%s in %s:%d func:%s\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
    exit(1);\
}
#else
#define DEBUG_ASSERT(cond) ;
#endif //N_DEBUG

//————————————————————————————————————————————————————————————————————————————————

#define PRINTERR(str)\
fprintf (stderr, COLOR_RED "%s" COLOR_RESET " in %s:%d in %s", #str, __FILE__, __LINE__, __PRETTY_FUNCTION__);

//————————————————————————————————————————————————————————————————————————————————

#endif //MACROS_H