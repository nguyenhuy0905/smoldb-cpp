#ifndef SMOLDB_CPP_DATA_TYPE_HXX
#define SMOLDB_CPP_DATA_TYPE_HXX

#include "module_cfg.h"

#ifndef ENABLE_MODULE
#include <cstdint>
#endif // !ENABLE_MODULE

SMOLDB_CPP_EXPORT namespace smoldb {

using UInt32 = std::uint32_t;
using VarChar = std::string;

}

#endif // !SMOLDB_CPP_DATA_TYPE_HXX
