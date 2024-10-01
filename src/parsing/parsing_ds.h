#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace SLCParsing {

enum class OpType {
  UnMinus,
  Sin,
  Cos,
  Tan,
  ArcSin,
  ArcCos,
  ArcTan,
  Log,
  Ln,
  BinPlus,
  BinMinus,
  BinMult,
  BinDivide,
  BinPow,
};

struct Expr {
  using Exprs = std::vector<Expr>;
  using ExprVariant = std::variant<double,      // Double
                                   std::string, // Variable
                                   OpType>;     // Operator

  public:
    Expr() = default;
    ~Expr() = default;

    Expr(const ExprVariant& value) : value_(value), child_(nullptr) {}

    Expr(const Expr& other) : value_(other.value_),
      child_(other.child_ ? std::make_unique<Exprs>(*other.child_) : nullptr) {}

    auto operator=(const Expr& other) -> Expr& {
      if (this != &other) {
        value_ = other.value_;
        child_ = other.child_ ? std::make_unique<Exprs>(*other.child_) : nullptr;
      }
      return *this;
    }

    auto operator=(Expr&& other) noexcept -> Expr& {
      if (this != &other) {
        value_ = std::move(other.value_);
        child_ = std::move(other.child_);
      }
      return *this;
    }

    auto operator==(const Expr& other) const -> bool;
    auto operator!=(const Expr& other) const -> bool;

    auto debug_expr(int indent = 0) -> void;

    auto set_value(const ExprVariant& value) -> void { value_ = value; }
    auto value() -> ExprVariant { return value_; }

    auto set_child(std::unique_ptr<Exprs> child) -> void { child_ = std::move(child); }
    auto child() -> Exprs { return *child_; }

    auto add_children(Expr child) -> void {
      if (!child_) {
        child_ = std::make_unique<Exprs>(Exprs());
      }
      child_->push_back(child);
    }

    template <typename... Rest>
    auto add_children(Expr child, Rest... children) -> void {
      if (!child_) {
        child_ = std::make_unique<Exprs>(Exprs());
      }
      child_->push_back(child);
      add_children(children...);
    }

    auto add_children(ExprVariant child) -> void {
      if (!child_) {
        child_ = std::make_unique<Exprs>(Exprs());
      }
      child_->push_back(Expr(child));
    }

    template <typename... Rest>
    auto add_children(ExprVariant child, Rest... children) -> void {
      if (!child_) {
        child_ = std::make_unique<Exprs>(Exprs());
      }
      child_->push_back(Expr(child));
      add_children(children...);
    }

  private:
    ExprVariant value_;
    std::unique_ptr<Exprs> child_;
};

using Exprs = std::vector<Expr>;

} // namespace SLCParsing
