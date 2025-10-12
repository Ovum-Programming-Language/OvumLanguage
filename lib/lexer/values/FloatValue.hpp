#ifndef FLOATVALUE_HPP_
#define FLOATVALUE_HPP_

#include <memory>
#include <sstream>
#include <string>

#include "Value.hpp"

struct FloatValue : Value {
  long double v;
  explicit FloatValue(long double x) : v(x) {
  }
  std::unique_ptr<Value> clone() const override {
    return std::make_unique<FloatValue>(v);
  }
  std::string to_string() const override {
    std::ostringstream os;
    os << static_cast<double>(v);
    return os.str();
  }
  std::string type_name() const override {
    return "Float";
  }
};

#endif // FLOATVALUE_HPP_
