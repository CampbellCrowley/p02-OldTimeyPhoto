#include "CampbellLib.h"
#include <math.h>

namespace Campbell {
bool Common::isdigit(const char input) {
  if ((int)input >= 48 && (int)input <= 57) return true;
  return false;
}

bool Strings::isNumber(const char input[]) {
  bool firstDecimal = true;
  for (int i = 0; i < Arrays::length(input); i++) {
    if (!Common::isdigit(input[i]) && input[i] != '.' &&
        !((input[i] == '-' || input[i] == '+') && i == 0)) {
      return false;
    }
    if (input[i] == '.') {
      if (firstDecimal) {
        firstDecimal = false;
      } else {
        return false;
      }
    }
  }
  return true;
}

double Strings::toNumber(const char input[]) {
  if (!isNumber(input)) return 0;
  unsigned int intermediate = 0;
  bool isSigned = input[0] == '+' || input[0] == '-';
  bool isNegative = input[0] == '-';
  int decimalLocation = Arrays::length(input);

  for (int i = 0; i < Arrays::length(input); i++) {
    if (input[i] == '.') {
      decimalLocation = i;
      break;
    }
  }

  for (int i = 0; i < Arrays::length(input); i++) {
    if (i == decimalLocation || (isSigned && i == 0)) {
      continue;
    }
    // Casting to int gives ASCII codes from 48 to 57 for 0-9. Shifting this
    // gives the correct numbers.
    int power = Arrays::length(input) - i + ((i >= decimalLocation) ? 0 : -1);
    intermediate += ((int)input[i] - 48) * pow(10, power);
  }

  double output =
      intermediate * pow(10, decimalLocation - Arrays::length(input));
  return isNegative ? -output : output;
}

// TODO: test this
bool Strings::toYesNo(const char input[], bool defaultYes) {
  if (defaultYes) {
    if (input[0] == 'n' || input[0] == 'N') {
      return false;
    } else {
      return true;
    }
  } else {
    if (input[0] == 'y' || input[0] == 'Y') {
      return true;
    } else {
      return false;
    }
  }
}

}  // namespace Campbell
