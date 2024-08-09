module;

#include <string_view>
#include <cstdint>
#include <expected>
// Note, I don't expect the command to ever be too large that vector reallocation is an issue
#include <type_traits> 
#include <memory>
#include <vector>
#include "debug.hpp"

export module interpreter;

// some basic knowledge check for me
static_assert(std::is_trivially_copyable_v<std::string_view>);

/*
 * Language specs:
 *
 * All statements end with a semicolon.
 *
 * Meta commands preceded by a pound (#).
 * eg, #exit;
 *
 * If-scope:
 * if (condition);
 * ...
 * else;
 * ...
 * endif;
 *
 * set identifier: {...}
 * eg, create {...};
 *
 * */

namespace smoldb {

export enum class InterpretError : uint8_t {
    InvalidMetaCmd = 1, 
    UnterminatedCmd = 2,
};

export enum class TokenType : uint8_t {
    // meta commands
    Exit = 0,
    // keyword
    Create,
    Insert,
    Delete,
    If,
    Endif,
    Else,
    // literal
    String,
    Number,
    Identifier,
    // special characters
    Pound,
    Semicolon,
    LeftRoundParen,
    RightRoundParen,
    LeftCurlyParen,
    RightCurlyParen,
};

export class interpreter {
    public:
        explicit interpreter(std::string_view strv);
        /**
         * @brief Interpret the string contained by the interpreter
         */
        auto operator()() -> std::expected<void, InterpretError>;
    private:
        class impl;
        const std::unique_ptr<impl> pImpl;
};

EXPORT_DEBUG struct token {
    std::string_view lexeme;
    TokenType type;
    explicit token(std::string_view lex, TokenType typ)
        : lexeme(lex), type(typ) {}
};

/**
 * @class scanner
 * @brief A functor responsible for breaking down the input string into tokens.
 *
 */
EXPORT_DEBUG class scanner {
  public:
    explicit scanner(std::string_view strv) : m_strv(strv) {}
    /**
     * @brief Scan the string_view to see what tokens it's holding
     */
    void operator()();

  private:
    std::vector<token> m_tokens;
    std::string_view m_strv;
    void scan_next();
    auto match_at(char match, std::size_t pos) -> bool {
        return m_strv.length() >= 2 && m_strv.at(pos) == match;
    }
};

} // namespace smoldb
