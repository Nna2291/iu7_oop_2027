#pragma once

#include "list/ListConcepts.h"
#include <memory>

template <MoveAndCopy T>
class List;

template <MoveAndCopy T>
class ListNode
{
    friend class List<T>;

  public:
    ListNode(const T &value, std::shared_ptr<ListNode<T>> next);

	std::shared_ptr<ListNode<T>> getNext() const;
    T getValue() const;

    T &getValueRef();
    const T &getValueRef() const;

  private:
    T value;

    std::shared_ptr<ListNode<T>> next;
};

#include <list/ListNode.hpp>
