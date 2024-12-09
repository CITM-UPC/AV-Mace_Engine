#pragma once

#include <list>
#include <memory>
#include <ranges>

template <class T>
class TreeExtension
{
private:
    T* _parent{ nullptr };
    std::list<T> _children;
    std::ranges::ref_view<std::list<T>> _children_view{ _children };

public:
	TreeExtension() = default;
	TreeExtension(const TreeExtension& other) : _children(other._children) { for (auto& child : _children) child._parent = static_cast<T*>(this);  }
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
    const auto& parent() const { return _parent; }
    auto& parent() { return *_parent; }
	void SetParent(T* parent) { _parent = parent; }

    const auto& children() const { return _children; }
    auto& children() { return _children_view; }

    template <typename ...Args>
    auto& emplaceChild(Args&&... args) {
        auto& child = _children.emplace_back(std::forward<Args>(args)...);
        child._parent = static_cast<T*>(this);
        return child;
    }

    void removeChild(const T& child) { return _children.remove(std::forward(child)); }
};