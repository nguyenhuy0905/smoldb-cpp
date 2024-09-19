#ifndef SMOLDB_CPP_COLUMN_BUILDER_HXX
#define SMOLDB_CPP_COLUMN_BUILDER_HXX

#include "column.hxx"
#include <expected>
#include <type_traits>

namespace smoldb {

enum class ColumnBuildingError : uint8_t {
    NonsensicalFlag = 1,
    ColIDNotSet,
    ColIDAlreadySet,
};
/**
 * @class ColumnBuilder
 * @brief Literally just a wrapper around Column.
 *
 * This dirty trick is to restrict end-user from constructing Columns by
 * themselves.
 *
 */
template <CT T> class ColumnBuilder {
  public:
    /**
     * @brief Constructs a new ColumnBuilder.
     *
     */
    static auto init() -> ColumnBuilder;

    template <typename Tp>
        requires std::same_as<std::remove_cvref_t<Tp>, T>
    auto set_value(Tp&& val) -> std::reference_wrapper<ColumnBuilder> {
        m_build.set_data(std::forward<T>(val));
        return this;
    }

    /**
     * @brief Sets the column flag to the specified flag.
     *
     * @param flag
     */
    auto set_column_flags(std::underlying_type_t<ColumnFlags> flag)
        -> std::expected<std::reference_wrapper<ColumnBuilder>,
                         ColumnBuildingError> {
        if (flag >= static_cast<std::underlying_type_t<ColumnFlags>>(
                        ColumnFlags::Nonsense)) {
            return std::unexpected(ColumnBuildingError::NonsensicalFlag);
        }

        m_build.m_flags = flag;
        return {this};
    }

    /**
     * @brief Sets the column ID if already set. If not already set, return
     * ColumnBuildingError::ColIDNotSet. Otherwise, return a reference wrapper
     * around this ColumnBuilder.
     *
     * @param colid
     */
    auto
    change_column_id(std::underlying_type_t<typename Column<T>::m_col_id> colid)
        -> std::expected<std::reference_wrapper<ColumnBuilder>,
                         ColumnBuildingError> {
        if (!is_colid_set) {
            return std::unexpected(ColumnBuildingError::ColIDNotSet);
        }
        m_build.m_col_id = colid;
        return {this};
    }

    /**
     * @brief Sets the column ID if not already set. If already set, return
     * ColumnBuildingError::ColIDAlreadySet. Otherwise, return a reference
     * wrapper around this ColumnBuilder.
     *
     * @param colid
     */
    auto
    set_column_id(std::underlying_type_t<typename Column<T>::m_col_id> colid)
        -> std::expected<std::reference_wrapper<ColumnBuilder>,
                         ColumnBuildingError> {
        if (is_colid_set) {
            return std::unexpected(ColumnBuildingError::ColIDAlreadySet);
        }
        is_colid_set = true;
        // this does incur 1 unnecessary check. But, it should not be so slow that
        // it's concerning.
        return change_column_id(colid);
    }

  private:
    Column<T> m_build;
    bool is_colid_set{false};
};

} // namespace smoldb

#endif // !SMOLDB_CPP_COLUMN_BUILDER_HXX
