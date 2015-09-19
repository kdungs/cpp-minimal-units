#include "unit.h"

#include <iostream>

int main() {
  auto m = Mass<double>{80.0};
  auto v = Velocity<double>{5.0};
  auto p = m * v;

  // auto fail = p + m;  // This does not work because units differ!

  std::cout << p << '\n';
}
