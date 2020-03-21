#ifndef INTERVALMAP_H
#define INTERVALMAP_H

#include <iterator>
#include <limits>
#include <map>

template <class K, class V>
class IntervalMap
{
public:
  using kType = K;
  using vType = V;

public:
  IntervalMap(V const &val)
  {
    m_map.insert(m_map.begin(),
                 std::make_pair(std::numeric_limits<K>::min(), val));
  }
  const V &operator[](K const &key) const
  {
    auto it = m_map.upper_bound(key);
    --it;
    return it->second;
  }

  void assign(K const &keyBegin, K const &keyEnd, V const &val)
  {
    auto it = m_map.lower_bound(keyBegin);
    if (it != m_map.cbegin())
    {
      it--;
    }
    if (it!= m_map.end())
    {
      auto prevVal = it->second;
      m_map[keyBegin] = val;
      m_map[keyEnd] = prevVal;
    }
  }

private:
  std::map<K, V> m_map;
};
#endif // INTERVALMAP_H