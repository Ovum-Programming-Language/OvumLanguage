#include "StringHandler.hpp"

OptToken StringHandler::scan(Lexer &lx) {
  std::string raw;
  std::string out;
  raw.push_back('"');
  while (!lx.is_at_end()) {
    char c = lx.advance();
    raw.push_back(c);
    if (c == '"')
      break;
    if (c == '\\') {
      char e = lx.advance();
      raw.push_back(e);
      switch (e) {
        case 'n':
          out.push_back('\n');
          break;
        case 't':
          out.push_back('\t');
          break;
        case 'r':
          out.push_back('\r');
          break;
        case '\\':
          out.push_back('\\');
          break;
        case '"':
          out.push_back('"');
          break;
        default:
          out.push_back(e);
          break;
      }
    } else {
      if (c == '\n')
        throw LexerError("Unterminated string literal (newline inside)");
      out.push_back(c);
    }
  }
  return std::make_optional(
      TokenFactory::make_string_literal(std::move(raw), std::move(out), lx.line(), lx.token_col()));
}
