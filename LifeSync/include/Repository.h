#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
#include <memory>
#include <algorithm>
#include "Exceptions.h"

// OOP Concept: Template (Generic Programming) - Repository<T>
// Generic template storage: add(), findById(), remove()
template <typename T>
class Repository {
private:
    std::vector<T> items;

public:
    // Add an item to the repository
    void add(const T& item) {
        items.push_back(item);
    }

    // Return const reference to all items
    const std::vector<T>& getAll() const {
        return items;
    }

    // Return mutable reference (for updates)
    std::vector<T>& getAll() {
        return items;
    }

    // Find item by id (T must have getId())
    T* findById(int id) {
        for (auto& item : items) {
            if (item.getId() == id) return &item;
        }
        return nullptr;
    }

    // Remove item by id; throws NotFoundException if not present
    void remove(int id) {
        auto it = std::find_if(items.begin(), items.end(),
            [id](const T& item) { return item.getId() == id; });
        if (it == items.end()) {
            throw NotFoundException("ID " + std::to_string(id));
        }
        items.erase(it);
    }

    bool empty() const { return items.empty(); }
    size_t size() const { return items.size(); }

    void clear() { items.clear(); }
};

#endif // REPOSITORY_H
