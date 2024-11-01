#ifndef SMOLDB_CPP_COLUMN_META_HXX
#define SMOLDB_CPP_COLUMN_META_HXX
#include "module_cfg.h"

#ifndef ENABLE_MODULE
#include <cstdint>
#include <string>
#include <type_traits>
#endif // !ENABLE_MODULE

SMOLDB_CPP_EXPORT namespace smoldb {

    /**
     * @brief The types recognized by `smoldb`.
     *
     * Values larger or equal to `VarChar` are variable-length;
     * the others are fixed length.
     */
    enum class ColumnType : std::uint8_t {
        UInt,
        Int,
        UByte,
        Byte,
        UShort,
        Short,
        ULong,
        Long,
        VarChar,
        Text,
        Binary,
    };

    // convenient definition for ColumnType::Uint.
    using UIntID = std::integral_constant<ColumnType, ColumnType::UInt>;
    // convenient definition for ColumnType::VarChar.
    using VarCharID = std::integral_constant<ColumnType, ColumnType::VarChar>;
    // convenient definition for the range of scalar types.
    //
    // Which, ranges from 0 up to this number.
    constexpr auto FinalScalarType =
        static_cast<std::underlying_type_t<ColumnType>>(ColumnType::VarChar) -
        1;

    // convenient definition for the range of short variable types.
    // These are the variable types whose size can be at most 255 bytes.
    //
    // Which, ranges from `FinalScalarType` up to this number.
    constexpr auto FinalShortVariableType =
        static_cast<std::underlying_type_t<ColumnType>>(ColumnType::VarChar);

    /**
     * @brief Whether `coltype` is a scalar type.
     *
     * @tparam T
     * @param coltype
     */
    constexpr auto is_scalar(ColumnType coltype) -> bool;

    /**
     * @brief Whether `coltype` is a short variable type.
     *
     * @tparam T
     * @param coltype
     */
    constexpr auto is_short_variable(ColumnType coltype) -> bool;

    /**
     * @brief Whether `coltype` is a long variable type.
     *
     * @tparam T
     * @param coltype
     */
    constexpr auto is_long_variable(ColumnType coltype) -> bool;

    /**
     * @brief Definition of bitmaps that a ColumnMeta holds.
     */
    enum class ColumnFlags : std::uint8_t {
        // It's really "nothing special"
        NothingSpecial = 0,
        // Good old primary key. Not nullable, of course.
        // A very crude method is used: compare byte-by-byte the data of a
        // primary key.
        PrimaryKey = 1 << 0,
        // I doubt I'm gonna implement this any time soon.
        // Holds the primary key of another table.
        ForeignKey = 1 << 1,
        // Uses an extra byte at the front to mark whether we're dealing with a
        // null value.
        Nullable = 1 << 2,
        // There will be a dedicated hashtable to check for uniqueness of this
        // entry.
        UniqueValue = 1 << 3,
    };

    /**
     * @brief Chaining bit flags together.
     */
    constexpr auto operator|(ColumnFlags lhs, decltype(lhs) rhs)
        ->std::underlying_type_t<ColumnFlags>;

    /**
     * @brief Chaining bit flags together.
     */
    constexpr auto operator|(std::underlying_type_t<ColumnFlags> lhs,
                             ColumnFlags rhs)
        ->decltype(lhs);

    /**
     * @brief Returns true if the `val` bitmap contains all flags as specified
     * in `filter`.
     */
    constexpr auto contains(std::underlying_type_t<ColumnFlags> val,
                            decltype(val) filter) -> bool;

    /**
     * @class ColumnMeta
     * @brief Represents a column inside the database.
     *
     */
    class ColumnMeta {
      public:
        /**
         * @brief Constructor called when the `ColumnType T` is a scalar type.
         *
         * Such a type is `UInt`.
         *
         * @tparam `Fl` representation of `ColumnFlags`. Can be `ColumnFlags`
         * itself, or the underlying type of `ColumnFlags`.
         * @param `name` the name of the column.
         * @param `flags` can be one `ColumnFlags` or a bitmap.
         * @param `id` the ID of the column. In any given table, a column must
         * have an unique ID. ID = 0 delegates the ID resolution to the `Table`
         * object when this column is added into one.
         * @return A newly constructed `ColumnMeta`
         */
        template <ColumnType T, typename Fl>
            requires(is_scalar(T) &&
                     std::is_same_v<std::underlying_type_t<Fl>,
                                    std::underlying_type_t<ColumnFlags>>)
        constexpr ColumnMeta(std::string_view name, Fl flags = 0,
                             std::uint32_t id = 0);

        /**
         * @brief Constructor called when `ColumnType T` can be longer than a
         * short variable-length type (aka, its size can be bigger than 255).
         *
         * Such a type is `Text`.
         *
         * WIP: maybe I will store these large stuff as separate files. Maybe
         * the implementation will be kinda like how `git` stores stuff: abuse
         * the machine's filesystem.
         *
         * @tparam `Fl` representation of `ColumnFlags`. Can be `ColumnFlags`
         * itself, or the underlying type of `ColumnFlags`.
         * @param `name` the name of the column.
         * @param `size` the size of this column.
         * @param `flags` can be one `ColumnFlags` or a bitmap.
         * @param `id` the ID of the column. In any given table, a column must
         * have an unique ID. ID = 0 delegates the ID resolution to the `Table`
         * object when this column is added into one.
         * @return A newly constructed `ColumnMeta`
         */
        template <ColumnType T, typename Fl>
            requires(is_long_variable(T) &&
                     std::is_same_v<std::underlying_type_t<Fl>,
                                    std::underlying_type_t<ColumnFlags>>)
        constexpr ColumnMeta(std::string_view name, std::uint16_t size,
                             Fl flags = 0, std::uint32_t id = 0);

        /**
         * @brief Constructor called when `ColumnType T` is a short
         * variable-length type (aka, its size can go up to 255).
         *
         * Such a type is `VarChar`.
         *
         * @tparam `Fl` representation of `ColumnFlags`. Can be `ColumnFlags`
         * itself, or the underlying type of `ColumnFlags`.
         * @param `name` the name of the column.
         * @param `name` the name of the column.
         * @param `size` the size of this column.
         * @param `flags` can be one `ColumnFlags` or a bitmap.
         * @param `id` the ID of the column. In any given table, a column must
         * have an unique ID. ID = 0 delegates the ID resolution to the `Table`
         * object when this column is added into one.
         * @return A newly constructed `ColumnMeta`
         */
        template <ColumnType T, typename Fl>
            requires(is_short_variable(T) &&
                     std::is_same_v<std::underlying_type_t<Fl>,
                                    std::underlying_type_t<ColumnFlags>>)
        constexpr ColumnMeta(std::string_view name, std::uint16_t size,
                             Fl flags = 0, std::uint32_t id = 0);

        /**
         * @return Name of this column.
         */
        [[nodiscard]] constexpr auto
        get_name() const noexcept -> std::string_view {
            return this->m_name;
        }

        /**
         * @return ID of this column.
         */
        [[nodiscard]] constexpr auto
        get_col_id() const noexcept -> std::uint32_t {
            return this->m_col_id;
        }

        /**
         * @return size of the datum hold by this column.
         */
        [[nodiscard]] constexpr auto
        get_col_size() const noexcept -> std::uint16_t {
            return this->m_col_size;
        }

        /**
         * @return type of the datum hold by this column.
         */
        [[nodiscard]] constexpr auto get_type() const noexcept -> ColumnType {
            return this->m_type;
        }

        /**
         * @return bitmap flags of this column.
         */
        [[nodiscard]] constexpr auto get_column_flags() const noexcept
            -> std::underlying_type_t<ColumnFlags> {
            return this->m_flags;
        }

      private:
        std::string m_name;
        std::uint32_t m_col_id;
        std::uint16_t m_col_size;
        ColumnType m_type;
        std::underlying_type_t<ColumnFlags> m_flags;

        friend class Table;
    };

    /*
     * DEFINITIONS
     */

    constexpr auto is_scalar(ColumnType coltype) -> bool {
        return static_cast<std::underlying_type_t<ColumnType>>(coltype) <=
               FinalScalarType;
    }

    constexpr auto is_short_variable(ColumnType coltype) -> bool {
        return !is_scalar(coltype) &&
               static_cast<std::underlying_type_t<ColumnType>>(coltype) <=
                   FinalShortVariableType;
    }

    constexpr auto is_long_variable(ColumnType coltype) -> bool {
        return !is_scalar(coltype) && !is_short_variable(coltype);
    }

    constexpr auto operator|(ColumnFlags lhs, decltype(lhs) rhs)
        ->std::underlying_type_t<ColumnFlags> {
        return static_cast<std::underlying_type_t<decltype(lhs)>>(lhs) |
               static_cast<std::underlying_type_t<decltype(rhs)>>(rhs);
    }

    constexpr auto operator|(std::underlying_type_t<ColumnFlags> lhs,
                             ColumnFlags rhs)
        ->decltype(lhs) {
        return lhs | static_cast<std::underlying_type_t<decltype(rhs)>>(rhs);
    }

    constexpr auto contains(std::underlying_type_t<ColumnFlags> val,
                            decltype(val) filter) -> bool {
        return (val & filter) == filter;
    }

    template <ColumnType T, typename Fl>
        requires(is_scalar(T) &&
                 std::is_same_v<std::underlying_type_t<Fl>,
                                std::underlying_type_t<ColumnFlags>>)
    constexpr ColumnMeta::ColumnMeta(std::string_view name, Fl flags,
                                     std::uint32_t id)
        : m_name(std::move(name)), m_col_id(id), m_type(T), m_flags(flags) {
        using enum ColumnType;
        switch (T) {
        case UInt:
            this->m_col_size = 4;
            break;
        default:
            break;
        }
    }

    template <ColumnType T, typename Fl>
        requires(is_long_variable(T) &&
                 std::is_same_v<std::underlying_type_t<Fl>,
                                std::underlying_type_t<ColumnFlags>>)
    constexpr ColumnMeta::ColumnMeta(std::string_view name, std::uint16_t size,
                                     Fl flags, std::uint32_t id)
        : m_name(std::move(name)), m_col_id(id), m_col_size(size), m_type(T),
          m_flags(flags) {}

    template <ColumnType T, typename Fl>
        requires(is_short_variable(T) &&
                 std::is_same_v<std::underlying_type_t<Fl>,
                                std::underlying_type_t<ColumnFlags>>)
    constexpr ColumnMeta::ColumnMeta(std::string_view name, std::uint16_t size,
                                     Fl flags, std::uint32_t id)
        : m_name(std::move(name)), m_col_id(id), m_col_size(size), m_type(T),
          m_flags(flags) {}
}

#endif // !SMOLDB_CPP_COLUMN_META_HXX
