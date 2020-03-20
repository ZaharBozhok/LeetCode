#include <iostream>
#include <iterator>
#include <limits>
#include <map>

template <class K, class V> class IntervalMap {
public:
  IntervalMap(V const &val) {
    m_map.insert(m_map.begin(),
                 std::make_pair(std::numeric_limits<K>::min(), val));
  }
  V &operator[](K const &key) {
    auto it = m_map.upper_bound(key);
    --it;
    return it->second;
  }

  void assign(K const &keyBegin, K const &keyEnd, V const &val) {
	  
  }

private:
  std::map<K, V> m_map;
};

int main() {
  IntervalMap<unsigned int, char> imap{'a'};
  imap.assign(5, 10, 'b');
  std::cout << imap[5] << std::endl;
}