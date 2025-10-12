#ifndef BOOLVALUE_HPP_
#define BOOLVALUE_HPP_

#include <memory>
#include <string>

#include "Value.hpp"

struct BoolValue : Value {
  bool v;
  explicit BoolValue(bool b) : v(b) {
  }
  std::unique_ptr<Value> clone() const override {
    return std::make_unique<BoolValue>(v);
  }
  std::string to_string() const override {
    return v ? "true" : "false";
  }
  std::string type_name() const override {
    return "Bool";
  }
};

#endif // BOOLVALUE_HPP_
