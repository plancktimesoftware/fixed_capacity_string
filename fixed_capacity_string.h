#pragma once

#include <array>
#include <string_view>
#include <string>

template<size_t _Capacity, class _Elem, class _Traits = std::char_traits<_Elem>>
class fixed_capacity_string_base
{
public:
	using traits_type = _Traits;
	using value_type = _Elem;
	using pointer = _Elem*;
	using const_pointer = const _Elem*;
	using reference = _Elem&;
	using const_reference = const _Elem&;
	using size_type = size_t;

	static auto from_sized(const_pointer ptr, size_type size) -> fixed_capacity_string_base
	{
		fixed_capacity_string_base obj;
		if (ptr == nullptr) return obj;

		obj.mSize = size > _Capacity ? _Capacity : size;
		_Traits::copy(obj.mArray.data(), ptr, obj.mSize);
		_Traits::assign(obj.mArray[obj.mSize], _Elem());
		return obj;
	}

	static auto from_raw(const_pointer ptr) -> fixed_capacity_string_base
	{
		return from_sized(ptr, _Traits::length(ptr));
	}

	static auto from_sv(const std::basic_string_view<_Elem, _Traits>& sv)
		-> fixed_capacity_string_base
	{
		return from_sized(sv.data(), sv.size());
	}

	static auto from_str(const std::basic_string<_Elem, _Traits>& str)
		-> fixed_capacity_string_base
	{
		return from_sized(str.data(), str.size());
	}

	auto c_str() const -> const_pointer { return mArray.data(); }

	auto sv() const -> std::basic_string_view<_Elem, _Traits>
	{
		return std::basic_string_view<_Elem, _Traits>(mArray.data(), mSize);
	}

	auto alloc_str() const -> std::basic_string<_Elem, _Traits>
	{
		return std::basic_string<_Elem, _Traits>(mArray.data(), mSize);
	}

	auto length() const -> size_type { return mSize; }
	auto size() const -> size_type { return mSize; }
	auto capacity() const -> size_type { return _Capacity; }

	auto assign(const_pointer ptr, size_type size) -> fixed_capacity_string_base&
	{
		mSize = size > _Capacity ? _Capacity : size;
		_Traits::copy(mArray.data(), ptr, mSize);
		_Traits::assign(mArray[mSize], _Elem());
		return *this;
	}

	auto assign(const_pointer ptr) -> fixed_capacity_string_base&
	{
		return assign(ptr, _Traits::length(ptr));
	}

	auto assign(const std::basic_string_view<_Elem, _Traits>& sv)
		-> fixed_capacity_string_base&
	{
		return assign(sv.data(), sv.size());
	}

	auto assign(const std::basic_string<_Elem, _Traits>& str)
		-> fixed_capacity_string_base&
	{
		return assign(str.data(), str.size());
	}

	template<size_t _OtherCapacity>
	auto assign(fixed_capacity_string_base<_OtherCapacity, _Elem, _Traits>& other)
		-> fixed_capacity_string_base&
	{
		return assign(other.data(), other.size());
	}

	auto append(const_pointer ptr, size_type size) -> fixed_capacity_string_base&
	{
		size_t oldSize = mSize;
		mSize =
			_Capacity - mSize > size
			? mSize + size
			: _Capacity;
		_Traits::copy(mArray.data() + oldSize, ptr, mSize - oldSize);
		_Traits::assign(mArray[mSize], _Elem());
		return *this;
	}

	auto append(const_pointer ptr) -> fixed_capacity_string_base&
	{
		return append(ptr, _Traits::length(ptr));
	}

	auto append(const std::basic_string_view<_Elem, _Traits>& sv)
		-> fixed_capacity_string_base&
	{
		return append(sv.data(), sv.size());
	}

	auto append(const std::basic_string<_Elem, _Traits>& str)
		-> fixed_capacity_string_base&
	{
		return append(str.data(), str.size());
	}

	template<size_t _OtherCapacity>
	auto append(fixed_capacity_string_base<_OtherCapacity, _Elem, _Traits>& other)
		-> fixed_capacity_string_base&
	{
		return append(other.data(), other.size());
	}

	fixed_capacity_string_base() = default;
	fixed_capacity_string_base(fixed_capacity_string_base& other) = default;
	fixed_capacity_string_base(fixed_capacity_string_base&& other) = default;
	auto operator=(fixed_capacity_string_base& other)->fixed_capacity_string_base & = default;
	auto operator=(fixed_capacity_string_base&& other)->fixed_capacity_string_base & = default;

	auto data() -> pointer
	{
		return mArray.data();
	}

private:
	size_type mSize{};
	std::array<_Elem, _Capacity + 1> mArray{};
};

template<size_t _Capacity>
using fixed_capacity_string = fixed_capacity_string_base<_Capacity, char>;
