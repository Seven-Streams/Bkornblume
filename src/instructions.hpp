#pragma once

/*
    The base class of all instructions.
*/
#include <string>
#include <vector>
namespace Bkornblume {
enum Instruction_type {
  _add,
  _sub,
  _mul,
  _sdiv,
  _srem,
  _shl,
  _ashr,
  _and,
  _or,
  _xor,
  _br,
  _ret,
  _alloca,
  _load,
  _store,
  _getelementptr,
  _icmp,
  _call,
  _phi,
  _select
};
class Instruction {
private:
  int id = -1;
  std::vector<std::string> ins_paras;
  Instruction_type type;

public:
  Instruction(int _id, std::vector<std::string> _paras, Instruction_type _type)
      : id(_id), ins_paras(_paras), type(_type) {}
  ~Instruction() = default;
  void Execute();
};
} // namespace Bkornblume
