#ifndef PUNCTTOKEN_HPP_
#define PUNCTTOKEN_HPP_
#include <memory>
#include <sstream>
#include <string>

#include "Token.hpp"
#include "TokenType.hpp"
#include "TokenVisitor.hpp"

class PunctToken final : public Token {
public:
  PunctToken(char ch, int line, int col) : Token(line, col), lexeme_(1, ch) {
  }

  PunctToken(std::string punct, int line, int col) : Token(line, col), lexeme_(std::move(punct)) {
  }

  TokenType type() const noexcept override {
    return TokenType::PUNCT;
  }
  std::string lexeme() const noexcept override {
    return lexeme_;
  }

  std::unique_ptr<Token> clone() const override {
    return std::make_unique<PunctToken>(*this);
  }
  void accept(TokenVisitor& visitor) const override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    std::ostringstream os;
    os << "Token(PUNCT, '" << lexeme_ << "', @";
    os << this->line() << ":" << this->column() << ")";
    return os.str();
  }

private:
  std::string lexeme_;
};

#endif // PUNCTTOKEN_HPP_
