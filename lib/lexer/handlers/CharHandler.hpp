#ifndef CHARHANDLER_HPP_
#define CHARHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"
#include "lib/lexer/LexerError.hpp"

struct CharHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // CHARHANDLER_HPP_
