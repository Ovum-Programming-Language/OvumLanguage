#ifndef STRINGVALUE_HPP_
#define STRINGVALUE_HPP_

#include <string>

#include "Value.hpp"

struct StringValue : Value {
  std::string v;
  explicit StringValue(std::string s) : v(std::move(s)) {
  }
  std::unique_ptr<Value> clone() const override {
    return std::make_unique<StringValue>(v);
  }
  std::string to_string() const override {
    return std::string("\"") + v + "\"";
  }
  std::string type_name() const override {
    return "String";
  }
};

#endif // STRINGVALUE_HPP_
