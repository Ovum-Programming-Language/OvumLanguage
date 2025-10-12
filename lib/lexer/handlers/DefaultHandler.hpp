#ifndef DEFAULTHANDLER_HPP_
#define DEFAULTHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

struct DefaultHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // DEFAULTHANDLER_HPP_
