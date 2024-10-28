#ifndef ENABLE_MODULE
#include "table.hxx"
#else
import smoldb_cpp;
#endif // !ENABLE_MODULE

auto main() -> int {
    auto tbl = smoldb::Table{"tbl"};
    tbl.add_column();
    return 0;
}
