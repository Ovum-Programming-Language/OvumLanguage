#include "WhitespaceHandler.hpp"

OptToken WhitespaceHandler::scan(Lexer &lx) {
  while (!lx.is_at_end()) {
    if (char p = lx.peek(); p == ' ' || p == '\t' || p == '\r') {
      lx.advance();
      continue;
    }
    break;
  }
  return std::nullopt;
}
