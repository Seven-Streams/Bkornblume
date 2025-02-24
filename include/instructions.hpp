#pragma once

#include <string>
#include <vector>

namespace Bkornblume {

enum class Instruction_type {
    Add,
    Sub,
    Mul,
    Sdiv,
    Srem,
    Shl,
    Ashr,
    And,
    Or,
    Xor,
    Br,
    Ret,
    Alloca,
    Load,
    Store,
    Getelementptr,
    Icmp,
    Call,
    Phi,
    Select
};

class Instruction {
private:
    int id = -1;
    std::vector<std::string> ins_paras;
    Instruction_type type;

public:
    explicit Instruction(int id, std::vector<std::string> paras, Instruction_type type) :
        id(id), ins_paras(std::move(paras)), type(type) {}
    void Execute();
};

} // namespace Bkornblume
