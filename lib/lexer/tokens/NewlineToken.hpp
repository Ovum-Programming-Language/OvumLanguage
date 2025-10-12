#ifndef NEWLINETOKEN_HPP_
#define NEWLINETOKEN_HPP_

#include <memory>
#include <sstream>
#include <string>

#include "Token.hpp"
#include "TokenType.hpp"
#include "TokenVisitor.hpp"

class NewlineToken final : public Token {
public:
  NewlineToken(int line, int col) : Token(line, col), lexeme_("\\n") {
  }

  TokenType type() const noexcept override {
    return TokenType::NEWLINE;
  }
  std::string lexeme() const noexcept override {
    return lexeme_;
  }

  std::unique_ptr<Token> clone() const override {
    return std::make_unique<NewlineToken>(*this);
  }
  void accept(TokenVisitor& visitor) const override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    std::ostringstream os;
    os << "Token(NEWLINE, '\\n', @" << this->line() << ":" << this->column() << ")";
    return os.str();
  }

private:
  std::string lexeme_;
};

#endif // NEWLINETOKEN_HPP_
