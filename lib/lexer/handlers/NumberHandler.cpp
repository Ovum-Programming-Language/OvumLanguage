#include "NumberHandler.hpp"

static bool is_digit_char(char c) {
  return std::isdigit(static_cast<unsigned char>(c));
}

OptToken NumberHandler::scan(Lexer &lx) {
  std::string raw;
  char first = lx.current_char();
  if (first == '.') {
    raw.push_back('.');
    lx.consume_while(raw, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
    if (lx.peek() == 'e' || lx.peek() == 'E') {
      raw.push_back(lx.advance());
      if (lx.peek() == '+' || lx.peek() == '-')
        raw.push_back(lx.advance());
      if (!is_digit_char(lx.peek()))
        throw LexerError("Malformed exponent");
      lx.consume_while(raw, [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); });
    }
    long double v;
    try {
      v = std::stold(raw);
    } catch (...) {
      throw LexerError(std::string("Malformed float literal: ") + raw);
    }
    return std::make_optional(TokenFactory::make_float_literal(std::move(raw), v, lx.line(), lx.token_col()));
  } else {
    lx.retreat_one();
    lx.consume_while(raw, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
    if (lx.peek() == '.' && std::isdigit(static_cast<unsigned char>(lx.peek(1)))) {
      raw.push_back(lx.advance()); // dot
      lx.consume_while(raw, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
      if (lx.peek() == 'e' || lx.peek() == 'E') {
        raw.push_back(lx.advance());
        if (lx.peek() == '+' || lx.peek() == '-')
          raw.push_back(lx.advance());
        if (!is_digit_char(lx.peek()))
          throw LexerError("Malformed exponent");
        lx.consume_while(raw, [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); });
      }
      long double v;
      try {
        v = std::stold(raw);
      } catch (...) {
        throw LexerError(std::string("Malformed float literal: ") + raw);
      }
      return std::make_optional(TokenFactory::make_float_literal(std::move(raw), v, lx.line(), lx.token_col()));
    }
    if (lx.peek() == 'e' || lx.peek() == 'E') {
      raw.push_back(lx.advance());
      if (lx.peek() == '+' || lx.peek() == '-')
        raw.push_back(lx.advance());
      if (!is_digit_char(lx.peek()))
        throw LexerError("Malformed exponent");
      lx.consume_while(raw, [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); });
      long double v;
      try {
        v = std::stold(raw);
      } catch (...) {
        throw LexerError(std::string("Malformed float literal: ") + raw);
      }
      return std::make_optional(TokenFactory::make_float_literal(std::move(raw), v, lx.line(), lx.token_col()));
    }
    long long vi;
    try {
      vi = std::stoll(raw);
    } catch (...) {
      throw LexerError(std::string("Malformed integer literal: ") + raw);
    }
    return std::make_optional(TokenFactory::make_int_literal(std::move(raw), vi, lx.line(), lx.token_col()));
  }
}
