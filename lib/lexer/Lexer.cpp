#include "Lexer.hpp"
#include "utils.hpp"

Lexer::Lexer(std::string_view src, bool keep_comments) :
    src_(src), keep_comments_(keep_comments), start_(0), current_(0), line_(1), col_(1), token_col_(1) {
  register_defaults();
}

bool Lexer::is_at_end() const noexcept {
  return current_ >= src_.size();
}

char Lexer::peek(size_t offset) const noexcept {
  size_t idx = current_ + offset;
  if (idx >= src_.size())
    return '\0';
  return src_[idx];
}

char Lexer::current_char() const noexcept {
  if (current_ == 0)
    return '\0';
  return src_[current_ - 1];
}

char Lexer::advance() {
  if (is_at_end())
    return '\0';
  char c = src_[current_++];
  if (c == '\n') {
    ++line_;
    col_ = 1;
  } else
    ++col_;
  return c;
}

void Lexer::retreat_one() {
  if (current_ == 0)
    return;
  --current_;
  int l = 1;
  for (size_t i = 0; i < current_; ++i)
    if (src_[i] == '\n')
      ++l;
  line_ = l;
  int col = 1;
  for (size_t i = current_; i > 0; --i) {
    if (src_[i - 1] == '\n') {
      col = static_cast<int>(current_ - i + 1);
      break;
    }
    if (i == 1)
      col = static_cast<int>(current_);
  }
  col_ = col;
}

void Lexer::consume_while(std::string &out, const std::function<bool(char)> &pred) {
  while (!is_at_end() && pred(peek())) {
    out.push_back(advance());
  }
}

std::string Lexer::raw_lexeme() const {
  if (current_ >= start_)
    return std::string(src_.substr(start_, current_ - start_));
  return {};
}

bool Lexer::is_keyword(std::string_view s) const {
  return keyword_set().contains(std::string(s));
}

bool Lexer::is_multiop(std::string_view s) const {
  return multi_ops_set().contains(std::string(s));
}

std::vector<TokenPtr> Lexer::tokenize() {
  std::vector<TokenPtr> tokens;
  while (!is_at_end()) {
    start_ = current_;
    token_col_ = col_;
    char c = advance();
    Handler *h = handlers_[static_cast<unsigned char>(c)].get();
    if (!h)
      h = default_handler_.get();
    OptToken maybe = h->scan(*this);
    if (maybe && *maybe)
      tokens.push_back(std::move(*maybe));
  }
  tokens.push_back(TokenFactory::make_eof(line_, col_));
  return tokens;
}

void Lexer::register_defaults() {
  for (auto &p : handlers_)
    p.reset();
  default_handler_.reset();

  set_handler(' ', std::make_unique<WhitespaceHandler>());
  set_handler('\t', std::make_unique<WhitespaceHandler>());
  set_handler('\r', std::make_unique<WhitespaceHandler>());

  set_handler('\n', std::make_unique<NewlineHandler>());

  for (unsigned char c = 'a'; c <= 'z'; ++c)
    set_handler(c, std::make_unique<IdentifierHandler>());
  for (unsigned char c = 'A'; c <= 'Z'; ++c)
    set_handler(c, std::make_unique<IdentifierHandler>());
  set_handler((unsigned char) '_', std::make_unique<IdentifierHandler>());

  for (unsigned char d = '0'; d <= '9'; ++d)
    set_handler(d, std::make_unique<NumberHandler>());
  set_handler((unsigned char) '.', std::make_unique<NumberHandler>());

  set_handler((unsigned char) '"', std::make_unique<StringHandler>());
  set_handler((unsigned char) '\'', std::make_unique<CharHandler>());

  set_handler((unsigned char) '/', std::make_unique<SlashHandler>());

  const std::string opchars = "+-*/%<>=!&|^~?:.";
  for (unsigned char c : opchars)
    set_handler(c, std::make_unique<OperatorHandler>());

  const std::string puncts = ",;:(){}[]";
  for (unsigned char c : puncts)
    set_handler(c, std::make_unique<PunctHandler>());

  set_default_handler(std::make_unique<DefaultHandler>());
}
