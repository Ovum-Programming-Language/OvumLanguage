#include "PunctHandler.hpp"

OptToken PunctHandler::scan(Lexer &lx) {
  char c = lx.current_char();
  return std::make_optional(TokenFactory::make_punct(c, lx.line(), lx.token_col()));
}
