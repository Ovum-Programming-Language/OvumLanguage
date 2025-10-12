#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <cctype>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "handlers/CharHandler.hpp"
#include "handlers/DefaultHandler.hpp"
#include "handlers/Handler.hpp"
#include "handlers/IdentifierHandler.hpp"
#include "handlers/NewlineHandler.hpp"
#include "handlers/NumberHandler.hpp"
#include "handlers/OperatorHandler.hpp"
#include "handlers/PunctHandler.hpp"
#include "handlers/SlashHandler.hpp"
#include "handlers/StringHandler.hpp"
#include "handlers/WhitespaceHandler.hpp"

#include "tokens/TokenFactory.hpp"

class Lexer {
public:
  explicit Lexer(std::string_view src, bool keep_comments = false);

  std::vector<TokenPtr> tokenize();

  bool is_at_end() const noexcept;
  char peek(size_t offset = 0) const noexcept;
  char current_char() const noexcept;
  char advance();
  void retreat_one();
  void consume_while(std::string &out, const std::function<bool(char)> &pred);
  std::string raw_lexeme() const;

  int line() const noexcept {
    return line_;
  }
  int token_col() const noexcept {
    return token_col_;
  }
  bool keep_comments() const noexcept {
    return keep_comments_;
  }
  bool is_keyword(std::string_view s) const;
  bool is_multiop(std::string_view s) const;

  void set_handler(unsigned char c, std::unique_ptr<Handler> handler) {
    handlers_[c] = std::move(handler);
  }
  void set_default_handler(std::unique_ptr<Handler> handler) {
    default_handler_ = std::move(handler);
  }

private:
  void register_defaults();

  std::string_view src_;
  bool keep_comments_;

  size_t start_{0};
  size_t current_{0};
  int line_{1};
  int col_{1};
  int token_col_{1};

  std::array<std::unique_ptr<Handler>, 256> handlers_;
  std::unique_ptr<Handler> default_handler_;
};

#endif // LEXER_HPP_
