#ifndef COMMENTTOKEN_HPP_
#define COMMENTTOKEN_HPP_

#include <iosfwd>
#include <memory>
#include <sstream>
#include <string>

#include "Token.hpp"
#include "TokenType.hpp"
#include "TokenVisitor.hpp"

class CommentToken final : public Token {
public:
  CommentToken(std::string txt, int line, int col) : Token(line, col), text_(std::move(txt)) {
  }

  TokenType type() const noexcept override {
    return TokenType::COMMENT;
  }
  std::string lexeme() const noexcept override {
    return text_;
  }

  std::unique_ptr<Token> clone() const override {
    return std::make_unique<CommentToken>(*this);
  }
  void accept(TokenVisitor& visitor) const override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    std::ostringstream os;
    os << "Token(COMMENT, '" << text_ << "', @" << this->line() << ":" << this->column() << ")";
    return os.str();
  }

private:
  std::string text_;
};

#endif // COMMENTTOKEN_HPP_
