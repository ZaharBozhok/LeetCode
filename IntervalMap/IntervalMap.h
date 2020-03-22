#ifndef INTERVALMAP_H
#define INTERVALMAP_H

#include <iterator>
#include <limits>
#include <map>
#include <iomanip>
#include <iostream>

template<class Map>
void ShowMapFromTill(const Map& map, const typename Map::kType& keyBegin, const typename Map::kType& keyEnd)
{
  for(auto i=keyBegin; i<keyEnd; i++)
  {
    std::cout << std::setw(2) << map[i] << ' ';
  }
  std::cout << std::endl;
  for(auto i=keyBegin; i<keyEnd; i++)
  {
    std::cout << std::setw(2) << (int)i << ' ';
  }
  std::cout << std::endl;
}

template <class K, class V>
class IntervalMap
{
private:
  std::map<K, V> m_map;
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
    if(keyBegin > keyEnd)
    {
      return;
    }
    auto right = m_map.upper_bound(keyEnd);
    auto left = m_map.upper_bound(keyBegin);
    V prevValue = V();
    /* extracting previous value */
    {
      auto r = right; 
      r--;
      prevValue = r->second;
    }
    /* prevent inserting range of values that are already in there */
    if (right == left && val == prevValue)
    {
      return;
    }
    /* eliminate between */
    {
      m_map.erase(left, right);
    }
    /* inserting new value */
    m_map[keyBegin] = val;
    /* continuing previous value */
    if (val != prevValue)
    {
      m_map[keyEnd] = prevValue;
    }
  }
  const decltype(m_map)& getMap() const
  {
    return m_map;
  }
};
#endif // INTERVALMAP_H