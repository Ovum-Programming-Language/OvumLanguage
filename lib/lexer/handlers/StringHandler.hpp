#ifndef STRINGHANDLER_H
#define STRINGHANDLER_H

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

struct StringHandler : Handler {
  OptToken scan(Lexer &lx) override;
};

#endif // STRINGHANDLER_H
