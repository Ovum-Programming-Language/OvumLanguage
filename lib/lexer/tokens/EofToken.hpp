#ifndef EOFTOKEN_HPP_
#define EOFTOKEN_HPP_

#include <iosfwd>
#include <memory>
#include <sstream>
#include <string>

#include "Token.hpp"
#include "TokenType.hpp"
#include "TokenVisitor.hpp"

class EofToken final : public Token {
public:
  EofToken(int line, int col) : Token(line, col), lexeme_("") {
  }

  TokenType type() const noexcept override {
    return TokenType::EOF_T;
  }
  std::string lexeme() const noexcept override {
    return lexeme_;
  }

  std::unique_ptr<Token> clone() const override {
    return std::make_unique<EofToken>(*this);
  }
  void accept(TokenVisitor& visitor) const override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    std::ostringstream os;
    os << "Token(EOF, @" << this->line() << ":" << this->column() << ")";
    return os.str();
  }

private:
  std::string lexeme_;
};

#endif // EOFTOKEN_HPP_
