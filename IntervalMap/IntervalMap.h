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
    if (right == left && val == prevRightValue) /* prevent inserting range of values that are already in there */
    {
      return;
    }
    if (left != m_map.cbegin()) /* RightMerge check */
    {
      auto l = left;
      l--;
      if (l != m_map.cbegin() && l->first == keyBegin) /* if and only if is side by side with same value*/
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
    m_map.erase(left, right); /* "eat" values between */
    if (prevLeftValue != val) /* check if new value is not old */
    {
      m_map[keyBegin] = val; /* inserting new value */
    }
    if (val != prevRightValue) /* continuing previous value */
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