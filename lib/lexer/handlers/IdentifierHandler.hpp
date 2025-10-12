#ifndef IDENTIFIERHANDLER_HPP_
#define IDENTIFIERHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

struct IdentifierHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // IDENTIFIERHANDLER_HPP_
