#ifndef KEYWORDTOKEN_HPP_
#define KEYWORDTOKEN_HPP_

#include <memory>
#include <sstream>
#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"

class KeywordToken final : public Token {
public:
  KeywordToken(std::string lex, int line, int col) : Token(line, col), lexeme_(std::move(lex)) {
  }

  TokenType type() const noexcept override {
    return TokenType::KEYWORD;
  }
  std::string lexeme() const noexcept override {
    return lexeme_;
  }

  std::unique_ptr<Token> clone() const override {
    return std::make_unique<KeywordToken>(*this);
  }
  void accept(TokenVisitor& visitor) const override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    std::ostringstream os;
    os << "Token(KEYWORD, '" << lexeme_ << "', @";
    os << this->line() << ":" << this->column() << ")";
    return os.str();
  }

private:
  std::string lexeme_;
};

#endif // KEYWORDTOKEN_HPP_
