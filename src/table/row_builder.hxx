#ifndef SMOLDB_CPP_ROW_BUILDER_HXX
#define SMOLDB_CPP_ROW_BUILDER_HXX

#include "row.hxx"
#include <cstdint>
#include <expected>
#include <functional>

namespace smoldb {

/**
 * @class RowBuilder
 * @brief Utility class to build a row.
 *
 */
class RowBuilder {
  public:
    enum class RowBuildingError : uint8_t {
        IDAlreadySet = 1,
        IDNotSet,
        NoColumnAdded,
    };

    /**
     * @brief Constructs a new RowBuilder instance.
     * Imo this looks nicer than callling a constructor, given how this class
     * is going to be used.
     *
     * @return
     */
    static auto init() -> RowBuilder { return RowBuilder{}; }

    /**
     * @brief Sets the rowid, if not already set. Otherwise, return the error
     * RowBuildingError::IDAlreadySet.
     *
     * @param id the new rowid.
     * @return the reference to this row builder, or the error.
     */
    auto set_rowid(std::uint32_t id)
        -> std::expected<std::reference_wrapper<RowBuilder>, RowBuildingError> {
        if (!is_rowid_set) {
            return std::unexpected(RowBuildingError::IDAlreadySet);
        }
        m_result.m_rowid = id;
        is_rowid_set = true;
        return {*this};
    }
    
    // TODO: change_rowid which allows to change rowid after it's set.

    /**
     * @brief Adds a column to the row.
     * This function won't return any error. The return type is still
     * std::expected to keep it consistent with other functions.
     *
     * @return the reference to this row builder.
     */
    auto add_column(auto&& col)
        -> std::expected<std::reference_wrapper<RowBuilder>, RowBuildingError> {
        any_col_added = true;
        m_result.m_cols.push_back(std::forward<decltype(col)>(col));
        return {*this};
    }

    /**
     * @brief Returns the resultant row, if there's no error. Otherwise:
     *   - If ID is not yet set, returns RowBuildingError::IDNotSet.
     *   - If no column has been added, returns RowBuildingError::NoColumnAdded.
     *
     * @return the resultant row.
     */
    [[nodiscard]] auto build() const -> std::expected<Row, RowBuildingError> {
        if (!any_col_added) {
            return std::unexpected(RowBuildingError::NoColumnAdded);
        }
        if (!is_rowid_set) {
            return std::unexpected(RowBuildingError::IDNotSet);
        }
        return {std::move(m_result)};
    }

  private:
    Row m_result;
    bool is_rowid_set{false};
    bool any_col_added{false};
};

} // namespace smoldb

#endif // !SMOLDB_CPP_ROW_BUILDER_HXX
