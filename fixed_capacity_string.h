#pragma once

#include <array>
#include <string_view>
#include <string>

#if __cplusplus < 201703L
#error "fixed_capacity_string requires atleast C++17"
#endif

#if __cplusplus >= 202002L
#define _CONSTEXPR_20 constexpr
#else
#define _CONSTEXPR_20
#endif


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
	static constexpr size_t npos = static_cast<size_t>(-1);

	static _CONSTEXPR_20 auto from_sized(const_pointer ptr, size_t count) -> fixed_capacity_string_base
	{
		fixed_capacity_string_base obj;
		if (ptr == nullptr) return obj;

		obj.mSize = count > _Capacity ? _Capacity : count;
		_Traits::copy(obj.mArray.data(), ptr, obj.mSize);
		_Traits::assign(obj.mArray[obj.mSize], _Elem());
		return obj;
	}

	static _CONSTEXPR_20 auto from_raw(const_pointer ptr) -> fixed_capacity_string_base
	{
		return from_sized(ptr, _Traits::length(ptr));
	}

	static _CONSTEXPR_20 auto from_sv(const std::basic_string_view<_Elem, _Traits>& sv)
		-> fixed_capacity_string_base
	{
		return from_sized(sv.data(), sv.size());
	}

	template<class _Alloc = std::allocator<_Elem>>
	static _CONSTEXPR_20 auto from_str(const std::basic_string<_Elem, _Traits, _Alloc>& str)
		-> fixed_capacity_string_base
	{
		return from_sized(str.data(), str.size());
	}

	constexpr auto c_str() const -> const_pointer { return mArray.data(); }

	constexpr auto sv() const -> std::basic_string_view<_Elem, _Traits>
	{
		return std::basic_string_view<_Elem, _Traits>(mArray.data(), mSize);
	}

	_CONSTEXPR_20 auto alloc_str() const -> std::basic_string<_Elem, _Traits>
	{
		return std::basic_string<_Elem, _Traits>(mArray.data(), mSize);
	}

	constexpr auto length() const -> size_t { return mSize; }
	constexpr auto size() const -> size_t { return mSize; }
	constexpr auto capacity() const -> size_t { return _Capacity; }

	_CONSTEXPR_20 auto assign(const_pointer ptr, size_t count) -> fixed_capacity_string_base&
	{
		mSize = count > _Capacity ? _Capacity : count;
		_Traits::copy(mArray.data(), ptr, mSize);
		_Traits::assign(mArray[mSize], _Elem());
		return *this;
	}

	_CONSTEXPR_20 auto assign(const_pointer ptr) -> fixed_capacity_string_base&
	{
		return assign(ptr, _Traits::length(ptr));
	}

	_CONSTEXPR_20 auto assign(const std::basic_string_view<_Elem, _Traits>& sv)
		-> fixed_capacity_string_base&
	{
		return assign(sv.data(), sv.size());
	}

	template<class _Alloc = std::allocator<_Elem>>
	_CONSTEXPR_20 auto assign(const std::basic_string<_Elem, _Traits, _Alloc>& str)
		-> fixed_capacity_string_base&
	{
		return assign(str.data(), str.size());
	}

	template<size_t _OtherCapacity>
	_CONSTEXPR_20 auto assign(fixed_capacity_string_base<_OtherCapacity, _Elem, _Traits>& other)
		-> fixed_capacity_string_base&
	{
		return assign(other.data(), other.size());
	}

	_CONSTEXPR_20 auto append(const_pointer ptr, size_t count) -> fixed_capacity_string_base&
	{
		size_t oldSize = mSize;
		mSize =
			_Capacity - mSize > count
			? mSize + count
			: _Capacity;
		_Traits::copy(mArray.data() + oldSize, ptr, mSize - oldSize);
		_Traits::assign(mArray[mSize], _Elem());
		return *this;
	}

	_CONSTEXPR_20 auto append(const_pointer ptr) -> fixed_capacity_string_base&
	{
		return append(ptr, _Traits::length(ptr));
	}

	_CONSTEXPR_20 auto append(const std::basic_string_view<_Elem, _Traits>& sv)
		-> fixed_capacity_string_base&
	{
		return append(sv.data(), sv.size());
	}

	template<class _Alloc = std::allocator<_Elem>>
	_CONSTEXPR_20 auto append(const std::basic_string<_Elem, _Traits, _Alloc>& str)
		-> fixed_capacity_string_base&
	{
		return append(str.data(), str.size());
	}

	template<size_t _OtherCapacity>
	_CONSTEXPR_20 auto append(fixed_capacity_string_base<_OtherCapacity, _Elem, _Traits>& other)
		-> fixed_capacity_string_base&
	{
		return append(other.data(), other.size());
	}

	_CONSTEXPR_20 auto insert(size_t index, const_pointer ptr, size_t count)
		-> fixed_capacity_string_base&
	{
		if (index >= mSize)
			return append(ptr, count);

		mSize =
			_Capacity - mSize > count
			? mSize + count
			: _Capacity;

		_Elem* const insertAt = mArray.data() + index;
		if (count < _Capacity - index)
		{
			// Inserted range will not exceed capacity.
			// So we need to move the suffix to the end.
			const size_t numElemsToMove = mSize - (index + count);
			_Traits::move(insertAt + count, insertAt, numElemsToMove);
		}

		const size_t numElemsToInsert = std::min(count, mSize - index);
		_Traits::copy(insertAt, ptr, numElemsToInsert);
		_Traits::assign(mArray[mSize], _Elem());
		return *this;
	}

	_CONSTEXPR_20 auto insert(size_t index, const_pointer ptr) -> fixed_capacity_string_base&
	{
		return insert(index, ptr, _Traits::length(ptr));
	}

	_CONSTEXPR_20 auto insert(size_t index, const std::basic_string_view<_Elem, _Traits>& sv)
		-> fixed_capacity_string_base&
	{
		return insert(index, sv.data(), sv.size());
	}

	template<class _Alloc = std::allocator<_Elem>>
	_CONSTEXPR_20 auto insert(size_t index, const std::basic_string<_Elem, _Traits, _Alloc>& str)
		-> fixed_capacity_string_base&
	{
		return insert(index, str.data(), str.size());
	}

	template<size_t _OtherCapacity>
	_CONSTEXPR_20 auto insert(size_t index,
				fixed_capacity_string_base<_OtherCapacity, _Elem, _Traits>& other)
				-> fixed_capacity_string_base&
	{
		return insert(index, other.data(), other.size());
	}

	_CONSTEXPR_20 auto insert(size_t index, size_t count, _Elem elem)
		-> fixed_capacity_string_base&
	{
		const bool insertAtEnd = index >= mSize;
		const size_t realIndex = insertAtEnd ? mSize : index;

		mSize =
			_Capacity - mSize > count
			? mSize + count
			: _Capacity;

		_Elem* const insertAt = mArray.data() + realIndex;
		if (!insertAtEnd &&
			count < _Capacity - realIndex)
		{
			// We are inserting in the middle of the string, and
			// the inserted range will not exceed capacity.
			// So we need to move the suffix to the end.
			const size_t numElemsToMove = mSize - (realIndex + count);
			_Traits::move(insertAt + count, insertAt, numElemsToMove);
		}

		const size_t numElemsToInsert = std::min(count, mSize - realIndex);
		_Traits::assign(insertAt, numElemsToInsert, elem);
		_Traits::assign(mArray[mSize], _Elem());
		return *this;
	}

	_CONSTEXPR_20 auto clear() -> void
	{
		mSize = 0;
		_Traits::assign(mArray[mSize], _Elem());
	}

	_CONSTEXPR_20 auto erase(size_t index = 0, size_t count = npos)
		-> fixed_capacity_string_base&
	{
		if (index >= mSize)
			return *this;

		if (count >= mSize - index)
		{
			// Erase till the end. Just add a null-terminator.
			_Traits::assign(mArray[index], _Elem());
			mSize = index;
			return *this;
		}

		// There is a suffix remaining. Move it up front.
		_Elem* const eraseAt = mArray.data() + index;
		const size_t numElemsToMove = mSize - (index + count) + 1; // +1 to include the null-terminator.
		_Traits::move(eraseAt, eraseAt + count, numElemsToMove);
		mSize -= count;
		return *this;
	}

	_CONSTEXPR_20 auto replace(size_t index, size_t count, const_pointer ptr, size_t count2)
		-> fixed_capacity_string_base&
	{
		if (index >= mSize)
			return append(ptr, count2);

		if (count > mSize - index)
			count = mSize - index;

		_Elem* const replaceAt = mArray.data() + index;

		if (count == count2)
		{
			_Traits::copy(replaceAt, ptr, count2);
			return *this;
		}

		mSize = std::min(mSize + (count2 - count), _Capacity);
		
		if (count2 < _Capacity - index)
		{
			// Replaced range will not exceed capacity. So we need to move the suffix.
			const size_t numElemsToMove = mSize - (index + count2);
			_Traits::move(replaceAt + count2, replaceAt + count, numElemsToMove);
		}

		const size_t numElemsToInsert = std::min(count2, mSize - index);
		_Traits::copy(replaceAt, ptr, numElemsToInsert);
		_Traits::assign(mArray[mSize], _Elem());
		return *this;
	}

	_CONSTEXPR_20 auto replace(size_t index, size_t count, const_pointer ptr)
		-> fixed_capacity_string_base&
	{
		return replace(index, count, ptr, _Traits::length(ptr));
	}

	_CONSTEXPR_20 auto replace(size_t index, size_t count, const std::basic_string_view<_Elem, _Traits>& sv)
		-> fixed_capacity_string_base&
	{
		return replace(index, count, sv.data(), sv.size());
	}

	template<class _Alloc = std::allocator<_Elem>>
	_CONSTEXPR_20 auto replace(size_t index, size_t count,
				 const std::basic_string<_Elem, _Traits, _Alloc>& str)
				 -> fixed_capacity_string_base&
	{
		return replace(index, count, str.data(), str.size());
	}

	template<size_t _OtherCapacity>
	_CONSTEXPR_20 auto replace(size_t index, size_t count,
		fixed_capacity_string_base<_OtherCapacity, _Elem, _Traits>& other)
		-> fixed_capacity_string_base&
	{
		return replace(index, count, other.data(), other.size());
	}

	_CONSTEXPR_20 auto swap(fixed_capacity_string_base& other) -> void
	{
		mArray.swap(other.mArray);
		std::swap(mSize, other.mSize);
	}

	constexpr fixed_capacity_string_base() = default;
	constexpr fixed_capacity_string_base(fixed_capacity_string_base& other) = default;
	constexpr fixed_capacity_string_base(fixed_capacity_string_base&& other) = default;
	constexpr auto operator=(fixed_capacity_string_base& other)->fixed_capacity_string_base & = default;
	constexpr auto operator=(fixed_capacity_string_base&& other)->fixed_capacity_string_base & = default;

	constexpr auto data() -> pointer
	{
		return mArray.data();
	}

private:
	size_t mSize{};
	std::array<_Elem, _Capacity + 1> mArray{};
};

template<size_t _Capacity>
using fixed_capacity_string = fixed_capacity_string_base<_Capacity, char>;

#if __cpp_lib_char8_t
template<size_t _Capacity>
using fixed_capacity_u8string = fixed_capacity_string_base<_Capacity, char8_t>;
#endif //__cpp_char8_t

template<size_t _Capacity>
using fixed_capacity_u16string = fixed_capacity_string_base<_Capacity, char16_t>;
template<size_t _Capacity>
using fixed_capacity_u32string = fixed_capacity_string_base<_Capacity, char32_t>;
template<size_t _Capacity>
using fixed_capacity_wstring = fixed_capacity_string_base<_Capacity, wchar_t>;

