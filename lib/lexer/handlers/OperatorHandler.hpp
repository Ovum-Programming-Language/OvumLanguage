#ifndef OPERATORHANDLER_HPP_
#define OPERATORHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

struct OperatorHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // OPERATORHANDLER_HPP_
