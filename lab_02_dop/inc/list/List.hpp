#pragma once

#include "list/List.h"
#include <iostream>
#include <list/ListExceptions.h>
#include <ranges>
#include <algorithm>

template <MoveAndCopy T>
List<T>::List()
{
}

template <MoveAndCopy T>
template <ConvertibleIterator<T> Iter>
List<T>::List(Iter &&begin, Iter &&end)
{
    std::ranges::for_each(begin, end, [this](const auto& value) {
        pushBack(value);
    });
}

template <MoveAndCopy T>
List<T>::List(std::initializer_list<T> list) : List(list.begin(), list.end())
{
}

template <MoveAndCopy T>
List<T>::List(size_type size)
    requires DefaultConstructible<T>
{
    resize(size);
}

template <MoveAndCopy T>
List<T>::List(size_type size, const value_type &instance)
    requires CopyConstructible<T>
{
    resize(size, instance);
}

template <MoveAndCopy T>
auto List<T>::pushFront(const T &value) -> iterator
{
    try
    {
        head = std::make_shared<ListNode<T>>(value, head);
    }
    catch (std::bad_alloc e)
    {
        throw NodeAllocationException(__FILE_NAME__, typeid(*this).name(), __FUNCTION__, __LINE__);
    }
    return iterator(head);
}

template <MoveAndCopy T>
void List<T>::popFront() noexcept
{
    if (head != nullptr)
        head = head->next;
}

template <MoveAndCopy T>
void List<T>::popBack() noexcept
{
    if (head->next == nullptr)
    {
        head = nullptr;
        return;
    }

    auto node = head;
    while (node != nullptr && node->next != nullptr && node->next->next != nullptr)
        node = node->next;

    node->next = nullptr;
}

template <MoveAndCopy T>
void List<T>::erase(const iterator &iterator) noexcept
{
	if(iterator)
    	eraseNode(iterator.getPtr());
}

template <MoveAndCopy T>
void List<T>::eraseNode(std::shared_ptr<ListNode<T>> remove) noexcept
{
    if (remove == nullptr)
        return;

    // If removing the head node
    if (head == remove)
    {
        head = remove->next;
        return;
    }

    // Find the previous node
    auto node = head;
    while (node != nullptr)
    {
        if (node->next == remove)
        {
            node->next = remove->next;
            return;
        }
        node = node->next;
    }
}

template <MoveAndCopy T>
auto List<T>::pushBack(const T &value) -> iterator
{
    std::shared_ptr<ListNode<T>> node = nullptr;
    try
    {
        node = std::make_shared<ListNode<T>>(value, nullptr);
    }
    catch (std::bad_alloc e)
    {
        throw NodeAllocationException(__FILE_NAME__, typeid(*this).name(), __FUNCTION__, __LINE__);
    }

    auto tail = head;
    if (tail == nullptr)
    {
        head = node;
        return iterator(node);
    }

    while (tail->next != nullptr)
        tail = tail->next;

    tail->next = node;
    return iterator(node);
}

template <MoveAndCopy T>
auto List<T>::getSize() const noexcept -> size_type
{
    auto node = head;
    size_type size = 0;
    while (node != nullptr)
    {
        size++;
        node = node->next;
    }
    return size;
}

template <MoveAndCopy T>
void List<T>::clear() noexcept
{
    head = nullptr;
}

template <MoveAndCopy T>
bool List<T>::isEmpty() const
{
    return head == nullptr;
}

template <MoveAndCopy T>
auto List<T>::at(size_type index) -> reference
{

    auto node = head;
    while (index-- != 0 && node != nullptr)
        node = node->next;

    return node->value;
}

template <MoveAndCopy T>
auto List<T>::at(size_type index) const -> const_reference
{

    auto node = head;
    while (index-- != 0 && node != nullptr)
        node = node->next;

    return node->value;
}

template <MoveAndCopy T>
auto List<T>::operator[](size_type index) -> reference
{
    return at(index);
}

template <MoveAndCopy T>
auto List<T>::operator[](size_type index) const -> const_reference
{
    return at(index);
}

template <MoveAndCopy T>
auto List<T>::getFront() -> reference
{
    return head->value;
}

template <MoveAndCopy T>
auto List<T>::getFront() const -> const_reference
{
    return head->value;
}

template <MoveAndCopy T>
auto List<T>::getBack() -> reference
{
    auto node = head;
    while (node != nullptr && node->next != nullptr)
        node = node->next;

    return node->value;
}

template <MoveAndCopy T>
auto List<T>::getBack() const -> const_reference
{
    auto node = head;
    while (node != nullptr && node->next != nullptr)
        node = node->next;

    return node->value;
}

template <MoveAndCopy T>
void List<T>::resize(size_type size)
    requires DefaultConstructible<T>
{
    head = nullptr;
    std::ranges::for_each(std::views::iota(0, static_cast<int>(size)), [this](int) {
        pushFront(T());
    });
}

template <MoveAndCopy T>
void List<T>::resize(size_type size, const value_type &instance)
    requires CopyConstructible<T>
{
    head = nullptr;
    std::ranges::for_each(std::views::iota(0, static_cast<int>(size)), [this, &instance](int) {
        pushFront(T(instance));
    });
}

template <MoveAndCopy T>
List<T> &List<T>::operator=(const List<T> &list)
{
    head = nullptr;
    std::ranges::for_each(list, [this](const auto& it) {
        pushBack(it);
    });
    return *this;
}

template <MoveAndCopy T>
auto List<T>::begin() -> iterator
{
    return iterator(head);
}

template <MoveAndCopy T>
auto List<T>::end() -> iterator
{
    return iterator(nullptr);
}

template <MoveAndCopy T>
auto List<T>::begin() const -> const_iterator
{
    return const_iterator(head);
}

template <MoveAndCopy T>
auto List<T>::end() const -> const_iterator
{
    return const_iterator(nullptr);
}

template <MoveAndCopy T>
auto List<T>::cbegin() const -> const_iterator
{
    return const_iterator(head);
}

template <MoveAndCopy T>
auto List<T>::cend() const -> const_iterator
{
    return const_iterator(nullptr);
}


