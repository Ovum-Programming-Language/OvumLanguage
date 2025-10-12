#ifndef VALUE_HPP_
#define VALUE_HPP_

#include <memory>
#include <string>

struct Value {
  virtual ~Value() = default;
  virtual std::unique_ptr<Value> clone() const = 0;
  virtual std::string to_string() const = 0;
  virtual std::string type_name() const = 0;
};

#endif // VALUE_HPP_
