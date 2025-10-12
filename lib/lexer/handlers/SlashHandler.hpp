#ifndef SLASHHANDLER_HPP_
#define SLASHHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

struct SlashHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // SLASHHANDLER_HPP_
