#include "SlashHandler.hpp"

OptToken SlashHandler::scan(Lexer &lx) {
  if (lx.peek() == '/') {
    std::string txt;
    while (!lx.is_at_end() && lx.peek() != '\n')
      txt.push_back(lx.advance());
    if (lx.keep_comments())
      return std::make_optional(TokenFactory::make_comment(std::move(txt), lx.line(), lx.token_col()));
    return std::nullopt;
  } else if (lx.peek() == '*') {
    lx.advance();
    std::string txt;
    bool closed = false;
    while (!lx.is_at_end()) {
      char c = lx.advance();
      if (c == '*' && lx.peek() == '/') {
        lx.advance();
        closed = true;
        break;
      }
      txt.push_back(c);
    }
    if (!closed)
      throw LexerError("Unterminated block comment");
    if (lx.keep_comments())
      return std::make_optional(TokenFactory::make_comment(std::move(txt), lx.line(), lx.token_col()));
    return std::nullopt;
  } else {
    return std::make_optional(TokenFactory::make_operator(std::string(1, '/'), lx.line(), lx.token_col()));
  }
}
