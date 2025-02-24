#pragma once
#include "utils/error.hpp"
#include <variant>
namespace Bkornblume {
class ArrayNode {
private:
    std::variant<ArrayNode **, int *> data;
    int size        = 0;
    ArrayNode **nxt = nullptr;

public:
    ArrayNode(size_t _size = 1, int nxt_size = 1) :
        data(new ArrayNode *[_size]), size(_size), nxt(new ArrayNode *[nxt_size]) {}
    ~ArrayNode() {
        delete[] nxt;
        if (std::holds_alternative<ArrayNode **>(data)) {
            delete std::get<ArrayNode **>(data);
        }
    }
    std::variant<ArrayNode *, int *> Get(int first_index, int second_index) {
        if (first_index >= size) {
            throw UserError("Index out of range");
        }
        if (std::holds_alternative<ArrayNode **>(data)) {
            return std::get<ArrayNode **>(data)[first_index]->Get(second_index);
        } else {
            throw UserError("Integer don't have any child");
        }
    }
    std::variant<ArrayNode *, int *> Get(int index) {
        if (index >= size) {
            throw UserError("Index out of range");
        }
        if (std::holds_alternative<ArrayNode **>(data)) {
            return std::get<ArrayNode **>(data)[index];
        } else {
            return std::get<int *>(data);
        }
    }
};
} // namespace Bkornblume
