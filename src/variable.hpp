#pragma once
#include "error.hpp"
#include <iostream>
#include <variant>
namespace Bkornblume {
class ArrayNode {
private:
  std::variant<ArrayNode *, int> data = nullptr;
  size_t size = 0;
  ArrayNode **nxt = nullptr;

public:
  ArrayNode(ArrayNode *_data, size_t _size = 1, int nxt_size = 1)
      : data(_data), size(_size), nxt(new ArrayNode *[nxt_size]) {}
  ~ArrayNode() {
    delete[] nxt;
    if (std::holds_alternative<ArrayNode *>(data)) {
      delete std::get<ArrayNode *>(data);
    }
  }
  std::variant<ArrayNode *, int> Get(int first_index, int second_index) {
    if (first_index < -1 || first_index >= size) {
      throw MyError("Index out of bounds!");
    }
    if(std::holds_alternative<int>(data)) {
        throw MyError("Cannot index into an integer!");
    }
    return std::get<ArrayNode *>(data)->Get(second_index);
  }
  std::variant<ArrayNode*, int> Get(int index) {
    if(index < -1 || index >= size) {
      throw MyError("Index out of bounds!");
    }
    return data;
  }
};
} // namespace Bkornblume