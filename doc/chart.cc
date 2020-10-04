#include <cmath>

#include "dlog.h"
int main(int argc, char** argv) {
  for (int i = 0; i < 1000; i++) {
    if (std::rand() % 2) {
      Chart("a") << sin(i * 0.5);
    } else {
      Chart("b") << cos(i * 0.3) + 1;
    }
  }
  return 0;
}