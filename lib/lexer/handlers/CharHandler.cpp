#include "CharHandler.hpp"

OptToken CharHandler::scan(Lexer &lx) {
  std::string raw;
  raw.push_back('\'');
  char val = '\0';
  if (lx.peek() == '\\') {
    lx.advance();
    raw.push_back('\\');
    char e = lx.advance();
    raw.push_back(e);
    switch (e) {
      case 'n':
        val = '\n';
        break;
      case 't':
        val = '\t';
        break;
      case '\\':
        val = '\\';
        break;
      case '\'':
        val = '\'';
        break;
      default:
        val = e;
        break;
    }
  } else {
    char c = lx.advance();
    raw.push_back(c);
    val = c;
  }
  if (lx.peek() == '\'') {
    lx.advance();
    raw.push_back('\'');
  } else
    throw LexerError("Unterminated char literal");
  return std::make_optional(TokenFactory::make_char_literal(std::move(raw), val, lx.line(), lx.token_col()));
}
