#ifndef TOKENFACTORY_HPP_
#define TOKENFACTORY_HPP_

#include <cstdint>
#include <memory>
#include <string>

#include "CommentToken.hpp"
#include "EofToken.hpp"
#include "IdentToken.hpp"
#include "KeywordToken.hpp"
#include "LiteralToken.hpp"
#include "NewlineToken.hpp"
#include "OperatorToken.hpp"
#include "PunctToken.hpp"
#include "Token.hpp"
#include "lib/lexer/values/BoolValue.hpp"
#include "lib/lexer/values/CharValue.hpp"
#include "lib/lexer/values/FloatValue.hpp"
#include "lib/lexer/values/IntValue.hpp"
#include "lib/lexer/values/StringValue.hpp"

struct TokenFactory {
  static std::unique_ptr<Token> make_ident(std::string lex, int line, int col) {
    return std::make_unique<IdentToken>(std::move(lex), line, col);
  }
  static std::unique_ptr<Token> make_keyword(std::string lex, int line, int col) {
    return std::make_unique<KeywordToken>(std::move(lex), line, col);
  }
  static std::unique_ptr<Token> make_operator(std::string lex, int line, int col) {
    return std::make_unique<OperatorToken>(std::move(lex), line, col);
  }
  static std::unique_ptr<Token> make_punct(char ch, int line, int col) {
    return std::make_unique<PunctToken>(ch, line, col);
  }
  static std::unique_ptr<Token> make_punct(std::string lex, int line, int col) {
    return std::make_unique<PunctToken>(std::move(lex), line, col);
  }
  static std::unique_ptr<Token> make_newline(int line, int col) {
    return std::make_unique<NewlineToken>(line, col);
  }
  static std::unique_ptr<Token> make_comment(std::string text, int line, int col) {
    return std::make_unique<CommentToken>(std::move(text), line, col);
  }
  static std::unique_ptr<Token> make_eof(int line, int col) {
    return std::make_unique<EofToken>(line, col);
  }

  static std::unique_ptr<Token> make_int_literal(std::string raw, int64_t v, int line, int col) {
    return std::make_unique<LiteralToken>(TokenType::INT, std::move(raw), std::make_unique<IntValue>(v), line, col);
  }
  static std::unique_ptr<Token> make_float_literal(std::string raw, long double v, int line, int col) {
    return std::make_unique<LiteralToken>(TokenType::FLOAT, std::move(raw), std::make_unique<FloatValue>(v), line, col);
  }
  static std::unique_ptr<Token> make_string_literal(std::string raw, std::string s, int line, int col) {
    return std::make_unique<LiteralToken>(
        TokenType::STRING, std::move(raw), std::make_unique<StringValue>(std::move(s)), line, col);
  }
  static std::unique_ptr<Token> make_char_literal(std::string raw, char c, int line, int col) {
    return std::make_unique<LiteralToken>(TokenType::CHAR, std::move(raw), std::make_unique<CharValue>(c), line, col);
  }
  static std::unique_ptr<Token> make_bool_literal(std::string raw, bool b, int line, int col) {
    return std::make_unique<LiteralToken>(TokenType::BOOL, std::move(raw), std::make_unique<BoolValue>(b), line, col);
  }
};

#endif // TOKENFACTORY_HPP_
