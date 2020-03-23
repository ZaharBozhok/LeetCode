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
    auto keyBeginCopy = keyBegin;
    auto keyEndCopy = keyEnd;
    auto right = m_map.upper_bound(keyEndCopy);
    auto left = m_map.upper_bound(keyBeginCopy);
    if (left == m_map.cbegin()) 
    {
      keyBeginCopy = std::numeric_limits<K>::min();
      left++;
    }
    if (right == m_map.cbegin())
    {
      keyEndCopy = std::numeric_limits<K>::min();
      right++;
    }
    auto eq = [](const auto& t1, const auto& t2) -> bool {return (!(t1 < t2) && !(t2 < t1));};
    if (keyEndCopy < keyBeginCopy || eq(keyEndCopy, keyBeginCopy)) return;

    /* extracting previous values */
    V prevRightValue = std::prev(right)->second;
    V prevLeftValue = std::prev(left)->second;

    /* prevent inserting range of values that are already in there */
    if (right == left && val == prevRightValue)
      return;
    
    /* RightMerge check */
    if (left != m_map.cbegin()) 
    {
      auto l = std::prev(left);
      /* if and only if is side by side with same value*/
      if (l != m_map.cbegin() && eq(l->first, keyBeginCopy)) 
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
    /* check if new value is not old */
    if (!(prevLeftValue == val))
    {
      /* inserting new value */
      m_map[keyBeginCopy] = val; 
    }
    /* continuing previous value */
    if (!(prevRightValue == val)) 
    {
      m_map[keyEndCopy] = prevRightValue;
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