#pragma once

#include "hashmap/HashMapConcepts.h"
#include "list/List.h"

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
class HashMap;

template <typename K, typename V>
class ConstHashMapIterator : public BaseIterator
{
	template<typename T>
	friend class ConstListIterator;
  public:
    using iterator_category = std::forward_iterator_tag;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using value_type = std::pair<const K, V>;
    using const_value_type = const value_type;
    using pointer = const_value_type *;
    using reference = const_value_type &;
    using local_iterator = List<value_type>::const_iterator;
    using iterator = ConstHashMapIterator<K, V>;
    using buckets_iterator = List<List<value_type>>::const_iterator;

    ConstHashMapIterator() = default;
    ConstHashMapIterator(const buckets_iterator &current, const buckets_iterator &end);
    ConstHashMapIterator(const buckets_iterator &current, const buckets_iterator &end, const local_iterator &element);

    operator bool() const;

    pointer operator->() const;
    reference operator*() const;

    ConstHashMapIterator<K, V> &operator=(const ConstHashMapIterator<K, V> &other);

    ConstHashMapIterator<K, V> &operator++();
    ConstHashMapIterator<K, V> operator++(int);
    ConstHashMapIterator<K, V> operator+(size_type offset) const;
    ConstHashMapIterator<K, V> &operator+=(size_type offset);

    bool operator==(const ConstHashMapIterator<K, V> &iterator) const;

    bool isValid() const;

  protected:
    void moveNextBucket();

    local_iterator elementIterator;
    buckets_iterator currentBucket;
    buckets_iterator endBucket;
};

#include <hashmap/iterators/ConstHashMapIterImpl.hpp>
