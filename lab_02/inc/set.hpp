#ifndef _SET_HPP
#define _SET_HPP

#include "set.h"
#include <ranges>
#include <algorithm>

// Конструкторы
template <SetType T>
Set<T>::Set()
{
    head = nullptr;
    len = 0;
}

template <SetType T>
Set<T>::Set(const Set<T> &s)
{
    clear();
    std::ranges::for_each(s, [this](const auto &element) { add(element); });
}

template <SetType T>
template <SetType U>
    requires Convertable<T, U>
Set<T>::Set(const Set<U> &s)
{
    clear();
    std::ranges::for_each(s, [this](const auto &element) { add(element); });
}

template <SetType T>
Set<T>::Set(Set<T> &&s) noexcept
{
    head = s.head;
    len = s.len;
    s.clear();
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T>::Set(const C &container)
{
    clear();
    std::ranges::for_each(container, [this](const auto &element) { add(element); });
}

template <SetType T>
template <typename I>
    requires IteratorConcepts::ForwardIterator<I> && std::convertible_to<typename I::value_type, T>
Set<T>::Set(const I &begin, const I &end)
{
    clear();
    std::ranges::for_each(begin, end, [this](const auto &element) { add(element); });
}

template <SetType T>
template <typename I>
    requires IteratorConcepts::ForwardIterator<I> && std::convertible_to<typename I::value_type, T>
Set<T>::Set(const I &begin, const size_type &num)
{
    clear();
    std::ranges::for_each(begin, begin + num, [this](const auto &element) { add(element); });
}

template <SetType T>
Set<T>::Set(size_type values_len, const T *values_array)
{
    clear();
    std::ranges::for_each(values_array, values_array + values_len, [this](const auto &element) { add(element); });
}

template <SetType T>
template <SetType U>
    requires Convertable<T, U>
Set<T>::Set(size_type values_len, const U *values_array)
{
    clear();
    std::ranges::for_each(values_array, values_array + values_len, [this](const auto &element) { add(element); });
}

template <SetType T>
Set<T>::Set(std::initializer_list<T> args)
{
    clear();
    std::ranges::for_each(args, [this](const auto &element) { add(element); });
}

// Деструкторы

template <SetType T>
Set<T>::~Set()
{
    if (head)
    {
        head.reset();
    }
}

// Булевые операторы

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
bool Set<T>::operator==(const C &container) const
{
    return std::ranges::all_of(container, [this](const auto &e) { return has(e); });
}

template <SetType T>
Set<T>::operator bool() const noexcept
{
    return len != 0;
}

template <SetType T>
Set<T> &Set<T>::operator=(const Set<T> &set)
{
    clear();
    std::ranges::for_each(set, [this](const auto &e) { add(e); });
    return *this;
}

template <SetType T>
template <SetType U>
    requires Convertable<T, U>
Set<T> &Set<T>::operator=(const Set<U> &set)
{
    *this = Set<T>(set);
    return *this;
}

template <SetType T>
Set<T> &Set<T>::operator=(Set<T> &&set) noexcept
{
    head = set.head;
    len = set.len;
    set.clear();
    return *this;
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> &Set<T>::operator=(const C &container)
{
    *this = Set<T>(container);
    return *this;
}

template <SetType T>
Set<T> &Set<T>::operator=(std::initializer_list<T> args)
{
    *this = Set<T>(args);
    return *this;
}

template <SetType T>
template <SetType U>
    requires Convertable<T, U>
Set<T> Set<T>::operator+(const U &element) const
{
    Set<T> new_set(*this);
    new_set += element;
    return new_set;
}

template <SetType T, SetType U>
    requires Convertable<T, U>
Set<T> operator+(const U &element, Set<T> &t)
{
    return t + element;
}

template <SetType T>
template <SetType U>
    requires Convertable<T, U>
Set<T> &Set<T>::operator+=(const U &element)
{
    add(element);
    return *this;
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> Set<T>::operator+(const C &container) const
{
    Set<T> new_set(*this);
    new_set += container;
    return new_set;
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> &Set<T>::operator+=(const C &container)
{
    add(container);
    return *this;
}

template <SetType T>
Set<T> operator+(std::initializer_list<T> args, Set<T> &set)
{
    return set + args;
}

template <SetType T>
template <SetType U>
    requires Convertable<T, U>
Set<T> Set<T>::operator-(const U &element) const
{
    Set<T> new_set(*this);
    new_set -= element;
    return new_set;
}

template <SetType T, SetType U>
    requires Convertable<T, U>
Set<T> operator-(const U &element, Set<T> &t)
{
    return t - element;
}

template <SetType T>
template <SetType U>
    requires Convertable<T, U>
Set<T> &Set<T>::operator-=(const U &element)
{
    remove(element);
    return *this;
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> Set<T>::operator-(const C &container) const
{
    Set<T> new_set(*this);
    new_set -= container;
    return new_set;
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> &Set<T>::operator-=(const C &container)
{
    remove(container);
    return *this;
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> Set<T>::operator&(const C &container) const
{
    Set<T> new_set(*this);
    new_set &= container;
    return Set(new_set);
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> &Set<T>::operator&=(const C &container)
{
    intersect_update(container);
    return *this;
}

// xor

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> Set<T>::operator^(const C &container) const
{
    Set<T> new_set(*this);
    new_set ^= container;
    return Set(new_set);
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> &Set<T>::operator^=(const C &container)
{
    symmetric_difference_update(container);
    return *this;
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> Set<T>::symmetric_difference(const C &container)
{
    Set<T> res(*this);
    res.symmetric_difference_update(container);
    return Set(res);
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> &Set<T>::symmetric_difference_update(const C &container)
{
    auto tmp_set = intersect(container);
    remove(container);
    std::ranges::for_each(container, [this, &tmp_set](const auto& e) {
        if (!tmp_set.has(e))
            add(e);
    });
    return *this;
}

// добавление

template <SetType T>
void Set<T>::add(const T &element)
{
    time_t cur_time = time(NULL);
    
    if (has(element))
        return;
    try
    {
        auto old_head = head;
        head = std::make_shared<Node>(element);
        head->setNext(old_head);
        ++len;
    }
    catch (const std::bad_alloc &e)
    {
        throw ErrorSetAllocate(__FILE__,
                               typeid(*this).name(),
                               __LINE__,
                               ctime(&cur_time));
    }
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
void Set<T>::add(const C &container)
{
    std::ranges::for_each(container, [this](const auto& e) { add(e); });
}

// наличие 

template <SetType T>
template <SetType U>
    requires equalTo<T, U>
bool Set<T>::has(U value) const
{
    return std::ranges::any_of(*this, [value](const auto& v) { return v == value; });
}

// пересечение

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> Set<T>::intersect(const C &container)
{
    auto res = Set<T>();
    std::ranges::for_each(container, [this, &res](const auto& e) {
        if (has(e))
            res.add(e);
    });
    return Set(res);
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> &Set<T>::intersect_update(const C &container)
{
    Set<T> tmp_set;
    std::ranges::for_each(container, [this, &tmp_set](const auto& e) {
        if (has(e))
            tmp_set.add(e);
    });
    *this = tmp_set;
    return *this;
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> Set<T>::unite(const C &container)
{
    Set<T> res(*this);
    res.unite_update(container);
    return Set(res);
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> &Set<T>::unite_update(const C &container)
{
    std::ranges::for_each(container, [this](const auto& e) { add(e); });
    return *this;
}

// вычитание

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> Set<T>::difference(const C &container)
{
    Set<T> res(*this);
    res.difference_update(container);
    return Set(res);
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
Set<T> &Set<T>::difference_update(const C &container)
{
    remove(container);
    return *this;
}

template <SetType T>
template <class C>
    requires Container<C> && ConvertableTo<typename C::value_type, T>
void Set<T>::remove(const C &container)
{
    std::ranges::for_each(container, [this](const auto& e) { remove(e); });
}

template <SetType T>
void Set<T>::remove(const T &element)
{
    if (!has(element))
        return;

    for (auto n = head; n != nullptr && n->get_next().lock() != nullptr; n = n->get_next().lock())
    {
        if (n->get_next().lock()->getValue() == element)
        {
            auto next = n->get_next().lock()->get_next().lock();
            n->setNext(next);
        }
    }
}

template <SetType T>
void Set<T>::clear()
{
    head.reset();
    len = 0;
}

template <SetType T>
void Set<T>::update()
{
    Set<T> tmp;
    std::ranges::for_each(*this, [&tmp](const auto& e) { tmp.add(e); });
    *this = tmp;
}

template <SetType T>
Set<T>::const_iterator Set<T>::begin() const
{
    return const_iterator(head);
};

template <SetType T>
Set<T>::const_iterator Set<T>::end() const
{
    return const_iterator(nullptr, size());
};

template <SetType T>
Set<T>::size_type Set<T>::size() const noexcept
{
    return len;
}

template <SetType T>
bool Set<T>::isEmpty() const noexcept
{
    return (len <= 0);
}

template <SetType T>
std::ostream &operator<<(std::ostream &os, const Set<T> &set)
{
    os << '{';

    auto range = set | std::views::transform([](const auto &e) {
        return std::to_string(e) + ", ";
    });

    std::ranges::copy(range, std::ostream_iterator<std::string>(os));
    
    os << "}\n";
    return os;
}
#endif