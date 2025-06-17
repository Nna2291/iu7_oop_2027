#pragma once
#include "Memento.h"
#include <memory>
#include <map>
#include <vector>

class CareTaker {
public:
    CareTaker() = default;
    ~CareTaker() = default;

    void SaveState(size_t objectId, std::shared_ptr<Memento> memento);
    std::shared_ptr<Memento> GetLastState(size_t objectId);
    void ClearHistory(size_t objectId);
    void ClearAllHistory();

private:
    std::map<size_t, std::vector<std::shared_ptr<Memento>>> _mementos;
}; 