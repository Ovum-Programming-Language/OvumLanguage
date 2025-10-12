#ifndef NUMBERHANDLER_HPP_
#define NUMBERHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

struct NumberHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // NUMBERHANDLER_HPP_
