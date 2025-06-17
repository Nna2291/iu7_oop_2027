#pragma once

#include "collection/BaseCollection.h"
#include "hashmap/iterators/ConstHashMapIter.h"
#include "hashmap/iterators/HashMapIter.h"
#include "list/iterators/ListIter.h"

template <MoveAndCopy K, MoveAndCopy V, HashFunction<K> Hash = std::hash<K>,
          EqualFunction<K> KeyFunction = std::equal_to<K>>
class HashMap : public BaseCollectionInterface
{
    static_assert(MoveAndCopy<K>, "Key type must be move constructible and copy constructible");
    static_assert(MoveAndCopy<V>, "Value type must be move constructible and copy constructible");
    static_assert(HashFunction<Hash, K>, "Hash must be a valid hash function for key type");
    static_assert(EqualFunction<KeyFunction, K>, "KeyFunction must be a valid equality function for key type");

  public:
    using key_type = K;
    using mapped_type = V;
    using value_type = std::pair<const K, V>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyFunction;
    using iterator = HashMapIterator<K, V>;
    using const_iterator = ConstHashMapIterator<K, V>;
    using local_iterator = List<value_type>::iterator;
    using const_local_iterator = List<value_type>::const_iterator;
    using hashmap = HashMap<K, V, Hash, KeyFunction>;

    // Constructors
    HashMap();
    explicit HashMap(size_type size);
    HashMap(std::initializer_list<value_type> list);
    HashMap(hashmap &&map) = default;
    explicit HashMap(const hashmap &map) = default;

    template <ConvertibleIterator<std::pair<const K, V>> Iter>
    HashMap(Iter &&begin, Iter &&end);
    
    template <typename Container>
    requires requires(Container c) { 
        std::begin(c); 
        std::end(c); 
        typename Container::value_type; 
        std::convertible_to<typename Container::value_type, value_type>; 
    }
    explicit HashMap(const Container& container) : HashMap(std::begin(container), std::end(container)) {}

    virtual ~HashMap() = default;

    hashmap &operator=(const hashmap &map);
    hashmap &operator=(hashmap &&map) noexcept = default;

    // Iterators
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    // Capacity
    bool empty() const;
    size_type size() const;
    size_type max_size() const;

    // Modifiers
    void clear() noexcept override;
    virtual bool isEmpty() const override;
    std::pair<iterator, bool> insert(const value_type &value);
    std::pair<iterator, bool> insert(value_type &&value);
    std::pair<iterator, bool> insert(const K &key, const V &value);
    void insert_or_assign(const K &key, const V &value);
    void insert_or_assign(const value_type &value);
    
    template<class... Args>
    std::pair<iterator, bool> emplace(Args&&... args);
    
    template<class... Args>
    std::pair<iterator, bool> try_emplace(const K& k, Args&&... args);
    
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_type erase(const K& key);
    
    void swap(HashMap& other) noexcept;

    // Lookup
    V &at(const K &key);
    const V &at(const K &key) const;
    V &operator[](const K &key);
    const V &operator[](const K &key) const;
    size_type count(const K &key) const;
    iterator find(const K &key);
    const_iterator find(const K &key) const;
    bool contains(const K &key) const;
    std::pair<iterator, iterator> equal_range(const K& key);
    std::pair<const_iterator, const_iterator> equal_range(const K& key) const;

    // Bucket interface
    local_iterator begin(size_type bucket);
    local_iterator end(size_type bucket);
    const_local_iterator begin(size_type bucket) const;
    const_local_iterator end(size_type bucket) const;
    const_local_iterator cbegin(size_type bucket) const;
    const_local_iterator cend(size_type bucket) const;
    
    size_type bucket_count() const;
    size_type max_bucket_count() const;
    size_type bucket_size(size_type bucket) const;
    size_type bucket(const K &key) const;

    // Hash policy
    float load_factor() const;
    float max_load_factor() const;
    void max_load_factor(float ml);
    void rehash(size_type count);
    void reserve(size_type count);

    // Observers
    hasher hash_function() const;
    key_equal key_eq() const;

    // BaseCollection
    virtual size_type getSize() const override;

  private:
    float countLoadFactor() const;

    std::pair<iterator, bool> insert(List<List<value_type>> &buckets, const value_type &entry);

    bool isPrime(size_type value) const;
    size_type getNextPrime(size_type size) const;

    const Hash hashFunction;
    const KeyFunction keyEqualFunction;

    List<List<value_type>> buckets;

    float maxLoadFactor = 1.0f;
	  static constexpr size_type initialSize = 7;
};

#include <hashmap/HashMap.hpp>
