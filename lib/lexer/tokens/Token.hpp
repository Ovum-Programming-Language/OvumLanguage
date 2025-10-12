#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <memory>
#include <sstream>
#include <string>

#include "TokenType.hpp"
#include "TokenVisitor.hpp"

class Token {
public:
  Token(int line, int column) : line_(line), column_(column) {
  }
  virtual ~Token() = default;
  virtual TokenType type() const = 0;
  virtual std::string lexeme() const = 0;
  virtual std::unique_ptr<Token> clone() const = 0;
  virtual void accept(TokenVisitor& v) const = 0;
  virtual std::string to_string() const {
    std::ostringstream os;
    os << "Token(" << to_string_view(type()) << ", '" << lexeme() << "', @" << line_ << ":" << column_ << ")";
    return os.str();
  }
  int line() const noexcept {
    return line_;
  }
  int column() const noexcept {
    return column_;
  }

private:
  int line_;
  int column_;
};

#endif // TOKEN_HPP_
