#ifndef SMOLDB_CPP_COLUMN_BUILDER_HXX
#define SMOLDB_CPP_COLUMN_BUILDER_HXX

#include "column.hxx"
#include <expected>

namespace smoldb {

class ColumnBuilder;

/**
 * @class ColumnBuilder
 * @brief Literally just a wrapper around Column.
 *
 * This dirty trick is to restrict end-user from constructing Columns by themselves.
 *
 */
class ColumnBuilder {
  public:
    template <CT T>
    static auto build(std::uint32_t id, std::string_view name) -> Column<T> {
        return std::move(Column<T>{id, name});
    }

    template <CT T>
    static auto build(std::uint32_t id, std::string_view name,
                      auto&& data) -> Column<T> {
        return std::move(Column<T>{id, name, std::forward<T>(data)});
    }
  private:
};

} // namespace smoldb

#endif // !SMOLDB_CPP_COLUMN_BUILDER_HXX
