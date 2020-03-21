#include <iostream>
#include "IntervalMap.h"

int main() {
  IntervalMap<unsigned int, char> m{'a'};
  m.assign(5, 10, 'b');
  ShowMapFromTill(m, 0, 20);
}