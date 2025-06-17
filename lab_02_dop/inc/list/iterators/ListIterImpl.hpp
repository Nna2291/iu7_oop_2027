#pragma once

#include <hashmap/HashMapExceptions.h>
#include <list/iterators/ListIter.h>

template <typename T>
ListIterator<T>::ListIterator()
{
}

template <typename T>
ListIterator<T>::ListIterator(const List<T> &map)
{
    nodePtr = map.lastNode;
}

template <typename T>
ListIterator<T>::ListIterator(const ListIterator<T> &iterator)
{
    nodePtr = iterator.nodePtr;
}

template <typename T>
ListIterator<T>::ListIterator(const std::shared_ptr<ListNode<T>> &node)
{
    nodePtr = node;
}

template <typename T>
ListIterator<T>::operator bool() const
{
    return isValid();
}

template <typename T>
auto ListIterator<T>::operator*() const -> reference
{
    return nodePtr.lock().get()->getValueRef();
}

template <typename T>
auto ListIterator<T>::operator->() const -> pointer
{
    return &(nodePtr.lock().get()->getValueRef());
}

// prefix
template <typename T>
ListIterator<T> &ListIterator<T>::operator++()
{
    nodePtr = nodePtr.lock()->getNext();
    return *this;
}

// postfix
template <typename T>
ListIterator<T> ListIterator<T>::operator++(int)
{
    ListIterator<T> copy(*this);

    ++(*this);

    return copy;
}

template <typename T>
ListIterator<T> ListIterator<T>::operator+(size_type offset) const
{
    ListIterator<T> offsetIter(*this);
    while (offset-- > 0)
        ++offsetIter;
    return offsetIter;
}

template <typename T>
ListIterator<T> &ListIterator<T>::operator+=(size_type offset)
{
    while (offset-- > 0)
        ++(*this);
    return *this;
}

template <typename T>
ListIterator<T> &ListIterator<T>::operator=(const ListIterator<T> &other)
{
    nodePtr = other.nodePtr;
    return *this;
}

template <typename T>
bool ListIterator<T>::operator==(const ListIterator<T> &other) const
{
    return other.nodePtr.lock() == nodePtr.lock();
}

template <typename T>
bool ListIterator<T>::isValid() const
{
    return nodePtr.lock() != nullptr && !nodePtr.expired();
}



template <typename T>
std::shared_ptr<ListNode<T>> ListIterator<T>::getPtr()
{
	return nodePtr.lock();
}

template <typename T>
const std::shared_ptr<ListNode<T>> ListIterator<T>::getPtr() const
{
	return nodePtr.lock();
}

static_assert(std::forward_iterator<ListIterator<int>>);
