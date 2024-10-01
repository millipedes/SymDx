#include "test_helper.h"

auto test_expr(const slcp::Expr& expr, const slcp::Expr& value) -> void {
  ASSERT_EQ(expr, value);
}
