#pragma once

#include "list/ListConcepts.h"
#include "list/iterators/ConstListIter.h"
#include <collection/BaseCollection.h>
#include <list/ListNode.h>
#include <list/iterators/ListIter.h>

template <MoveAndCopy T>
class List : public BaseCollectionInterface
{
  public:
    friend class ListNode<T>;

    using iterator = ListIterator<T>;
    using const_iterator = ConstListIterator<T>;
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    List();
    List(std::initializer_list<T> list);
    List(const List<T> &list) = default;
    List(List<T> &&list) noexcept = default;

    explicit List(size_type size)
        requires DefaultConstructible<T>;

    List(size_type size, const value_type &instance)
        requires CopyConstructible<T>;

    template <ConvertibleIterator<T> Iter>
    List(Iter &&begin, Iter &&end);

	~List() = default;

    List<T> &operator=(const List<T> &list);
    List<T> &operator=(List<T> &&list) noexcept = default;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    iterator pushFront(const T &value);
    iterator pushBack(const T &value);

    void popFront() noexcept;
    void popBack() noexcept;

    void erase(const iterator& pos) noexcept;

    virtual void clear() noexcept override;
    virtual bool isEmpty() const override;

    reference getFront();
    const_reference getFront() const;

    reference getBack();
    const_reference getBack() const;

    reference at(size_type index);
    const_reference at(size_type index) const;
    reference operator[](size_type index);
    const_reference operator[](size_type index) const;

    void resize(size_type size)
        requires DefaultConstructible<T>;
    void resize(size_type size, const value_type &instance)
        requires CopyConstructible<T>;

    virtual size_type getSize() const noexcept override;

  protected:
    void eraseNode(std::shared_ptr<ListNode<T>> node) noexcept;

    std::shared_ptr<ListNode<T>> head;
};

#include <list/List.hpp>
