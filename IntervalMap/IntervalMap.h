#ifndef INTERVALMAP_H
#define INTERVALMAP_H

#include <iterator>
#include <limits>
#include <map>

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
    if(keyBegin >= keyEnd)
    {
      return;
    }
    auto right = m_map.upper_bound(keyEnd);
    auto left = m_map.upper_bound(keyBegin);
    V prevRightValue = V();
    V prevLeftValue = V();
    /* extracting previous values */
    {
      auto r = right; 
      auto l = left;
      r--;
      l--;
      prevRightValue = r->second;
      prevLeftValue = l->second;
    }
    /* prevent inserting range of values that are already in there */
    if (right == left && val == prevRightValue)
    {
      return;
    }
    /* RightMerge check */
    if (left != m_map.cbegin())
    {
      auto l = left;
      l--;
      if (l != m_map.cbegin() && l->first == keyBegin)
      {
        auto l1 = l;
        l1--;
        if (l1->second == val)
        {
          prevLeftValue = l1->second;
          left = l;
        }
      }
    }
    /* eliminate between */
    {
      m_map.erase(left, right);
    }
    /* check if new value is not old */
    if (prevLeftValue != val)
    {
    /* inserting new value */
      m_map[keyBegin] = val;
    }
    /* continuing previous value */
    if (val != prevRightValue)
    {
      m_map[keyEnd] = prevRightValue;
    }
  }
  const decltype(m_map)& getMap() const
  {
    return m_map;
  }
};
#endif // INTERVALMAP_H