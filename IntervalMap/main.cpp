#include <iostream>
#include "IntervalMap.h"

/* TODO: maybe remove IntervalMap executable */
int main() {
  IntervalMap<unsigned int, char> m{'a'};
  m.assign(5, 10, 'b');
  //ShowMapFromTill(m, 0, 20);
}