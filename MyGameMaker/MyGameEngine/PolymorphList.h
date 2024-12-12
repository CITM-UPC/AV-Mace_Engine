#pragma once

#include <list>
#include <memory>

template <typename TParent>
class PolymorphList
{
	std::list<std::unique_ptr<TParent>> _list;

public:
	PolymorphList() = default;
	PolymorphList(const PolymorphList& other) = delete;
	PolymorphList& operator=(const PolymorphList& other) = delete;
	PolymorphList(PolymorphList&& other) = default;
	virtual ~PolymorphList() = default;

	template <typename TChild, typename ...Args>
	TChild& emplace(Args&&... args) {
		static_assert(std::is_base_of_v<TParent, TChild>, "TChild must be derived from TParent");
		auto& item = _list.emplace_back(std::make_unique<TChild>(std::forward<Args>(args)...));
		return static_cast<TChild&>(*item);
	}

	template <class TChild>
	bool hasType() const {
		for (const auto& item : _list) if (dynamic_cast<TChild*>(item.get())) return true;
		return false;
	}

	template <typename TChild>
	TChild& getType() {
		for (auto& item : _list) if (dynamic_cast<TChild*>(item.get()))	return static_cast<TChild&>(*item);
		throw std::out_of_range("Item not found");
	}

	template <typename TChild>
	const TChild& getType() const {
		for (const auto& item : _list)
			if (dynamic_cast<const TChild*>(item.get()))
				return static_cast<const TChild&>(*item);
		throw std::out_of_range("Item not found");
	}

	template <typename TChild>
	auto getAllOfType() {
		return _list
			| std::views::filter([](auto& uptr) { return dynamic_cast<TChild*>(uptr.get()); })
			| std::views::transform([](auto& uptr) -> TChild& { return static_cast<TChild&>(*uptr); });
	}

	template <class TChild>
	auto getAllOfType() const {
		return _list
			| std::views::filter([](const auto& uptr) { return dynamic_cast<const TChild*>(uptr.get()); })
			| std::views::transform([](const auto& uptr) -> const TChild& { return static_cast<const TChild&>(*uptr); });
	}

	auto& list() { return _list; }
	auto size() const { return _list.size(); }
	auto empty() const { return _list.empty(); }
	auto begin() { return _list.begin(); }
	auto end() { return _list.end(); }
};
