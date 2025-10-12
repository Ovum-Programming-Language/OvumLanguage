#include "NewlineHandler.hpp"

OptToken NewlineHandler::scan(Lexer &lx) {
  return std::make_optional(TokenFactory::make_newline(lx.line(), lx.token_col()));
}
