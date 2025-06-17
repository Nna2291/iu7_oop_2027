#ifndef __ITERATOR_CONCEPTS_H__
#define __ITERATOR_CONCEPTS_H__

#include <concepts>
#include <iterator>

namespace IteratorConcepts
{
    template <typename I>
    concept Iterator = requires() {
        typename I::value_type;
        typename I::difference_type;
        typename I::pointer;
        typename I::reference;
    };

    template <typename T, typename U>
    concept DerivedFrom = std::is_base_of<U, T>::value;

    template <typename T>
    concept equalTo = requires(T a, T b) {
        { a == b } -> std::same_as<bool>;
        { a != b } -> std::same_as<bool>;
        { b == a } -> std::same_as<bool>;
        { b != a } -> std::same_as<bool>;
    };

    template <typename I>
    concept InputIterator = Iterator<I> &&
                            requires { typename I::iterator_category; } &&
                            equalTo<I> &&
                            DerivedFrom<typename I::iterator_category, std::input_iterator_tag>;

    template <typename I>
    concept Incrementable = requires(I it) {
        { ++it } -> std::same_as<I &>;
        { it++ } -> std::same_as<I>;
    };

    template <typename I>
    concept ForwardIterator = InputIterator<I> &&
                              Incrementable<I> &&
                              DerivedFrom<typename I::iterator_category, std::forward_iterator_tag>;

};

#endif
