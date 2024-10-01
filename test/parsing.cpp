#include "test_helper.h"

namespace slcp = SLCParsing;

TEST(parsing, number_test_0) {
  const char * input = "123";
  slcp::Expr expr;
  auto remainder = slcp::parse_number(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  test_expr(expr, slcp::Expr(123.));
}

TEST(parsing, number_test_1) {
  const char * input = "123.123";
  slcp::Expr expr;
  auto remainder = slcp::parse_number(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  test_expr(expr, slcp::Expr(123.123));
}

TEST(parsing, number_test_2) {
  const char * input = "1e-2";
  slcp::Expr expr;
  auto remainder = slcp::parse_number(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  test_expr(expr, slcp::Expr(0.01));
}

TEST(parsing, number_test_3) {
  const char * input = "1E-2";
  slcp::Expr expr;
  auto remainder = slcp::parse_number(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  test_expr(expr, slcp::Expr(0.01));
}

TEST(parsing, word_test_0) {
  const char * input = ".\\";
  auto remainder = slcp::parse_word(input, ".\\");
  ASSERT_EQ(remainder[0], '\0');
}

TEST(parsing, precedence_1_test_0) {
  const char * input = "(((((-1e-2)))))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_1_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result = slcp::Expr(slcp::OpType::UnMinus);
  result.add_children(0.01);
  test_expr(expr, result);
}

TEST(parsing, precedence_1_test_1) {
  const char * input = "-(((((-1e-2)))))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_1_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::UnMinus);
  result_child.add_children(0.01);
  auto result = slcp::Expr(slcp::OpType::UnMinus);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_2_test_0) {
  const char * input = "(-1e-2 ** 0.1) ** 2";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_2_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child_child = slcp::Expr(slcp::OpType::UnMinus);
  result_child_child.add_children(0.01);
  auto result_child = slcp::Expr(slcp::OpType::BinPow);
  result_child.add_children(result_child_child, 0.1);
  auto result = slcp::Expr(slcp::OpType::BinPow);
  result.add_children(result_child, 2.);
  expr.debug_expr();
  result.debug_expr();
  test_expr(expr, result);
}

TEST(parsing, precedence_2_test_1) {
  const char * input = "-1e-2 ** 0.1 ** 2";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_2_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child_one = slcp::Expr(slcp::OpType::UnMinus);
  result_child_one.add_children(0.01);
  auto result_child_two = slcp::Expr(slcp::OpType::BinPow);
  result_child_two.add_children(0.1, 2.);
  auto result = slcp::Expr(slcp::OpType::BinPow);
  result.add_children(result_child_one, result_child_two);
  expr.debug_expr();
  result.debug_expr();
  test_expr(expr, result);
}

TEST(parsing, precedence_3_test_0) {
  const char * input = "1e-2 * 1e-2";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_3_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result = slcp::Expr(slcp::OpType::BinMult);
  result.add_children(0.01, 0.01);
  test_expr(expr, result);
}

TEST(parsing, precedence_3_test_1) {
  const char * input = "1e-2 * 1e-2 * 1e-3";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_3_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinMult);
  result_child.add_children(0.01, 0.001);
  auto result = slcp::Expr(slcp::OpType::BinMult);
  result.add_children(0.01, result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_3_test_2) {
  const char * input = "1 / 2 * 3";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_3_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinMult);
  result_child.add_children(2., 3.);
  auto result = slcp::Expr(slcp::OpType::BinDivide);
  result.add_children(1., result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_0) {
  const char * input = "-123";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result = slcp::Expr(slcp::OpType::UnMinus);
  result.add_children(123.);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_1) {
  const char * input = "-1e-2";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result = slcp::Expr(slcp::OpType::UnMinus);
  result.add_children(0.01);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_4) {
  const char * input = "1 + 2";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result = slcp::Expr(slcp::OpType::BinPlus);
  result.add_children(1., 2.);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_5) {
  const char * input = "1 + 2 * 3";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinMult);
  result_child.add_children(2., 3.);
  auto result = slcp::Expr(slcp::OpType::BinPlus);
  result.add_children(1., result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_6) {
  const char * input = "(1 + 2) * 3";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::BinMult);
  result.add_children(result_child, 3.);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_7) {
  const char * input = "-(1 + 2)";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::UnMinus);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_8) {
  const char * input = "(((((1 - - 2)))))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::UnMinus);
  result_child.add_children(2.);
  auto result = slcp::Expr(slcp::OpType::BinMinus);
  result.add_children(1., result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_9) {
  const char * input = "1 ** 2";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result = slcp::Expr(slcp::OpType::BinPow);
  result.add_children(1., 2.);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_10) {
  const char * input = "(1 + 2) ** -(1 + 2)";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child_one = slcp::Expr(slcp::OpType::BinPlus);
  result_child_one.add_children(1., 2.);
  auto result_child_two_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child_two_child.add_children(1., 2.);
  auto result_child_two = slcp::Expr(slcp::OpType::UnMinus);
  result_child_two.add_children(result_child_two_child);
  auto result = slcp::Expr(slcp::OpType::BinPow);
  result.add_children(result_child_one, result_child_two);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_19) {
  const char * input = "sin((1 + 2))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::Sin);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_20) {
  const char * input = "cos((1 + 2))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::Cos);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_21) {
  const char * input = "tan((1 + 2))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::Tan);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_22) {
  const char * input = "arcsin((1 + 2))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::ArcSin);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_23) {
  const char * input = "arccos((1 + 2))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::ArcCos);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_24) {
  const char * input = "arctan((1 + 2))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::ArcTan);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_25) {
  const char * input = "log((1 + 2))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::Log);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_26) {
  const char * input = "ln((1 + 2))";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(1., 2.);
  auto result = slcp::Expr(slcp::OpType::Ln);
  result.add_children(result_child);
  test_expr(expr, result);
}

TEST(parsing, precedence_4_test_30) {
  const char * input = "-ln(0.5) + 1.0 * 2.0";
  slcp::Expr expr;
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child_child_one = slcp::Expr(slcp::OpType::Ln);
  result_child_child_one.add_children(0.5);
  auto result_child_one = slcp::Expr(slcp::OpType::UnMinus);
  result_child_one.add_children(result_child_child_one);
  auto result_child_two = slcp::Expr(slcp::OpType::BinMult);
  result_child_two.add_children(1.0, 2.0);
  auto result = slcp::Expr(slcp::OpType::BinPlus);
  result.add_children(result_child_one, result_child_two);
  test_expr(expr, result);
}

TEST(evaluation, evaluate_expression_test_20) {
  slcp::Expr expr;
  const char * input = "- 7 * 4 ** 9";
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child_child = slcp::Expr(slcp::OpType::BinPow);
  result_child_child.add_children(4., 9.);
  auto result_child = slcp::Expr(slcp::OpType::BinMult);
  result_child.add_children(7., result_child_child);
  auto result = slcp::Expr(slcp::OpType::UnMinus);
  result.add_children(result_child);
  result.debug_expr();
  expr.debug_expr();
  test_expr(expr, result);
}

// TEST(evaluation, evaluate_expression_test_21) {
//   slcp::Expr expr;
//   const char * input = "16 + 19 - 7 * 4 ** 9";
//   auto remainder = slcp::parse_precedence_4_expr(input, expr);
//   ASSERT_EQ(remainder[0], '\0');
//   auto result_child_child = slcp::Expr(slcp::OpType::BinPow);
//   result_child_child.add_children(4., 9.);
//   auto result_child = slcp::Expr(slcp::OpType::BinMult);
//   result_child.add_children(7., result_child_child);
//   auto result = slcp::Expr(slcp::OpType::UnMinus);
//   result.add_children(result_child);
//   result.debug_expr();
//   expr.debug_expr();
//   test_expr(expr, result);
// }

TEST(evaluation, evaluate_expression_test_19) {
  slcp::Expr expr;
  const char * input = "16 + 19 - 7 * 4 ** 9 + 1834976";
  auto remainder = slcp::parse_precedence_4_expr(input, expr);
  ASSERT_EQ(remainder[0], '\0');
  auto result_child_child_one = slcp::Expr(slcp::OpType::BinPlus);
  result_child_child_one.add_children(16., 19.);
  auto result_child_child_two_child_child_two = slcp::Expr(slcp::OpType::BinPow);
  result_child_child_two_child_child_two.add_children(4., 9.);
  auto result_child_child_two_child = slcp::Expr(slcp::OpType::BinMult);
  result_child_child_two_child.add_children(7., result_child_child_two_child_child_two);
  auto result_child_child_two = slcp::Expr(slcp::OpType::UnMinus);
  result_child_child_two.add_children(result_child_child_two_child);
  auto result_child = slcp::Expr(slcp::OpType::BinPlus);
  result_child.add_children(result_child_child_one, result_child_child_two);
  auto result = slcp::Expr(slcp::OpType::BinPlus);
  result.add_children(result_child, 1834976.);
  result.debug_expr();
  expr.debug_expr();
  test_expr(expr, result);
}
