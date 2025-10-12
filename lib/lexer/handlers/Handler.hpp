#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <memory>
#include <optional>

class Token;
class Lexer;

using TokenPtr = std::unique_ptr<Token>;
using OptToken = std::optional<TokenPtr>;

struct Handler {
  virtual ~Handler() = default;
  virtual OptToken scan(Lexer &lx) = 0;
};

#endif // HANDLER_HPP_
