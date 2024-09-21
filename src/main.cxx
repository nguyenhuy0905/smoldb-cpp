#ifndef ENABLE_MODULE
#include "table.hxx"
#else
import smoldb_cpp;
#endif // !ENABLE_MODULE

auto main() -> int {
    smoldb::Table tab{};
    return 0;
}
