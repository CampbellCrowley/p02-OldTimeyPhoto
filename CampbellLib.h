#ifndef CAMPBELL_LIB_H
#define CAMPBELL_LIB_H

#include <algorithm>
#include <cstddef>
#include <iostream>

namespace Campbell {
// Other common things that don't have their own class yet.
class Common {
 private:
  Common() {}

 public:
  static bool isdigit(const char input);

};  // class Common

// For things relating to color in the terminal.
class Color {
 private:
  Color() {}

 public:
  // Escape codes for terminal colors.
  static constexpr const char *reset = "\033[1;0m";
  static constexpr const char *black = "\033[1;30m";
  static constexpr const char *red = "\033[1;31m";
  static constexpr const char *green = "\033[1;32m";
  static constexpr const char *yellow = "\033[1;33m";
  static constexpr const char *blue = "\033[1;34m";
  static constexpr const char *magenta = "\033[1;35m";
  static constexpr const char *cyan = "\033[1;36m";
  static constexpr const char *white = "\033[1;37m";
};  // class Color

// For things relating to arrays.
class Arrays {
 private:
  Arrays() {}

 public:
  // Finds the number of elements in an array input[].
  static inline int length(const char *input) {
    int Size = 0;
    while (input[Size] != '\0') Size++;
    return Size;
  }
  template <typename T>
  static inline int length(T &input) {
    return sizeof input / sizeof input[0];
  }
};  // class Arrays

// For things relating to arrays of characters and their manipuation or
// comparison.
class Strings {
 private:
  Strings() {}

 public:
  // Check if a string is a valid number. May start with -/+ and contain one
  // decimal and contain only numbers.
  static bool isNumber(const char input[]);
  // Converts a string to a double if isStringNumber returns true. Otherwise
  // returns 0.
  static double toNumber(const char input[]);
  // Takes a user input and chooses whether the input is equivalent to "Yes" or
  // "No". If defaultYes is true, it will assume true unless the input begins
  // with an 'n'. If false, it will assume "No" unless the input begins with
  // 'y'.
  static bool toYesNo(const char input[], bool defaultYes = true);

};  // class Strings

// Class for writing tests.
class Test {
 private:
  Test() {}

  // Outputs the message that the test passed.
  static void PASS() {
    std::cout << Campbell::Color::green << "PASS\n" << Campbell::Color::reset;
  }
  // Outputs message that the test failed as well as expected and actual values.
  template <typename T>
  static void FAIL(T actual, T expected) {
    std::cout << Campbell::Color::red << "FAIL" << Campbell::Color::reset
              << "\n Expected: " << expected << "\n Actual: " << actual
              << std::endl;
  }

 public:
  // Checks for exact equality.
  template <typename T>
  static void ASSERT(T actual, T expected) {
    if (expected == actual) {
      PASS();
    } else {
      FAIL(actual, expected);
    }
  }
  // Checks that two numbers are similar, to account for floating point
  // precision issues.
  template <typename T>
  static void ASSERT_SIMILAR(T actual, T expected) {
    double epsilon = std::max(abs(actual), abs(expected)) * 1E-15;
    if (abs(actual - expected) <= epsilon) {
      PASS();
    } else {
      FAIL(actual, expected);
    }
  }

};  // class Test
}  // namespace Campbell

#include "CampbellLib.cc"
#endif  // CAMPBELL_LIB_H
