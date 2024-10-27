#ifndef SMOLDB_CPP_TABLE_BUILDER_HXX
#define SMOLDB_CPP_TABLE_BUILDER_HXX

#include "column_meta.hxx"
#include "module_cfg.h"
#include <type_traits>

#ifndef ENABLE_MODULE
#include "table.hxx"
#include <expected>
#endif // !ENABLE_MODULE

/**
 * `TableBuilder` is not exposed to the end user.
 */

SMOLDB_CPP_EXPORT namespace smoldb {

    /**
     * @class `TableBuilder`
     * @brief The whole purpose of this is to prevent the end-user from playing
     * with the `ColumnMeta` builder alone.
     *
     * Constructing a `ColumnMeta` should be done in the context of a database
     * file. (coming soon)
     */
    class TableBuilder {
      public:
        TableBuilder(std::string_view tbl_name) : m_tbl(tbl_name) {}

        // TODO: stop being lazy and provide the arguments needed.
        template <typename... Args> static auto from(Args&&... args) -> Table {
            return Table{std::forward<Args>(args)...};
        }

        /**
         * @brief Adds a column into the table. This overload pushes back the
         * `ColumnMeta` passed in.
         * The function returns a reference to this `TableBuilder` so that
         * column additions can be chained.
         *
         * @param col
         */
        template <typename T>
            requires std::is_same_v<T, ColumnMeta>
        constexpr auto add_column(T&& col) -> TableBuilder& {
            m_tbl.m_cols.push_back(std::forward<T>(col));
            return *this;
        }

        /**
         * @brief Adds a column into the table. This overload emplaces back the
         * construction arguments for a `ColumnMeta`.
         * The function returns a reference to this `TableBuilder` so that
         * column additions can be chained.
         *
         * @tparam Args
         * @param args
         */
        template <typename... Args>
        constexpr auto add_column(Args&&... args) -> TableBuilder& {
            m_tbl.m_cols.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        /**
         * @brief Builds the table.
         *
         * @return The table
         */
        constexpr auto build() -> Table&& { return std::move(m_tbl); }

      private:
        Table m_tbl;
    };

} // namespace smoldb

#endif // !SMOLDB_CPP_TABLE_BUILDER_HXX
