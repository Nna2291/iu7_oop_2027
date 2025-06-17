#pragma once

#include <concepts>

template <typename T, typename K>
concept HashFunction = requires(T a, const K& b) {
	requires std::is_invocable_v<T, const K&>;
    { a(b) } -> std::convertible_to<std::size_t>;
};

template <typename T, typename K>
concept EqualFunction = requires (T a, const K &b, const K &c) { 
	requires std::is_invocable_v<T, const K&, const K&>;
	{ a(b, c) } -> std::same_as<bool>;
};
