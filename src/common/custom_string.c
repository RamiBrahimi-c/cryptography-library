#include "custom_string.h"



int cstrcpy(uchar_t *dest , CString src) {
    if (dest == NULL)
    {
        fprintf(stderr , "ERROR : dest is NULL\n");
        return -1;
    }
    
    for (size_t i = 0; i < src.length; i++)
    {
        dest[i] = src.string[i];
    }
    dest[src.length]=0x0 ;
    return 0;
}