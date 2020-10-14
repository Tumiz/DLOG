#include <cmath>
#include <thread>
#include "dlog.h"
int main(int argc, char** argv) {
  for (int i = 0; i < 1000; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    if (std::rand() % 2) {
      Chart("a") << sin(i * 0.05);
    } else {
      Chart("b") << cos(i * 0.01) + 1;
    }
  }
  return 0;
}