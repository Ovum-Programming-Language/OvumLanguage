#ifndef LITERALTOKEN_HPP_
#define LITERALTOKEN_HPP_

#include <sstream>
#include <string>

#include "Token.hpp"
#include "TokenType.hpp"
#include "lib/lexer/values/Value.hpp"

class LiteralToken final : public Token {
public:
  LiteralToken(TokenType typ, std::string rawLexeme, std::unique_ptr<Value> value, int line, int col) :
      Token(line, col), typ_(typ), lexeme_(std::move(rawLexeme)), value_(std::move(value)) {
  }

  TokenType type() const noexcept override {
    return typ_;
  }
  std::string lexeme() const noexcept override {
    return lexeme_;
  }

  const Value* value() const noexcept {
    return value_.get();
  }
  Value* value() noexcept {
    return value_.get();
  }

  std::unique_ptr<Token> clone() const override {
    std::unique_ptr<Value> vcopy = value_ ? value_->clone() : nullptr;
    return std::make_unique<LiteralToken>(typ_, lexeme_, std::move(vcopy), this->line(), this->column());
  }

  void accept(TokenVisitor& visitor) const override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    std::ostringstream os;
    os << "Token(" << to_string_view(typ_) << ", '" << lexeme_ << "'";
    if (value_)
      os << ", " << value_->to_string();
    os << ", @" << this->line() << ":" << this->column() << ")";
    return os.str();
  }

private:
  TokenType typ_;
  std::string lexeme_;
  std::unique_ptr<Value> value_;
};

#endif // LITERALTOKEN_HPP_
