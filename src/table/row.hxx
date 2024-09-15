#ifndef SMOLDB_CPP_ROW_HXX
#define SMOLDB_CPP_ROW_HXX

#include "column.hxx"
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
        -> std::expected<const T&, DataAccessError> {

        if (index >= m_cols.size()) {
            return std::unexpected(DataAccessError::IndexOutOfBound);
        }
        if (not std::holds_alternative<T>(m_cols[index])) {
            return std::unexpected(DataAccessError::InvalidDataType);
        }

        return {std::get<T>(m_cols[index])};
    }

    [[nodiscard]] auto get_rowid() const noexcept -> std::size_t {
        return m_rowid;
    }

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
        if (not std::holds_alternative<T>(m_cols[index])) {
            return std::unexpected(DataAccessError::InvalidDataType);
        }

        m_cols[index] = std::forward<T>(data);
        return {};
    }

  private:
    std::vector<ColumnVariant> m_cols;
    std::size_t m_rowid;

    friend class RowBuilder;
};

} // namespace smoldb

#endif // !SMOLDB_CPP_ROW_HXX
