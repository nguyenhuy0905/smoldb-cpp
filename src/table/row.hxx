#ifndef SMOLDB_CPP_ROW_HXX
#define SMOLDB_CPP_ROW_HXX

#include "column.hxx"
#include "data_type.hxx"
#include <cstdint>
#include <expected>
#include <variant>
#include <vector>

namespace smoldb {

class Row {
  public:
    enum class DataAccessError : uint8_t {
        InvalidDataType = 1,
        IndexOutOfBound = 2,
    };
    /**
     * @brief Returns the column data at the specified index, if data
     * types match. Otherwise, return `DataAccessError::InvalidDataType`.
     *
     * If `index` is out of bounds, return `DataAccessError::IndexOutOfBound`.
     *
     * @param index
     * @return The column data at the specified index.
     */
    template <CT T>
    [[nodiscard]] auto get_data_at(std::size_t index) const
        -> std::expected<std::reference_wrapper<const T>, DataAccessError> {

        if (index >= m_cols.size()) {
            return std::unexpected(DataAccessError::IndexOutOfBound);
        }
        if (not std::holds_alternative<Column<T>>(m_cols[index])) {
            return std::unexpected(DataAccessError::InvalidDataType);
        }

        const Column<T>& col = std::get<Column<T>>(m_cols[index]);

        return {col.get_data()};
    }

    /**
     * @brief Returns the row ID of this row.
     *
     * @return the row ID of this row.
     */
    [[nodiscard]] auto get_rowid() const noexcept -> std::size_t {
        return m_rowid;
    }

    /**
     * @brief Sets the data at specified column to the specifed value.
     * Returns nothing if successful.
     * Returns `DataAccessError::IndexOutOfBound` if index is out of bound.
     * Returns `DataAccessError::InvalidDataType` if `T` is not the same as
     * the current alternative held by the column.
     *
     * @param index the specifed column index.
     * @param data the specifed value.
     */
    template <CT T>
    auto set_data_at(std::size_t index,
                     auto&& data) -> std::expected<void, DataAccessError> {
        if constexpr (std::is_same_v<std::remove_reference<decltype(data)>,
                                     T>) {
            return std::unexpected(DataAccessError::InvalidDataType);
        }

        if (index >= m_cols.size()) {
            return std::unexpected(DataAccessError::IndexOutOfBound);
        }
        // technically this ccould be an `if constexpr`, but in no situation
        // will this be evaluated at compile time. So, `constexpr` here would
        // be misleading.
        if (not std::holds_alternative<Column<T>>(m_cols[index])) {
            return std::unexpected(DataAccessError::InvalidDataType);
        }

        Column<T>& col = std::get<Column<T>>(m_cols[index]);
        col.set_data(std::forward<T>(data));

        return {};
    }

    /**
     * @brief A wrapper around `set_data_at` for varchar type.
     * Returns nothing if successful.
     * Returns `DataAccessError::IndexOutOfBound` if index is out of bound.
     * Returns `DataAccessError::InvalidDataType` if the current value held
     * by the specified column is not `varchar` (aka, `std::string`)
     *
     * @param index the specifed column index.
     * @param data the specifed value.
     */
    auto set_varchar_data_at(std::size_t index, auto&& data)
        -> std::expected<void, DataAccessError> {
        return set_data_at<VarChar>(index, std::forward<std::string>(data));
    }

    /**
     * @brief A wrapper around `set_data_at` for uint32 type.
     * Returns nothing if successful.
     * Returns `DataAccessError::IndexOutOfBound` if index is out of bound.
     * Returns `DataAccessError::InvalidDataType` if the current value held
     * by the specified column is not `uint32`
     *
     * @param index the specifed column index.
     * @param data the specifed value.
     */
    auto set_uint32_data_at(std::size_t index, auto&& data)
        -> std::expected<void, DataAccessError> {
        return set_data_at<UInt32>(index, std::forward<std::uint32_t>(data));
    }

  private:
    Row() = default;
    std::vector<ColumnVariant> m_cols;
    std::size_t m_rowid{0};

    friend class RowBuilder;
};

} // namespace smoldb

#endif // !SMOLDB_CPP_ROW_HXX
