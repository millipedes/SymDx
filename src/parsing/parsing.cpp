#include "parsing.h"

namespace SLCParsing {

auto is_double_delineator(char c) -> bool {
  return c == '.' || c == 'e' || c == 'E';
}

auto is_whitespace(char c) -> bool {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f';
}

auto parse_ws(const char * input) -> const char * {
  uint32_t inc = 0;
  while(is_whitespace(*(input + inc))) inc++;
  return input + inc;
}

auto parse_variable_name(const char * input, Expr& expr) -> const char * {
  uint32_t inc = 0;
  while (isalpha(*(input + inc)) || *(input + inc) == '_') inc++;
  if (inc > 0) {
    expr.set_value(std::string(input, inc));
    return input + inc;
  } else return NULL;
}

auto parse_number(const char * input, Expr& expr) -> const char * {
  uint32_t inc = 0;

  // decimal
  while (isdigit(*(input + inc))) inc++;

  if (*(input + inc) == '.') {
    inc++;
  } else if ((*(input + inc) == 'e' || *(input + inc) == 'E')
      && (*(input + inc + 1) == '-' || isdigit(*input + inc + 1))) {
    inc++;
    if (*(input + inc) == '-') inc++;
  }

  // fractional
  while (isdigit(*(input + inc))) inc++;

  if (inc > 0) {
    expr.set_value((double)strtod(input, NULL));
    return input + inc;
  } else return NULL;
}

auto parse_word(const char * input, const char * word) -> const char * {
  if (input) {
    size_t len = strnlen(word, max_word_len);
    if (!strncmp(input, word, len)) {
      return input + len;
    } else return NULL;
  } else return NULL;
}

auto parse_axiom(const char * input, Expr& expr) -> const char * {
  const char * maybe_axiom;
  if (maybe_axiom = parse_number(parse_ws(input), expr)) {
    return maybe_axiom;
  } else if (maybe_axiom = parse_variable_name(parse_ws(input), expr)) {
    return maybe_axiom;
  } else return NULL;
}

using Parser = std::function<const char *(const char *, Expr&)>;

auto make_unary_tree(const char * input, Expr& expr, Exprs& child, OpType type,
    Parser parser) -> const char * {
  expr = Expr(type);
  input = parser(parse_ws(input), child[0]);
  if (!input) return NULL;
  expr.set_child(std::make_unique<Exprs>(child));
  return input;
}

auto make_binary_tree(const char * input, Expr& expr, Exprs& child, OpType type,
    Parser parser) -> const char * {
    Expr parent(type);
    child[0] = expr;
    child.push_back(Expr());
    input = parser(parse_ws(input), child[1]);
    if (!input) return NULL;
    parent.set_child(std::make_unique<Exprs>(child));
    expr = parent;
    return input;
}

auto parse_precedence_1_expr(const char * input, Expr& expr) -> const char * {
  const char * factor;
  Exprs child(1);
  if (factor = parse_word(parse_ws(input), "-")) {
    return make_unary_tree(parse_ws(factor), expr, child,
        OpType::UnMinus, parse_precedence_3_expr);
  } else if (factor = parse_word(parse_ws(input), "sin")) {
    return make_unary_tree(parse_ws(factor), expr, child,
        OpType::Sin, parse_precedence_1_expr);
  } else if (factor = parse_word(parse_ws(input), "cos")) {
    return make_unary_tree(parse_ws(factor), expr, child,
        OpType::Cos, parse_precedence_1_expr);
  } else if (factor = parse_word(parse_ws(input), "tan")) {
    return make_unary_tree(parse_ws(factor), expr, child,
        OpType::Tan, parse_precedence_1_expr);
  } else if (factor = parse_word(parse_ws(input), "arcsin")) {
    return make_unary_tree(parse_ws(factor), expr, child,
        OpType::ArcSin, parse_precedence_1_expr);
  } else if (factor = parse_word(parse_ws(input), "arccos")) {
    return make_unary_tree(parse_ws(factor), expr, child,
        OpType::ArcCos, parse_precedence_1_expr);
  } else if (factor = parse_word(parse_ws(input), "arctan")) {
    return make_unary_tree(parse_ws(factor), expr, child,
        OpType::ArcTan, parse_precedence_1_expr);
  } else if (factor = parse_word(parse_ws(input), "log")) {
    return make_unary_tree(parse_ws(factor), expr, child,
        OpType::Log, parse_precedence_1_expr);
  } else if (factor = parse_word(parse_ws(input), "ln")) {
    return make_unary_tree(parse_ws(factor), expr, child,
        OpType::Ln, parse_precedence_1_expr);
  } else if (factor = parse_word(parse_ws(input), "(")) {
    factor = parse_precedence_4_expr(parse_ws(factor), expr);
    factor = parse_word(parse_ws(factor), ")");
  } else {
    factor = parse_axiom(parse_ws(input), expr);
  }
  return factor;
}

auto parse_precedence_2_expr(const char * input, Expr& expr) -> const char * {
  const char * expression = parse_precedence_1_expr(parse_ws(input), expr);
  const char * maybe_boolean_expr;
  Exprs child(1);
    if (expression) {
      while (true) {
        if (maybe_boolean_expr = parse_word(parse_ws(expression), "**")) {
          expression = make_binary_tree(parse_ws(maybe_boolean_expr), expr, child,
              OpType::BinPow, parse_precedence_2_expr);
        } else return expression;
      }
    } else return NULL;
}

auto parse_precedence_3_expr(const char * input, Expr& expr) -> const char * {
  const char * factor = parse_precedence_2_expr(parse_ws(input), expr);
  const char * maybe_term;
  Exprs child(1);
  if (factor) {
    while (true) {
      if (!(maybe_term = parse_word(parse_ws(factor), "**"))
          && (maybe_term = parse_word(parse_ws(factor), "*"))) {
        factor = make_binary_tree(parse_ws(maybe_term), expr, child,
            OpType::BinMult, parse_precedence_3_expr);
      } else if (maybe_term = parse_word(parse_ws(factor), "/")) {
        factor = make_binary_tree(parse_ws(maybe_term), expr, child,
            OpType::BinDivide, parse_precedence_3_expr);
      } else return factor;
    }
  } else return NULL;
}

auto parse_precedence_4_expr(const char * input, Expr& expr) -> const char * {
  const char * term = parse_precedence_3_expr(parse_ws(input), expr);
  const char * maybe_expression;
  Exprs child(1);
  if (term) {
    while (true) {
      if (maybe_expression = parse_word(parse_ws(term), "+")) {
        term = make_binary_tree(parse_ws(maybe_expression), expr, child,
            OpType::BinPlus, parse_precedence_4_expr);
      } else if (maybe_expression = parse_word(parse_ws(term), "-")) {
        term = make_binary_tree(parse_ws(maybe_expression), expr, child,
            OpType::BinMinus, parse_precedence_4_expr);
      } else {
        return term;
      }
    }
  } else return NULL;
}

} // namespace SLCParsing
