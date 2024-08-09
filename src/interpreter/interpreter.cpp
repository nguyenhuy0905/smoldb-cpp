module;

#include <cassert>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

module interpreter;

namespace smoldb {


} // namespace smoldb

using TokenType = smoldb::TokenType;

namespace {

/**
 * @brief Trim leading space, tab, newline from strv.
 *
 * @param strv
 * @return the trimmed string_view. Empty string if either strv is
 * empty or strv only contains space, tab or newline.
 */
auto trim_leading(std::string_view strv) -> std::string_view;

// auto get_keyword_map()
//     -> const std::unordered_map<std::string_view, TokenType>&;
//
// auto get_metacmd_map()
//     -> const std::unordered_map<std::string_view, TokenType>&;

} // namespace

//
// IMPLEMENTATION
//

namespace {

auto trim_leading(std::string_view strv) -> std::string_view {
    if (strv.empty()) {
        return strv;
    }
    std::size_t curr_pos = 0;
    while (curr_pos < strv.length()) {
        switch (strv.at(curr_pos)) {
        case '\n':
        case '\t':
        case ' ':
            ++curr_pos;
        default:
            return strv.substr(curr_pos);
        }
    }
    // curr_pos == strv.length()
    return "";
}

// inline auto
// get_keyword_map() -> const std::unordered_map<std::string_view, TokenType>& {
//     static const std::unordered_map<std::string_view, smoldb::TokenType>
//         keyword_map = {
//             {"create", TokenType::Create}, {"insert", TokenType::Insert},
//             {"delete", TokenType::Delete}, {"if", TokenType::If},
//             {"else", TokenType::Else},
//         };
//
//     return keyword_map;
// }
//
// inline auto
// get_metacmd_map() -> const std::unordered_map<std::string_view, TokenType>& {
//     static const std::unordered_map<std::string_view, TokenType> metacmd_map = {
//         {"exit", TokenType::Exit},
//     };
//     return metacmd_map;
// }

} // namespace

namespace smoldb {

class interpreter::impl {
  public:
    explicit impl(std::string_view strv) : m_scan(strv) {}

  private:
    scanner m_scan;
};


interpreter::interpreter(std::string_view strv)
    : pImpl(std::make_unique<impl>(strv)) {}

void scanner::operator()() {
    while (!m_strv.empty()) {
        scan_next();
    }
}

void scanner::scan_next() {
    m_strv = trim_leading(m_strv);
    if (m_strv.empty()) {
        return;
    }

    const char curr = m_strv.at(0);

    switch (curr) {
    case '#':
        m_tokens.emplace_back("#", TokenType::Pound);
        m_strv = m_strv.substr(1);
        return;
    case ';':
        m_tokens.emplace_back(";", TokenType::Semicolon);
        m_strv = m_strv.substr(1);
        return;
    case '(':
        m_tokens.emplace_back("(", TokenType::LeftRoundParen);
        m_strv = m_strv.substr(1);
        return;
    case ')':
        m_tokens.emplace_back(")", TokenType::RightRoundParen);
        m_strv = m_strv.substr(1);
        return;
    case '{':
        m_tokens.emplace_back("{", TokenType::LeftCurlyParen);
        m_strv = m_strv.substr(1);
        return;
    case '}':
        m_tokens.emplace_back("}", TokenType::RightCurlyParen);
        m_strv = m_strv.substr(1);
        return;
    default:
        break;
    }

}

} // namespace smoldb
