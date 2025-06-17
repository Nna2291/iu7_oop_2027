#pragma once

#include <hashmap/HashMapExceptions.h>
#include <list/iterators/ConstListIter.h>

template <typename T>
ConstListIterator<T>::ConstListIterator()
{
}

template <typename T>
ConstListIterator<T>::ConstListIterator(const List<T> &map)
{
    nodePtr = map.lastNode;
}

template <typename T>
ConstListIterator<T>::ConstListIterator(const ConstListIterator<T> &iterator)
{
    nodePtr = iterator.nodePtr;
}

template <typename T>
ConstListIterator<T>::ConstListIterator(const std::shared_ptr<ListNode<T>> &node)
{
    nodePtr = node;
}

template <typename T>
ConstListIterator<T>::operator bool() const
{
    return isValid();
}

template <typename T>
auto ConstListIterator<T>::operator*() const -> reference
{
    return nodePtr.lock().get()->getValueRef();
}

template <typename T>
auto ConstListIterator<T>::operator->() const -> pointer
{
    return &(nodePtr.lock().get()->getValueRef());
}

// prefix
template <typename T>
ConstListIterator<T> &ConstListIterator<T>::operator++()
{
    nodePtr = nodePtr.lock()->getNext();
    return *this;
}

// postfix
template <typename T>
ConstListIterator<T> ConstListIterator<T>::operator++(int)
{
    ConstListIterator<T> copy(*this);

    ++(*this);

    return copy;
}

template <typename T>
ConstListIterator<T> ConstListIterator<T>::operator+(size_type offset) const
{
    ConstListIterator<T> offsetIter(*this);
    while (offset-- > 0)
        ++offsetIter;
    return offsetIter;
}

template <typename T>
ConstListIterator<T> &ConstListIterator<T>::operator+=(size_type offset)
{
    while (offset-- > 0)
        ++(*this);
    return *this;
}

template <typename T>
ConstListIterator<T> &ConstListIterator<T>::operator=(const ConstListIterator<T> &other)
{
    nodePtr = other.nodePtr;
    return *this;
}

template <typename T>
bool ConstListIterator<T>::operator==(const ConstListIterator<T> &other) const
{
    return other.nodePtr.lock() == nodePtr.lock();
}

template <typename T>
bool ConstListIterator<T>::isValid() const
{
    return nodePtr.lock() != nullptr && !nodePtr.expired();
}



template <typename T>
std::shared_ptr<ListNode<T>> ConstListIterator<T>::getPtr()
{
	return nodePtr.lock();
}

template <typename T>
const std::shared_ptr<ListNode<T>> ConstListIterator<T>::getPtr() const
{
	return nodePtr.lock();
}

static_assert(std::forward_iterator<ConstListIterator<int>>);
