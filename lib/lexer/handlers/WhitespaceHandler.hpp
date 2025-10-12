#ifndef WHITESPACEHANDLER_HPP_
#define WHITESPACEHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

struct WhitespaceHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // WHITESPACEHANDLER_HPP_
