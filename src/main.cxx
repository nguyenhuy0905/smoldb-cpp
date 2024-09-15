#include "row.hxx"
#include "row_builder.hxx"
#include <print>
auto main() -> int {
    // placeholder

    auto row =
        smoldb::RowBuilder::init()
            .set_rowid(0)
            .and_then([](smoldb::RowBuilder& rowb) {
                return rowb.add_column(
                    smoldb::Column<std::uint32_t>{1, "TestCol", 2U});
            })
            .and_then([](smoldb::RowBuilder& rowb) { return rowb.build(); });

    if (row.has_value()) {
        std::println("Row successfully built");
    } else {
        std::println("Row unsuccessully built");
        using RowError = smoldb::RowBuilder::RowBuildingError;
        switch (row.error()) {
        case RowError::IDAlreadySet:
            std::println("ID already set");
            return 1;
        case RowError::IDNotSet:
            std::println("ID not set");
            return 1;
        case RowError::NoColumnAdded:
            std::println("No column added");
            return 1;
        }
    }

    std::println("{}", row->get_data_at<uint32_t>(0)->get());
    row->set_data_at<std::uint32_t>(0, 1U);
    std::println("{}", row->get_data_at<uint32_t>(0)->get());

    return 0;
}
