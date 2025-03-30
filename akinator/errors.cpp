#include "errors.h"

const char* errors_messenger(Tree_errors status)
{
    switch(status)
    {
        case          SUCCESS: return "SUCCESS";
        case    FILE_OPEN_ERR: return "FILE OPEN ERROR";
        case   FILE_CLOSE_ERR: return "FILE CLOSE ERROR";
        case      NULLPTR_ERR: return "NULL POINTER";
        case MEMORY_ALLOC_ERR: return "MEMORY ALLOCATION ERROR";
        case        FREAD_ERR: return "FREAD ERROR: Not fully read";

        default:               return "UNKNOWN ERROR";
    }

}