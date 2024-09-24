#ifndef ENABLE_MODULE
#include "table.hxx"
#include "table_builder.hxx"
#else
import smoldb_cpp;
#endif // !ENABLE_MODULE

auto main() -> int {
    smoldb::Table tab = smoldb::TableBuilder::from("tab");
    return 0;
}
