#include "column.hxx"
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
                  smoldb::ColumnFlags::PrimaryKey) + 0
              << "\n";

    return 0;
}
