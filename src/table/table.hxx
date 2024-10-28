#ifndef SMOLDB_CPP_TABLE_HXX
#define SMOLDB_CPP_TABLE_HXX

#include <type_traits>
#ifndef ENABLE_MODULE
#include "column_meta.hxx"
#include <expected>
#include <span>
#include <string_view>
#include <vector>
#endif // !ENABLE_MODULE

#include "module_cfg.h"

SMOLDB_CPP_EXPORT namespace smoldb {

    /**
     * @class Table
     * @brief Represents a table in a database file.
     *
     */
    class Table {
        enum class TableAccessError {
            IndexOutOfBounds = 1,
        };

      public:
        Table() = delete;
        Table(std::string_view name, std::span<ColumnMeta> cols = {});
        /**
         * @return The name of the table.
         */
        [[nodiscard]] constexpr auto
        get_name() const noexcept -> std::string_view {
            return this->m_name;
        }

        /**
         * @return The number of columns inside the table.
         */
        [[nodiscard]] constexpr auto
        get_n_cols() const noexcept -> std::size_t {
            return this->m_cols.size();
        }

        /**
         * @brief Gets the `ColumnMeta` at the specified index.
         * Returns a reference to that `ColumnMeta` if index is in bounds.
         * Otherwise, return an IndexOutOfBounds error.
         *
         * @param pos
         */
        [[nodiscard]] constexpr auto get_col_meta_at(std::uint16_t pos) const
            -> std::expected<std::reference_wrapper<const ColumnMeta>,
                             TableAccessError>;

        template <typename T>
            requires std::is_same_v<T, ColumnMeta>
        constexpr void add_column(T&& col) {
            m_cols.push_back(std::forward<T>(col));
        }

      private:
        /**
         * @brief Name of the table.
         */
        std::string m_name;
        /**
         * @brief The list of column metadata.
         */
        std::vector<ColumnMeta> m_cols;
        /**
         * @brief A skip list of column offsets.
         */
        std::vector<std::uint16_t> m_offset;

        friend class TableBuilder;
    };

    [[nodiscard]] constexpr auto Table::get_col_meta_at(std::uint16_t pos)
        const -> std::expected<std::reference_wrapper<const ColumnMeta>,
                               TableAccessError> {
        if (pos >= this->m_cols.size()) {
            return std::unexpected(TableAccessError::IndexOutOfBounds);
        }
        return {static_cast<const ColumnMeta&>(this->m_cols.at(pos))};
    }
}

#endif // !SMOLDB_CPP_TABLE_HXX
