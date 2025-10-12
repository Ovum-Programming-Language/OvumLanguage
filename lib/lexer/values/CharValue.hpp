#ifndef CHARVALUE_HPP_
#define CHARVALUE_HPP_

#include <memory>
#include <string>

#include "Value.hpp"

struct CharValue : Value {
  char v;
  explicit CharValue(char c) : v(c) {
  }
  std::unique_ptr<Value> clone() const override {
    return std::make_unique<CharValue>(v);
  }
  std::string to_string() const override {
    return std::string("'") + v + "'";
  }
  std::string type_name() const override {
    return "Char";
  }
};

#endif // CHARVALUE_HPP_
