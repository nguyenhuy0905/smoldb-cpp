#ifndef SMOLDB_CPP_MODULE_CFG_H
#define SMOLDB_CPP_MODULE_CFG_H

// #include "smoldb_cpp_export.h" // NOLINT

/*
 * Simply switch SMOLDB_CPP_EXPORT into export (module)
 * */
#ifdef ENABLE_MODULE
#ifdef SMOLDB_CPP_EXPORT
#undef SMOLDB_CPP_EXPORT
#define SMOLDB_CPP_EXPORT export
#else
#define SMOLDB_CPP_EXPORT export
#endif // SMOLDB_CPP_EXPORT
#else
#ifndef SMOLDB_CPP_EXPORT
#define SMOLDB_CPP_EXPORT
#endif // !SMOLDB_CPP_EXPORT
#endif // ENABLE_MODULE
#endif // !SMOLDB_CPP_MODULE_CFG_H
