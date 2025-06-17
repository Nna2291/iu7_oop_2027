#pragma once

#include "hashmap/HashMap.h"
#include <algorithm>
#include <ranges>
#include <cmath>
#include <numeric>
#include <vector>
#include <limits>


template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
HashMap<K, V, Hash, KeyFunction>::HashMap() : HashMap(initialSize)
{
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
HashMap<K, V, Hash, KeyFunction>::HashMap(size_t size) : hashFunction(Hash()), keyEqualFunction(KeyFunction())
{
    size_type primeSize = getNextPrime(size > 1 ? size - 1 : 1);
    buckets = List<List<value_type>>(primeSize);
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
HashMap<K, V, Hash, KeyFunction>::HashMap(std::initializer_list<value_type> list) : HashMap(list.begin(), list.end())
{
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
template <ConvertibleIterator<std::pair<const K, V>> Iter>
HashMap<K, V, Hash, KeyFunction>::HashMap(Iter &&begin, Iter &&end) : HashMap()
{
    std::ranges::for_each(begin, end, [this](const auto& value) {
        insert(value);
    });
}

// Constructor from container implementation is in-class due to template constraints

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
HashMap<K, V, Hash, KeyFunction> &HashMap<K, V, Hash, KeyFunction>::operator=(const hashmap &map)
{
    buckets = List<List<value_type>>(map.buckets);
    return *this;
}


template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::begin() -> iterator
{
    return iterator(buckets.begin(), buckets.end());
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::end() -> iterator
{
    return iterator(buckets.end(), buckets.end());
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::begin() const -> const_iterator
{
    return const_iterator(buckets.begin(), buckets.end());
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::end() const -> const_iterator
{
    return const_iterator(buckets.end(), buckets.end());
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::cbegin() const -> const_iterator
{
    return const_iterator(buckets.cbegin(), buckets.cend());
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::cend() const -> const_iterator
{
    return const_iterator(buckets.cend(), buckets.cend());
}



template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
void HashMap<K, V, Hash, KeyFunction>::clear() noexcept
{
    std::ranges::for_each(buckets, [](auto& bucket) {
        bucket.clear();
    });
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
bool HashMap<K, V, Hash, KeyFunction>::isEmpty() const
{
    return getSize() == 0;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
void HashMap<K, V, Hash, KeyFunction>::insert_or_assign(const K &key, const V &value)
{
    auto result = insert(key, value);
    if (!result.second)
        result.first->second = value;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
void HashMap<K, V, Hash, KeyFunction>::insert_or_assign(const value_type &value)
{
    auto result = insert(value);
    if (!result.second)
        result.first->second = value.second;
}



template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::insert(const value_type &entry) -> std::pair<iterator, bool>
{
    if (countLoadFactor() >= maxLoadFactor)
        rehash(bucket_count() * 2);

    return insert(buckets, entry);
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::insert(value_type &&entry) -> std::pair<iterator, bool>
{
    if (countLoadFactor() >= maxLoadFactor)
        rehash(bucket_count() * 2);

    return insert(buckets, std::move(entry));
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::insert(const K &key, const V &value) -> std::pair<iterator, bool>
{
    return insert(std::make_pair(key, value));
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
template<class... Args>
auto HashMap<K, V, Hash, KeyFunction>::emplace(Args&&... args) -> std::pair<iterator, bool>
{
    return insert(value_type(std::forward<Args>(args)...));
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
template<class... Args>
auto HashMap<K, V, Hash, KeyFunction>::try_emplace(const K& k, Args&&... args) -> std::pair<iterator, bool>
{
    auto it = find(k);
    if (it != end()) {
        return {it, false};
    }
    return emplace(k, V(std::forward<Args>(args)...));
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::erase(const_iterator pos) -> iterator
{
    if (pos == end()) return end();
    
    // Find the key to erase
    const K& key = pos->first;
    auto next = pos;
    ++next;
    
    erase(key);
    return iterator(next);
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::erase(const_iterator first, const_iterator last) -> iterator
{
    auto it = first;
    while (it != last) {
        auto to_erase = it;
        ++it;
        erase(to_erase->first);
    }
    return iterator(it);
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::erase(const K &key) -> size_type
{
    size_type index = bucket(key);
    auto &bucket = buckets[index];

    auto it = std::ranges::find_if(bucket.begin(), bucket.end(),
                           [&key, this](const value_type &value) { return this->keyEqualFunction(value.first, key); });
    if (it == bucket.end())
        return 0;

    bucket.erase(it);

    return 1;
}



template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
V &HashMap<K, V, Hash, KeyFunction>::at(const K &key)
{
    auto it = find(key);
    if (it == end())
        throw InvalidKeyException(__FILE_NAME__, typeid(*this).name(), __PRETTY_FUNCTION__, __LINE__);

    return it->second;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
const V &HashMap<K, V, Hash, KeyFunction>::at(const K &key) const
{
    auto it = find(key);
    if (it == end())
        throw InvalidKeyException(__FILE_NAME__, typeid(*this).name(), __PRETTY_FUNCTION__, __LINE__);

    return it->second;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
V &HashMap<K, V, Hash, KeyFunction>::operator[](const K &key)
{
    auto it = find(key);
    if (it == end()) {
        auto result = insert(std::make_pair(key, V{}));
        return result.first->second;
    }
    return it->second;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
const V &HashMap<K, V, Hash, KeyFunction>::operator[](const K &key) const
{
    return at(key);
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::find(const K &key) -> iterator
{
    size_type index = bucket(key);

    auto it = std::ranges::find_if(begin(index), end(index),
                           [&key, this](const value_type &value) { return this->keyEqualFunction(value.first, key); });

    if (it == end(index)) {
        return end();
    }
    
    return iterator(buckets.begin() + index, buckets.end(), it);
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::find(const K &key) const -> const_iterator
{
    size_type index = bucket(key);

    auto it = std::ranges::find_if(begin(index), end(index),
                           [&key, this](const value_type &value) { return this->keyEqualFunction(value.first, key); });

    if (it == end(index)) {
        return end();
    }
    
    return const_iterator(buckets.begin() + index, buckets.end(), it);
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
bool HashMap<K, V, Hash, KeyFunction>::contains(const K &key) const
{
	size_type index = bucket(key);

	return std::ranges::any_of(begin(index), end(index), [this,&key](const value_type& value){return this->keyEqualFunction(value.first, key);});
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::count(const K &key) const -> size_type
{
    return contains(key) ? 1 : 0;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::equal_range(const K& key) -> std::pair<iterator, iterator>
{
    auto it = find(key);
    if (it == end()) {
        return {end(), end()};
    }
    auto next = it;
    ++next;
    return {it, next};
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::equal_range(const K& key) const -> std::pair<const_iterator, const_iterator>
{
    auto it = find(key);
    if (it == end()) {
        return {end(), end()};
    }
    auto next = it;
    ++next;
    return {it, next};
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
void HashMap<K, V, Hash, KeyFunction>::swap(HashMap& other) noexcept
{
    std::swap(buckets, other.buckets);
    std::swap(maxLoadFactor, other.maxLoadFactor);
}



template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::begin(size_type bucket) -> local_iterator
{
    return buckets[bucket].begin();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::end(size_type bucket) -> local_iterator
{
    return buckets[bucket].end();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::begin(size_type bucket) const -> const_local_iterator
{
    return buckets[bucket].begin();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::end(size_type bucket) const -> const_local_iterator
{
    return buckets[bucket].end();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::cbegin(size_type bucket) const -> const_local_iterator
{
    return buckets[bucket].cbegin();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::cend(size_type bucket) const -> const_local_iterator
{
    return buckets[bucket].cend();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::bucket_count() const -> size_type
{
    return buckets.getSize();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::max_bucket_count() const -> size_type
{
    return std::numeric_limits<size_type>::max();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::bucket_size(size_type bucket) const -> size_type
{
    return buckets[bucket].getSize();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::bucket(const K &key) const -> size_type
{
    return hashFunction(key) % bucket_count();
}



template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
float HashMap<K, V, Hash, KeyFunction>::max_load_factor() const
{
    return maxLoadFactor;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
void HashMap<K, V, Hash, KeyFunction>::max_load_factor(float ml)
{
    this->maxLoadFactor = ml;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
void HashMap<K, V, Hash, KeyFunction>::rehash(size_type count)
{
    size_type nextSize = getNextPrime(std::max(static_cast<size_type>(getSize() / max_load_factor()), count));

    List<List<value_type>> newBuckets(nextSize);

    std::ranges::for_each(*this, [this, &newBuckets](const auto& it) {
        insert(newBuckets, it);
    });

    buckets = newBuckets;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
void HashMap<K, V, Hash, KeyFunction>::reserve(size_type count)
{
    rehash(std::ceil(count / max_load_factor()));
}



template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::key_eq() const -> key_equal
{
    return keyEqualFunction;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::hash_function() const -> hasher
{
    return hashFunction;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::getSize() const -> size_type
{
    size_type total = 0;
    std::ranges::for_each(buckets, [&total](const auto& bucket) {
        total += bucket.getSize();
    });
    return total;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
float HashMap<K, V, Hash, KeyFunction>::countLoadFactor() const
{
    return static_cast<float>(getSize()) / bucket_count();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::insert(List<List<value_type>> &buckets, const value_type &entry)
    -> std::pair<iterator, bool>
{
    size_t hash = hashFunction(entry.first);
    size_type index = hash % buckets.getSize();
    auto &bucket = buckets[index];

    auto coll = find_if(bucket.begin(), bucket.end(), [&entry, this](const value_type &value) {
        return this->keyEqualFunction(value.first, entry.first);
    });
    if (coll != bucket.end())
        return std::make_pair(iterator(buckets.begin() + index, buckets.end(), coll), false);

    auto res = bucket.pushFront(entry);
    return std::make_pair(iterator(buckets.begin() + index, buckets.end(), res), true);
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::getNextPrime(size_type size) const -> size_type
{
    while (size < std::numeric_limits<size_type>::max() && !isPrime(++size))
        ;
    return size;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
bool HashMap<K, V, Hash, KeyFunction>::isPrime(size_type value) const
{
    return !std::ranges::any_of(std::views::iota(size_type{2}, value), [value](size_type i) {
        return value % i == 0;
    });
}


// Additional lookup methods

// Additional observer methods
template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
bool HashMap<K, V, Hash, KeyFunction>::empty() const
{
    return getSize() == 0;
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::size() const -> size_type
{
    return getSize();
}

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash, EqualFunction<K> KeyFunction>
auto HashMap<K, V, Hash, KeyFunction>::max_size() const -> size_type
{
    return std::numeric_limits<size_type>::max();
}

