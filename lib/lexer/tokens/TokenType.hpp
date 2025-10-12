#ifndef TOKENTYPE_HPP_
#define TOKENTYPE_HPP_

#include <string_view>

enum class TokenType { IDENT, KEYWORD, INT, FLOAT, STRING, CHAR, BOOL, OPERATOR, PUNCT, NEWLINE, COMMENT, EOF_T };

inline std::string_view to_string_view(TokenType token_type) {
  switch (token_type) {
    case TokenType::IDENT:
      return "IDENT";
    case TokenType::KEYWORD:
      return "KEYWORD";
    case TokenType::INT:
      return "INT";
    case TokenType::FLOAT:
      return "FLOAT";
    case TokenType::STRING:
      return "STRING";
    case TokenType::CHAR:
      return "CHAR";
    case TokenType::BOOL:
      return "BOOL";
    case TokenType::OPERATOR:
      return "OPERATOR";
    case TokenType::PUNCT:
      return "PUNCT";
    case TokenType::NEWLINE:
      return "NEWLINE";
    case TokenType::COMMENT:
      return "COMMENT";
    case TokenType::EOF_T:
      return "EOF";
    default:
      return "UNKNOWN";
  }
}

#endif // TOKENTYPE_HPP_
