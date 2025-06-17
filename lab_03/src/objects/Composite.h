#pragma once
#include "Object.h"
#include <memory>

class Composite : public Object {
public:
    using value_type = Object;
    using iterator = std::vector<std::shared_ptr<value_type>>::iterator;
    using const_iterator = std::vector<std::shared_ptr<value_type>>::const_iterator;
    using reverse_iterator = std::vector<std::shared_ptr<value_type>>::reverse_iterator;
    using const_reverse_iterator = std::vector<std::shared_ptr<value_type>>::const_reverse_iterator;
    using size_type = std::vector<std::shared_ptr<value_type>>::size_type;
    using difference_type = std::vector<std::shared_ptr<value_type>>::difference_type;

    Composite() = default;
    Composite(std::vector<std::shared_ptr<Object>> objects);

    ~Composite() = default;
    bool IsComposite() override { return true; };

    std::shared_ptr<Object> Clone() const override;
    bool Add(const std::shared_ptr<Object> object) override;
    bool Remove(const iterator &it) override;

    void Transform(const TransformAction& transform) override;
    void accept(const Visitor &visitor) override;

    bool IsVisible() const override;
    Point GetCenter() const override;
    iterator begin() override;
    iterator end() override;

protected:
    std::vector<std::shared_ptr<Object>> _objects;
};
