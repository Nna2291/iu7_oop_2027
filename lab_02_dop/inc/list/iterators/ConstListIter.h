#pragma once

#include "collection/BaseIterator.h"
#include "list/ListConcepts.h"
#include <cstddef>
#include <iterator>
#include <memory>

template <MoveAndCopy T>
class List;

template <MoveAndCopy T>
class ListNode;

template <typename T>
class ConstListIterator : public BaseIterator
{
  public:
    using value_type = const T;
    using pointer = const T *;
    using reference = const T &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = ConstListIterator<T>;
    using iterator_category = std::forward_iterator_tag;

    ConstListIterator();
    ConstListIterator(const List<T> &bucket);
    ConstListIterator(const ConstListIterator<T> &iterator);
    ConstListIterator(const std::shared_ptr<ListNode<T>> &node);

    reference operator*() const;
    pointer operator->() const;

    ConstListIterator<T> &operator++();
    ConstListIterator<T> operator++(int);
    ConstListIterator<T> operator+(size_type offset) const;
    ConstListIterator<T> &operator+=(size_type offset);

    ConstListIterator<T> &operator=(const ConstListIterator<T> &other);

    bool operator==(const ConstListIterator<T> &other) const;

    bool isValid() const;

    operator bool() const;

	std::shared_ptr<ListNode<T>> getPtr();
	const 	std::shared_ptr<ListNode<T>> getPtr() const;

  private:
    std::weak_ptr<ListNode<T>> nodePtr;
};

#include <list/iterators/ConstListIterImpl.hpp>
