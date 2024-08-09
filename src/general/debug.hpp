#ifndef SMOLDB_GENERAL_DEBUG_H
#define SMOLDB_GENERAL_DEBUG_H

// this file is supposed to define the macro, by which some not-exported stuff
// in release mode of a module gets exported in debug mode.

#ifdef DEBUG
#define EXPORT_DEBUG export
#else
#define EXPORT_DEBUG
#endif // DEBUG

#endif // !SMOLDB_GENERAL_DEBUG_H
