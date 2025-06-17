#pragma once

#include <list/ListNode.h>

template <MoveAndCopy T>
ListNode<T>::ListNode(const T &value, std::shared_ptr<ListNode<T>> next) : value(value)
{
    this->next = next;
}

template <MoveAndCopy T>
std::shared_ptr<ListNode<T>> ListNode<T>::getNext() const
{
    return next;
}

template <MoveAndCopy T>
T ListNode<T>::getValue() const
{
    return value;
}

template <MoveAndCopy T>
T &ListNode<T>::getValueRef()
{
    return value;
}

template <MoveAndCopy T>
const T &ListNode<T>::getValueRef() const
{
    return value;
}
