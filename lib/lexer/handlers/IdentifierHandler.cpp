#include "IdentifierHandler.hpp"

OptToken IdentifierHandler::scan(Lexer &lx) {
  std::string s;
  s.push_back(lx.current_char());
  lx.consume_while(s, [](char ch) { return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_'; });

  if (lx.is_keyword(s)) {
    if (s == "true" || s == "false") {
      return std::make_optional(TokenFactory::make_bool_literal(s, s == "true", lx.line(), lx.token_col()));
    }
    return std::make_optional(TokenFactory::make_keyword(std::move(s), lx.line(), lx.token_col()));
  }
  if (s == "xor") {
    return std::make_optional(TokenFactory::make_operator(std::move(s), lx.line(), lx.token_col()));
  }
  return std::make_optional(TokenFactory::make_ident(std::move(s), lx.line(), lx.token_col()));
}
