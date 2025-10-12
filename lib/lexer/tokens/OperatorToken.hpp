#ifndef OPERATORTOKEN_HPP_
#define OPERATORTOKEN_HPP_

#include <memory>
#include <sstream>
#include <string>

#include "Token.hpp"
#include "TokenType.hpp"

struct TokenVisitor;
class OperatorToken final : public Token {
public:
  OperatorToken(std::string op, int line, int col) : Token(line, col), lexeme_(std::move(op)) {
  }

  TokenType type() const noexcept override {
    return TokenType::OPERATOR;
  }
  std::string lexeme() const noexcept override {
    return lexeme_;
  }

  std::unique_ptr<Token> clone() const override {
    return std::make_unique<OperatorToken>(*this);
  }
  void accept(TokenVisitor& visitor) const override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    std::ostringstream os;
    os << "Token(OPERATOR, '" << lexeme_ << "', @";
    os << this->line() << ":" << this->column() << ")";
    return os.str();
  }

private:
  std::string lexeme_;
};

#endif // OPERATORTOKEN_HPP_
