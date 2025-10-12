#include "DefaultHandler.hpp"

OptToken DefaultHandler::scan(Lexer &lx) {
  char c = lx.current_char();
  throw LexerError(std::string("Unexpected character: ") + c);
}
