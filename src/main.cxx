#include <print>
import interpreter;

auto main() noexcept -> int { // NOLINT
    // placeholder, just to prove a point that the module thingy works
    std::println("{}", static_cast<std::underlying_type_t<smoldb::TokenType>>(
                           smoldb::TokenType::Else));
    return 0;
}
