#include "column.hxx"
#include "column_builder.hxx"
#include "row.hxx"
#include "row_builder.hxx"
#include <iostream>
#include <print>
auto main() -> int {
    // placeholder

    std::uint8_t something = smoldb::combine(smoldb::ColumnFlags::PrimaryKey,
                                             smoldb::ColumnFlags::PrimaryKey,
                                             smoldb::ColumnFlags::Nonsense);

    // i don't know why I need the + 0 at the end though.
    // it seems that this damn language just treats it as an unsigned char.

    std::cout << "value of something is: " << something + 0 << "\n";

    std::cout << "value of chain ORs is: "
              << (smoldb::ColumnFlags::PrimaryKey |
                  smoldb::ColumnFlags::PrimaryKey |
                  smoldb::ColumnFlags::PrimaryKey) +
                     0
              << "\n";

    auto ret =
        smoldb::ColumnBuilder<smoldb::UInt32>::build(1, 1U, "Testcol", 1U);

    if (!ret.has_value()) {
        std::cout << "Print failure!!!\n";
        return 1;
    }

    auto ret_val = ret.value();

    std::println("Column:\n\tID: {}\n\tName: {}\n\tvalue: {}", ret_val.get_id(),
                 ret_val.get_name(), ret_val.get_data());

    return 0;
}
