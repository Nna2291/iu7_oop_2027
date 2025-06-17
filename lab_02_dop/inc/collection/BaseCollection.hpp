#pragma once

#include "collection/BaseCollection.h"

template<typename Container>
BaseCollection<Container>::BaseCollection(const Container& container) : data(container)
{
}

template<typename Container>
BaseCollection<Container>::BaseCollection(Container&& container) : data(std::move(container))
{
}

template<typename Container>
template<typename... Args>
BaseCollection<Container>::BaseCollection(Args&&... args) : data(std::forward<Args>(args)...)
{
}

template<typename Container>
bool BaseCollection<Container>::isEmpty() const
{
    return data.empty();
}

template<typename Container>
typename BaseCollection<Container>::size_type BaseCollection<Container>::getSize() const
{
    return data.size();
}

template<typename Container>
void BaseCollection<Container>::clear()
{
    data.clear();
}

template<typename Container>
typename BaseCollection<Container>::iterator BaseCollection<Container>::begin()
{
    return data.begin();
}

template<typename Container>
typename BaseCollection<Container>::iterator BaseCollection<Container>::end()
{
    return data.end();
}

template<typename Container>
typename BaseCollection<Container>::const_iterator BaseCollection<Container>::begin() const
{
    return data.begin();
}

template<typename Container>
typename BaseCollection<Container>::const_iterator BaseCollection<Container>::end() const
{
    return data.end();
}

template<typename Container>
typename BaseCollection<Container>::const_iterator BaseCollection<Container>::cbegin() const
{
    return data.cbegin();
}

template<typename Container>
typename BaseCollection<Container>::const_iterator BaseCollection<Container>::cend() const
{
    return data.cend();
}

template<typename Container>
Container& BaseCollection<Container>::getContainer()
{
    return data;
}

template<typename Container>
const Container& BaseCollection<Container>::getContainer() const
{
    return data;
} 