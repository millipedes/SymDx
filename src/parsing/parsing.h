#pragma once

#include <cstdarg>
#include <cstring>
#include <functional>
#include <optional>

#include "parsing_ds.h"

namespace SLCParsing {

constexpr static auto max_word_len = 1024;

auto parse_ws(const char * input) -> const char *;

auto parse_variable_name(const char * input, Expr& data) -> const char *;
auto parse_number(const char * input, Expr& data) -> const char *;
auto parse_string(const char * input, Expr& expr) -> const char *;
auto parse_word(const char * input, const char * word) -> const char *;

auto parse_precedence_1_expr(const char * input, Expr& expr) -> const char *;
auto parse_precedence_2_expr(const char * input, Expr& expr) -> const char *;
auto parse_precedence_3_expr(const char * input, Expr& expr) -> const char *;
auto parse_precedence_4_expr(const char * input, Expr& expr) -> const char *;

} // namespace SLCParsing
