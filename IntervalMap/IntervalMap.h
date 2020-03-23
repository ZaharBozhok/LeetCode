#ifndef INTERVALMAP_H
#define INTERVALMAP_H

#include <limits>
#include <map>

template <typename K, typename V>
class interval_map
{
  std::map<K, V> m_map;

public:
  interval_map(V const &val)
  {
    m_map.insert(m_map.begin(),
                 std::make_pair(std::numeric_limits<K>::min(), val));
  }

  void assign(K const &keyBegin, K const &keyEnd, V const &val)
  {
    if (keyBegin >= keyEnd)
      return;
    auto right = m_map.upper_bound(keyEnd);
    auto left = m_map.upper_bound(keyBegin);
    /* extracting previous values */
    V prevRightValue = std::prev(right)->second;
    V prevLeftValue = std::prev(left)->second;

    /* prevent inserting range of values that are already in there */
    if (right == left && val == prevRightValue)
      return;

    if (left != m_map.cbegin()) /* RightMerge check */
    {
      auto l = std::prev(left);
      if (l != m_map.cbegin() && l->first == keyBegin) /* if and only if is side by side with same value*/
      {
        auto l1 = std::prev(l);
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

  V const &operator[](K const &key) const
  {
    return (--m_map.upper_bound(key))->second;
  }

public: /* for tests */
  const decltype(m_map) &getMap() const
  {
    return m_map;
  }

  using kType = K;
  using vType = V;
};
#endif // INTERVALMAP_H