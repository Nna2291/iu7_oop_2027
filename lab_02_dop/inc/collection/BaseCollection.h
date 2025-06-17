#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

template<typename Container>
class BaseCollection
{
  public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using difference_type = typename Container::difference_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using pointer = typename Container::pointer;
    using const_pointer = typename Container::const_pointer;
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;

    BaseCollection() = default;
    explicit BaseCollection(const Container& container);
    explicit BaseCollection(Container&& container);
    
    template<typename... Args>
    explicit BaseCollection(Args&&... args);

    virtual ~BaseCollection() = default;

    bool isEmpty() const;
    size_type getSize() const;
    void clear();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    Container& getContainer();
    const Container& getContainer() const;

  protected:
    Container data;
};

// Для совместимости с существующим кодом, создадим базовый класс без шаблонов
class BaseCollectionInterface
{
  public:
    using size_type = std::size_t;

    BaseCollectionInterface() = default;
    virtual ~BaseCollectionInterface() = default;

    virtual bool isEmpty() const = 0;
    virtual size_type getSize() const = 0;
    virtual void clear() = 0;
};

#include "collection/BaseCollection.hpp"
