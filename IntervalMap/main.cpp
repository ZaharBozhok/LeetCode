#include <iostream>
#include "IntervalMap.h"

int main() {
  IntervalMap<unsigned int, char> imap{'a'};
  imap.assign(5, 10, 'b');
  std::cout << imap[5] << std::endl;
}