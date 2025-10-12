#include "utils.hpp"

const std::unordered_set<std::string>& keyword_set() {
  static const std::unordered_set<std::string> s = {
      "fun",        "class",   "interface", "var",    "override", "pure",    "if",        "else",
      "for",        "while",   "return",    "unsafe", "val",      "static",  "public",    "private",
      "implements", "as",      "is",        "null",   "true",     "false",   "typealias", "destructor",
      "call",       "#import", "#define",   "#undef", "#ifdef",   "#ifndef", "#else",     "#endif"};
  return s;
}

const std::unordered_set<std::string>& multi_ops_set() {
  static const std::unordered_set<std::string> s = {"==", "!=", "<=", ">=", "&&", "||", "?:", "?.", "::", ":="};
  return s;
}
