#include "parsing_ds.h"

namespace SLCParsing {

auto Expr::operator==(const Expr& other) const -> bool {
  if (std::holds_alternative<double>(other.value_)
      && std::holds_alternative<double>(value_)) {
    if (!(std::get<double>(other.value_) == std::get<double>(value_))) {
      return false;
    }
  } else if (std::holds_alternative<std::string>(other.value_)
      && std::holds_alternative<std::string>(value_)) {
    if (!(std::get<std::string>(other.value_) == std::get<std::string>(value_))) {
      return false;
    }
  } else if (std::holds_alternative<OpType>(other.value_)
      && std::holds_alternative<OpType>(value_)) {
    if (!(std::get<OpType>(other.value_) == std::get<OpType>(value_))) {
      return false;
    }
  }
  if (child_ && other.child_) {
    if (child_->size() != other.child_->size()) {
      return false;
    }
    for (uint32_t i = 0; i < child_->size(); i++) {
      // Better synteax for this I am unaware of?
      if ((*child_)[i] != (*other.child_)[i]) {
        return false;
      }
    }
    return true;
  } else if (!child_ && !other.child_) {
    return true;
  } else return false;
}

auto Expr::operator!=(const Expr& other) const -> bool {
  return !(other == *this);
}

auto Expr::debug_expr(int indent) -> void {
  for (uint32_t i = 0; i < 2 * indent; i++) {
    std::cout << " ";
  }
  if (std::holds_alternative<double>(value_)) {
    std::cout << "double: " << std::get<double>(value_) << std::endl;
  } else if (std::holds_alternative<std::string>(value_)) {
    std::cout << "string: " << std::get<std::string>(value_) << std::endl;
  } else if (std::holds_alternative<OpType>(value_)) {
    switch(std::get<OpType>(value_)) {
      case OpType::UnMinus:
        std::cout << "-" << std::endl;
        break;
      case OpType::Sin:
        std::cout << "sin" << std::endl;
        break;
      case OpType::Cos:
        std::cout << "cos" << std::endl;
        break;
      case OpType::Tan:
        std::cout << "tan" << std::endl;
        break;
      case OpType::ArcSin:
        std::cout << "arcsin" << std::endl;
        break;
      case OpType::ArcCos:
        std::cout << "arccos" << std::endl;
        break;
      case OpType::ArcTan:
        std::cout << "arctan" << std::endl;
        break;
      case OpType::Log:
        std::cout << "log" << std::endl;
        break;
      case OpType::Ln:
        std::cout << "ln" << std::endl;
        break;
      case OpType::BinPlus:
        std::cout << "+" << std::endl;
        break;
      case OpType::BinMinus:
        std::cout << "-" << std::endl;
        break;
      case OpType::BinMult:
        std::cout << "*" << std::endl;
        break;
      case OpType::BinDivide:
        std::cout << "/" << std::endl;
        break;
      case OpType::BinPow:
        std::cout << "**" << std::endl;
        break;
    }
  }
  if (child_) {
    for(auto& children : *child_) {
      children.debug_expr(indent + 1);
    }
  }
}

} // namespace SLCParsing
