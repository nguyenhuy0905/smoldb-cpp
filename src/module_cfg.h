#ifndef SMOLDB_CPP_MODULE_CFG_H
#define SMOLDB_CPP_MODULE_CFG_H

#include "myproj_export.h" // NOLINT

/*
 * Simply switch SMOLDB_CPP_EXPORT into export (module)
 * */
    #ifdef ENABLE_MODULE
        #ifdef SMOLDB_CPP_EXPORT
            #undef SMOLDB_CPP_EXPORT
            #define SMOLDB_CPP_EXPORT export
        #endif
    #endif
#endif
