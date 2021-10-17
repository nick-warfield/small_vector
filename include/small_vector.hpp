#pragma once
#include <cstdlib>
#include <stdexcept>

template <typename T, size_t N = 0>
class small_vector {
	size_t count = 0;
	T buffer[N];

	size_t heap_capacity = 0;
	size_t heap_size = 0;
	T* heap = new int[0];

	void realloc(size_t new_capacity) {
		heap_capacity = new_capacity;
		int* temp = new int[heap_capacity];
		for (size_t i = 0; i < heap_size; i++)
			temp[i] = std::move(heap[i]);
		delete[] heap;
		heap = temp;
		heap_size = std::min(heap_size, heap_capacity);
	}

public:
	~small_vector() {
		delete[] heap;
	}

	constexpr size_t size() const { return count; }
	constexpr size_t capacity() const { return N + heap_capacity; }
	constexpr bool is_empty() const { return count == 0; }

	T& operator[](const size_t& index) {
		if (index < N)
			return buffer[index];
		else
			return heap[index - N];
	}
	const T& operator[](const size_t& index) const {
		return this[index];
	}

	T& front() { return buffer[0]; }
	const T& front() const { return buffer[0]; }
	T& back() { return this[count - 1]; }
	const T& back() const { return this[count - 1]; }

	void pop() {
		if (count != 0)
			count--;
		if (heap_size != 0) {
			heap_size--;
		}
	}
	void push(T item) {
		if (count >= N) {
			if (heap_size == heap_capacity)
				realloc(heap_capacity * 2 + 1);
			heap[heap_size++] = item;
			count++;
		} else {
			buffer[count++] = item;
		}
	}
};
