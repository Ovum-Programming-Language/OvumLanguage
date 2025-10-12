#include "OperatorHandler.hpp"

OptToken OperatorHandler::scan(Lexer &lx) {
  std::string op;
  op.push_back(lx.current_char());
  char p = lx.peek();
  if (p != '\0') {
    std::string two = op + p;
    if (lx.is_multiop(two)) {
      lx.advance();
      op = two;
    }
  }
  return std::make_optional(TokenFactory::make_operator(std::move(op), lx.line(), lx.token_col()));
}
