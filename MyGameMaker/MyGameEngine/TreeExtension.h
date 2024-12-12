#pragma once

#include <list>
#include <memory>
#include <ranges>

template <class T>
class TreeExtension
{
private:
    T* _parent = nullptr;
    std::list<std::shared_ptr<T>> _children;
    std::ranges::ref_view<std::list<T>> _children_view{ _children };

public:
    TreeExtension() = default;
    TreeExtension(const TreeExtension& other) : _children(other._children) { for (auto& child : _children) child._parent = static_cast<T*>(this); }
    TreeExtension& operator=(const TreeExtension& other) {
        _children = other._children;
        for (auto& child : _children) child._parent = static_cast<T*>(this);
        return *this;
    }
    TreeExtension(TreeExtension&& other)  noexcept : _parent(other._parent), _children(std::move(other._children)) {
        for (auto& child : _children) {
            child._parent = static_cast<T*>(this);
        }
        other._parent = nullptr;
    }
    TreeExtension& operator=(TreeExtension&& other) noexcept {
        _children = std::move(other._children);
        for (auto& child : _children) child._parent = static_cast<T*>(this);
        return *this;
    }
    virtual ~TreeExtension() = default;

    bool isRoot() const { return !hasParent(); }
    bool hasParent() const { return _parent != nullptr; }
    T* parent() { return _parent; }
    const T* parent() const { return _parent; }
    bool hasParent() const { return _parent != nullptr; }
	auto& setParent(T* parent) { _parent = parent; return *static_cast<T*>(this); }
    
    const auto& children() const { return _children; }
    auto& children() { return _children_view; }

    template <typename ...Args>
    auto& emplaceChild(Args&&... args) {
        auto& child = _children.emplace_back(std::forward<Args>(args)...);
        child._parent = static_cast<T*>(this);
        return child;
    }

    void removeChild(const T& child) { return _children.remove(std::forward(child)); }

    // Get shared pointer to this object from a child's list
    std::shared_ptr<T> getSharedFromChildren(T* ptr) {
        for (const auto& child : _children) {
            if (child.get() == ptr) {
                return child;
            }
        }
        return nullptr;
    }
    // Unparent this node (removes from parent's children)
    bool unparent() {
        if (_parent) {
            // Find our shared_ptr in parent's children
            auto selfPtr = _parent->getSharedFromChildren(static_cast<T*>(this));
            if (selfPtr) {
                _parent->_children.remove(selfPtr);
                _parent = nullptr;
                return true;
            }
        }
        return false;
    }
    // Reparent this node to a new parent
    bool reparent(T* newParent) {
        if (!newParent || newParent == this) {
            return false;
        }

        // Check for circular reference
        T* ancestor = newParent;
        while (ancestor) {
            if (ancestor == this) {
                return false;
            }
            ancestor = ancestor->_parent;
        }

        if (newParent != _parent) {
            // Get our shared_ptr from current parent
            std::shared_ptr<T> selfPtr;
            if (_parent) {
                selfPtr = _parent->getSharedFromChildren(static_cast<T*>(this));
                if (!selfPtr) {
                    return false;
                }
                _parent->_children.remove(selfPtr);
            }

            // Set new parent
            _parent = newParent;
            if (selfPtr) {
                newParent->_children.push_back(selfPtr);
            }
            return true;
        }
        return false;
    }
    // Check if this node is a descendant of potential ancestor
    bool isDescendantOf(const T* potentialAncestor) const {
        if (!_parent) return false;
        if (_parent == potentialAncestor) return true;
        return _parent->isDescendantOf(potentialAncestor);
    }
};