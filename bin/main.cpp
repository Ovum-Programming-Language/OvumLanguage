#include <iostream>

#include "lib/lexer/Lexer.hpp"

int main() {
  const std::string sample = R"ovum(
// demo
fun Main(args: StringArray): Int {
    val count: Int = args.Length()
    sys::Print("Args count: " + count.ToString())
    return 0
}
)ovum";

  Lexer lx(sample, false);
  try {
    auto toks = lx.tokenize();
    for (auto &t : toks) {
      std::cout << t->to_string() << "\n";
    }
  } catch (const std::exception &e) {
    std::cerr << "Lexer error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
