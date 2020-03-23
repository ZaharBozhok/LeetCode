#include <iostream>
#include <limits>
#include "IntervalMap.h"


class MyNumeric
{
  public:
  MyNumeric(int val) : m_val(val) {}
  bool operator==(const MyNumeric&) = delete;
  bool operator>(const MyNumeric&) = delete;
  bool operator<(const MyNumeric& k) const
  {
    return m_val < k.m_val;
  }
  friend std::ostream& operator<<(std::ostream&, const MyNumeric&);
  private:
  int m_val;
};

std::ostream& operator<<(std::ostream& o, const MyNumeric& num)
{
  o << num.m_val;
  return o;
}

namespace std {
        template<> class numeric_limits<MyNumeric> {
           public:
          static MyNumeric min() {return MyNumeric(0);};
          static MyNumeric max() {return MyNumeric(41);}
        };
} 

bool eqViaLess(const MyNumeric& k1, const MyNumeric& k2)
{
  return (!(k1 < k2) && !(k2 < k1));
}

/* TODO: maybe remove IntervalMap executable */
int main() {
  std::cout << "MyNumberic min : " << std::numeric_limits<MyNumeric>::min() << std::endl;
  std::cout << "MyNumberic max : " << std::numeric_limits<MyNumeric>::max() << std::endl;
  std::cout << "k1 == k2 : " << eqViaLess(MyNumeric(1),MyNumeric(1)) << std::endl;
  std::cout << "k1 != k2 : " << eqViaLess(MyNumeric(2),MyNumeric(1)) << std::endl;
  std::cout << "k1 != k2 : " << eqViaLess(MyNumeric(2),MyNumeric(3)) << std::endl;
  // std::cout << "k1 > k2 : " << (MyNumeric(4) > MyNumeric(5)) << std::endl;
  // std::cout << "k1 > k2 : " << (MyNumeric(4) == MyNumeric(5)) << std::endl;
  return 0;
}