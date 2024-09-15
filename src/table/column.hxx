#ifndef SMOLDB_CPP_COLUMN_HXX
#define SMOLDB_CPP_COLUMN_HXX
#include <cstdint>
#include <expected>
#include <string>
#include <variant>

namespace smoldb {

template <typename T>
concept CT = requires {
    std::is_same_v<T, std::uint32_t> || std::is_same_v<T, std::string>;
};

template <CT T> class Column {
  public:
    Column(std::uint32_t id, std::string_view name)
        : m_name(name), m_col_id(id) {}

    // I know what auto&& means already, but gosh, I hate how it looks
    // so similar to a rvalue reference.
    Column(std::uint32_t id, std::string_view name, auto&& data)
        : m_name(name), m_data(std::forward<T>(data)), m_col_id(id) {}

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
    std::string m_name;
    T m_data;
    std::uint32_t m_col_id{0};
};

using ColumnVariant = std::variant<Column<std::uint32_t>, Column<std::string>>;

} // namespace smoldb

#endif // !SMOLDB_CPP_COLUMN_HXX