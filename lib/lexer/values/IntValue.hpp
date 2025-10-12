#ifndef INTVALUE_HPP_
#define INTVALUE_HPP_

#include <cinttypes>

#include "Value.hpp"

struct IntValue : Value {
  int64_t v;
  explicit IntValue(int64_t x) : v(x) {
  }
  std::unique_ptr<Value> clone() const override {
    return std::make_unique<IntValue>(v);
  }
  std::string to_string() const override {
    return std::to_string(v);
  }
  std::string type_name() const override {
    return "Int";
  }
};

#endif // INTVALUE_HPP_
