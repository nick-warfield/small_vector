#pragma once
#include <cstdlib>
#include <stdexcept>

template <typename T, size_t N = 0>
class small_vector {
	size_t num_of_items = 0;
	T buffer[N];

	size_t heap_capacity = 0;
	T* heap = nullptr;

	void realloc();

public:
	constexpr size_t size() const { return num_of_items; }
	constexpr size_t capacity() const { return N + heap_capacity; }

	T& operator[](const size_t& index) { return buffer[index]; }
	const T& operator[](const size_t& index) const { return buffer[index]; }

	T pop() {
		num_of_items = num_of_items == 0 ? 0 : num_of_items - 1;
		return std::move(buffer[num_of_items]);
	}
	void push(T item) {
		buffer[num_of_items++] = item;
	}
};
