#ifndef PUNCTHANDLER_HPP_
#define PUNCTHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

struct PunctHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // PUNCTHANDLER_HPP_
