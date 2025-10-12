#ifndef IDENTTOKEN_HPP_
#define IDENTTOKEN_HPP_

#include <memory>
#include <sstream>
#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"

class IdentToken final : public Token {
public:
  IdentToken(std::string lex, int line, int col) : Token(line, col), lexeme_(std::move(lex)) {
  }

  TokenType type() const noexcept override {
    return TokenType::IDENT;
  }
  std::string lexeme() const noexcept override {
    return lexeme_;
  }

  std::unique_ptr<Token> clone() const override {
    return std::make_unique<IdentToken>(*this);
  }

  void accept(TokenVisitor& visitor) const override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    std::ostringstream os;
    os << "Token(IDENT, '" << lexeme_ << "', @";
    os << this->line() << ":" << this->column() << ")";
    return os.str();
  }

  const std::string& name() const noexcept {
    return lexeme_;
  }

private:
  std::string lexeme_;
};

#endif // IDENTTOKEN_HPP_
