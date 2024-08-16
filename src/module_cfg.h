#ifndef MYPROJ_MODULE_CFG_H
#define MYPROJ_MODULE_CFG_H

#include "myproj_export.h" // NOLINT

/*
 * Simply switch MYPROJ_EXPORT into export (module)
 * */
    #ifdef ENABLE_MODULE
        #ifdef MYPROJ_EXPORT
            #undef MYPROJ_EXPORT
            #define MYPROJ_EXPORT export
        #endif
    #endif
#endif
