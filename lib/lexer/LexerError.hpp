#ifndef LEXERERROR_HPP_
#define LEXERERROR_HPP_

#include <stdexcept>

class LexerError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

#endif // LEXERERROR_HPP_
