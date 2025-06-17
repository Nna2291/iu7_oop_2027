#include "CareTaker.h"

void CareTaker::SaveState(size_t objectId, std::shared_ptr<Memento> memento) {
    if (memento) {
        _mementos[objectId].push_back(memento);
    }
}

std::shared_ptr<Memento> CareTaker::GetLastState(size_t objectId) {
    auto it = _mementos.find(objectId);
    if (it != _mementos.end() && !it->second.empty()) {
        auto memento = it->second.back();
        it->second.pop_back();
        return memento;
    }
    return nullptr;
}

void CareTaker::ClearHistory(size_t objectId) {
    _mementos.erase(objectId);
}

void CareTaker::ClearAllHistory() {
    _mementos.clear();
} 