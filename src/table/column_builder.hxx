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
    // future uses
    // NullValueOnNonNull,
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
    static auto init() -> ColumnBuilder { return ColumnBuilder{}; }

    auto set_name(std::string_view strv)
        -> std::expected<std::reference_wrapper<ColumnBuilder>,
                         ColumnBuildingError> {
            m_build.m_name = strv;
            return {*this};
        }

    template <typename Tp>
        requires std::same_as<std::remove_cvref_t<Tp>, T>
    auto
    set_value(Tp&& val) -> std::expected<std::reference_wrapper<ColumnBuilder>,
                                         ColumnBuildingError> {
        m_build.set_data(std::forward<T>(val));
        return {*this};
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
        return {*this};
    }

    /**
     * @brief Sets the column ID if already set. If not already set, return
     * ColumnBuildingError::ColIDNotSet. Otherwise, return a reference wrapper
     * around this ColumnBuilder.
     *
     * @param colid
     */
    auto change_column_id(std::uint32_t colid)
        -> std::expected<std::reference_wrapper<ColumnBuilder>,
                         ColumnBuildingError> {
        if (!is_colid_set) {
            return std::unexpected(ColumnBuildingError::ColIDNotSet);
        }
        m_build.m_col_id = colid;
        return {*this};
    }

    /**
     * @brief Sets the column ID if not already set. If already set, return
     * ColumnBuildingError::ColIDAlreadySet. Otherwise, return a reference
     * wrapper around this ColumnBuilder.
     *
     * @param colid
     */
    auto set_column_id(std::uint32_t colid)
        -> std::expected<std::reference_wrapper<ColumnBuilder>,
                         ColumnBuildingError> {
        if (is_colid_set) {
            return std::unexpected(ColumnBuildingError::ColIDAlreadySet);
        }
        is_colid_set = true;
        // this does incur 1 unnecessary check. But, it should not be so slow
        // that it's concerning.
        return change_column_id(colid);
    }

    auto build() -> std::expected<Column<T>, ColumnBuildingError> {
        if (!is_colid_set) {
            return std::unexpected(ColumnBuildingError::ColIDNotSet);
        }

        return std::move(m_build);
    }

    template <typename Tp>
        requires std::same_as<std::remove_cvref_t<Tp>, T>
    static auto build(std::uint32_t colid, Tp&& val, std::string_view name, std::uint8_t flag)
        -> std::expected<Column<T>, ColumnBuildingError> {
        return ColumnBuilder::init()
            .set_column_id(colid)
            .and_then([&](ColumnBuilder& colb) {
                return colb.set_value(std::forward<T>(val));
            })
            .and_then([=](ColumnBuilder& colb) {
                return colb.set_column_flags(flag);
            }).and_then([&](ColumnBuilder& colb) {
                return colb.set_name(name); 
            })
            .and_then([](ColumnBuilder& colb) { return colb.build(); });
    }

  private:
    ColumnBuilder() = default;
    Column<T> m_build;
    bool is_colid_set{false};
};

} // namespace smoldb

#endif // !SMOLDB_CPP_COLUMN_BUILDER_HXX
