#ifndef __SET_H
#define __SET_H

#include <stdarg.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <ostream>

#include "iterator_concepts.h"
#include "base_set.h"
#include "set_const_iterator.h"
#include "set_concepts.h"
#include "set_exception.h"

template <SetType T>
class Set final : public BaseSet
{
public:
    using iterator = SetConstIterator<T>;
    using const_iterator = SetConstIterator<T>;
    using value_type = T;
    friend const_iterator;

public:
    Set();
    
    Set(const Set<T>& s);
    Set<T>& operator=(const Set<T>& set);

    Set(Set<T>&& s) noexcept;
    Set<T>& operator=(Set<T>&& set) noexcept;

    template <SetType U>
        requires Convertable<T, U>
    explicit Set(const Set<U>& s);

    template <SetType U>
        requires Convertable<T, U>
    Set<T>& operator=(const Set<U>& s);

    template <typename I>
        requires IteratorConcepts::ForwardIterator<I> && 
                 std::convertible_to<typename I::value_type, T>
    Set(const I& begin, const I& end);

    template <typename I>
        requires IteratorConcepts::ForwardIterator<I> && 
                 std::convertible_to<typename I::value_type, T>
    Set(const I& begin, const size_type& num);

    template <class C>  
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    explicit Set(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T>& operator=(const C& container);

    Set(size_type array_len, const T* array);

    template <SetType U>
        requires Convertable<T, U>
    Set(size_type array_len, const U* array);

    Set(std::initializer_list<T> args);
    Set<T>& operator=(std::initializer_list<T> args);

    ~Set();

    void add(const T& element);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    void add(const C& container);

    template <SetType U>
        requires Convertable<T, U>
    Set<T> operator+(const U& element) const;

    template <SetType U>
        requires Convertable<T, U>
    friend Set<T> operator+(const U& element, Set<T>& t);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T> operator+(const C& container) const;

    template <SetType U>
        requires Convertable<T, U>
    Set<T>& operator+=(const U& element);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T>& operator+=(const C& container);

    void remove(const T &element);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    void remove(const C& container);

    template <SetType U>
        requires Convertable<T, U>
    Set<T> operator-(const U& element) const;

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T> operator-(const C& container) const;
    
    template <SetType U>
        requires Convertable<T, U>
    Set<T>& operator-=(const U& element);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T>& operator-=(const C& container);

    void clear();
    void update();

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T> intersect(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T>& intersect_update(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T> operator&(const C& container) const;

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T>& operator&=(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T> unite(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T>& unite_update(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T> difference(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T>& difference_update(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T> symmetric_difference(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T>& symmetric_difference_update(const C& container);

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T> operator^(const C& container) const;

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    Set<T>& operator^=(const C& container);

    operator bool() const noexcept;

    template <class C>
        requires Container<C> && ConvertableTo<typename C::value_type, T>
    bool operator==(const C& container) const;

    virtual size_type size() const noexcept override;
    virtual bool isEmpty() const noexcept override;

    const_iterator begin() const;
    const_iterator end() const;

    template <SetType U>
        requires equalTo<T, U>
    bool has(U value) const;

protected:
    class Node
    {
        friend Set<T>::iterator;
        friend Set<T>::const_iterator;

    public:
        Node();
        explicit Node(const T& value);
        explicit Node(const Node& node);
        explicit Node(Node&& node);
        explicit Node(std::shared_ptr<Node>& node);

        std::weak_ptr<Node> get_next();
        void setValue(const T& value);
        void setNext(std::shared_ptr<Node>& node);
        void setNextNull();
        
        const T& getValue() const;

        bool operator==(const Node& node) const;
        bool operator!=(const Node& node) const;

        bool operator==(const std::shared_ptr<Node>& node) const;
        bool operator!=(const std::shared_ptr<Node>& node) const;

        ~Node() = default;

    private:
        T value;
        std::shared_ptr<Node> next;
    };

private:
    std::shared_ptr<Node> head;
};

template <SetType T>
std::ostream& operator<<(std::ostream& os, const Set<T>& set);

#include "set_node.hpp"
#include "set.hpp"

#endif