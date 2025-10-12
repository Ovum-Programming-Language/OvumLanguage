#ifndef NEWLINEHANDLER_HPP_
#define NEWLINEHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

struct NewlineHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // NEWLINEHANDLER_HPP_
