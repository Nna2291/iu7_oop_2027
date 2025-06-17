#pragma once

#include "hashmap/iterators/ConstHashMapIter.h"

template <typename K, typename V>
ConstHashMapIterator<K, V>::ConstHashMapIterator(const buckets_iterator &current, const buckets_iterator &end)
    : ConstHashMapIterator(current, end, current != end ? current->begin() : local_iterator(nullptr))
{
    if (elementIterator == local_iterator(nullptr))
        moveNextBucket();
}

template <typename K, typename V>
ConstHashMapIterator<K, V>::ConstHashMapIterator(const buckets_iterator &current, const buckets_iterator &end,
                                                 const local_iterator &element)
{
    this->currentBucket = current;
    this->endBucket = end;
    this->elementIterator = element;
}

template <typename K, typename V>
ConstHashMapIterator<K, V> &ConstHashMapIterator<K, V>::operator=(const ConstHashMapIterator<K, V> &other)
{
    this->elementIterator = other.elementIterator;
    this->currentBucket = other.currentBucket;
    this->endBucket = other.endBucket;
}

template <typename K, typename V>
auto ConstHashMapIterator<K, V>::operator->() const -> pointer
{
    return &(elementIterator.getPtr()->getValueRef());
}

template <typename K, typename V>
auto ConstHashMapIterator<K, V>::operator*() const -> reference
{
    return elementIterator.getPtr()->getValueRef();
}

template <typename K, typename V>
ConstHashMapIterator<K, V>::operator bool() const
{
    return isValid();
}

template <typename K, typename V>
ConstHashMapIterator<K, V> &ConstHashMapIterator<K, V>::operator++()
{
    if (elementIterator)
        ++elementIterator;
    moveNextBucket();
    return *this;
}

template <typename K, typename V>
ConstHashMapIterator<K, V> ConstHashMapIterator<K, V>::operator++(int)
{
    iterator copy(*this);
    ++(*this);
    return copy;
}

template <typename K, typename V>
ConstHashMapIterator<K, V> ConstHashMapIterator<K, V>::operator+(size_type offset) const
{
    ConstHashMapIterator<K, V> newIter(*this);
    return newIter + offset;
}

template <typename K, typename V>
ConstHashMapIterator<K, V> &ConstHashMapIterator<K, V>::operator+=(size_type offset)
{
    while (--offset > 0)
        ++(*this);
    return *this;
}

template <typename K, typename V>
bool ConstHashMapIterator<K, V>::operator==(const ConstHashMapIterator<K, V> &iterator) const
{
    return currentBucket == iterator.currentBucket && endBucket == iterator.endBucket &&
           elementIterator == iterator.elementIterator;
}

template <typename K, typename V>
bool ConstHashMapIterator<K, V>::isValid() const
{
    return currentBucket.isValid() && elementIterator.isValid();
}



template <typename K, typename V>
void ConstHashMapIterator<K, V>::moveNextBucket()
{
    while (currentBucket != endBucket && elementIterator == currentBucket->end())
    {
        ++currentBucket;
        if (currentBucket != endBucket)
        {
            elementIterator = currentBucket->begin();
        }
    }
}

static_assert(std::forward_iterator<ConstHashMapIterator<std::string, int>>);
