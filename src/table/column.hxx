#ifndef SMOLDB_CPP_COLUMN_HXX
#define SMOLDB_CPP_COLUMN_HXX
#include "data_type.hxx"
#include <cstdint>
#include <expected>
#include <string>
#include <type_traits>
#include <variant>

namespace smoldb {

template <typename T>
concept CT =
    requires { std::is_same_v<T, UInt32> || std::is_same_v<T, VarChar>; };

enum class ColumnFlags : uint8_t {
    PrimaryKey = 1 << 0,
    // SecondaryKey = 1 << 1,
    // Nullable = 1 << 2,
    // AutoIncrement = 1 << 3,
    Nonsense = PrimaryKey + 1,
};

template <CT T> class ColumnBuilder;

template <CT T> class Column {
  public:
    /**
     * @return the name of this column.
     */
    [[nodiscard]] auto get_name() const noexcept -> std::string_view {
        return m_name;
    }

    /**
     * @return this column's ID
     */
    [[nodiscard]] auto get_id() const noexcept -> std::uint32_t {
        return m_col_id;
    }

    /**
     * @return the current data of this column.
     */
    [[nodiscard]] auto get_data() const noexcept -> const T& { return m_data; }

    /**
     * Sets the current data of this column to the specified value.
     *
     * @param data
     */
    void set_data(auto&& data) noexcept { m_data = std::forward<T>(data); }

  private:
    Column() = default;

    Column(std::uint32_t id, std::string_view name)
        : m_name(name), m_col_id(id) {}

    // I know what auto&& means already, but gosh, I hate how it looks
    // so similar to a rvalue reference.
    Column(std::uint32_t id, std::string_view name, auto&& data)
        : m_name(name), m_data(std::forward<T>(data)), m_col_id(id) {}

    std::string m_name;
    T m_data{};
    std::uint32_t m_col_id{0};
    std::underlying_type_t<ColumnFlags> m_flags{0};

    friend class ColumnBuilder<T>;
};

/**
 * @brief Convenient operator to chain flags together. Useful for ColumnBuilder.
 *
 * @return the underlying type of ColumnFlags that can be used to assign to
 * a column.
 */
constexpr auto
operator|(std::underlying_type_t<ColumnFlags> lhs,
          ColumnFlags rhs) -> std::underlying_type_t<ColumnFlags> {
    return static_cast<std::underlying_type_t<decltype(rhs)>>(rhs) | lhs;
}

/**
 * @brief Convenient operator to chain flags together. Useful for ColumnBuilder.
 *
 * @return the underlying type of ColumnFlags that can be used to assign to
 * a column.
 */
constexpr auto operator|(ColumnFlags lhs, ColumnFlags rhs)
    -> std::underlying_type_t<ColumnFlags> {
    return static_cast<std::underlying_type_t<decltype(rhs)>>(rhs) | lhs;
}

/**
 * @brief Basically operator| but for multiple elements.
 *
 * @param lhs
 * @param rhs
 * @return
 */
constexpr auto combine(ColumnFlags lhs,
                       ColumnFlags rhs) -> std::underlying_type_t<ColumnFlags> {
    return lhs | rhs;
}

/**
 * @brief Basically operator| but for multiple elements.
 *
 * @param lhs
 * @param rhs
 * @return
 */
template <typename... Args>
    requires std::same_as<ColumnFlags,
                          std::remove_cvref_t<std::common_type_t<Args...>>>
constexpr auto combine(ColumnFlags lhs,
                       Args... rhs) -> std::underlying_type_t<ColumnFlags> {
    return lhs | static_cast<decltype(lhs)>(combine(rhs...));
}

using ColumnVariant = std::variant<Column<UInt32>, Column<VarChar>>;

} // namespace smoldb

#endif // !SMOLDB_CPP_COLUMN_HXX
