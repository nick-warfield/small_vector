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
		heap_size = std::min(heap_size, new_capacity);
		heap_capacity = new_capacity;
		int* temp = new int[heap_capacity];
		for (size_t i = 0; i < heap_size; i++)
			temp[i] = std::move(heap[i]);
		delete[] heap;
		heap = temp;
	}

public:
	~small_vector() {
		delete[] heap;
	}

	struct Iterator {
		Iterator(size_t index, small_vector<T, N> * ptr) :
			m_index(index), m_ptr(ptr) { }

		int index() const { return m_index; }
		T& operator*() { return m_ptr->operator[](m_index); }

		bool operator==(const Iterator& other) const {
			return m_index == other.m_index && m_ptr == other.m_ptr;
		}
		bool operator!=(const Iterator& other) const {
			return m_index != other.m_index || m_ptr != other.m_ptr;
		}

		// pre increment
		const Iterator& operator++() {
			m_index += m_index != m_ptr->size();
			return *this;
		}
		// post increment
		Iterator operator++(int) {
			auto output = *this;
			m_index += m_index != m_ptr->size();
			return output;
		}

	private:
		size_t m_index;
		small_vector<T, N> * m_ptr;
	};

	Iterator begin() { return Iterator(0, this); }
	Iterator end() { return Iterator(count, this); }

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

	void resize(size_t new_capacity) {
		if (new_capacity < N)
			realloc(0);
		else
			realloc(new_capacity - N);
		count = std::min(count, new_capacity);
	}

	void shrink() {
		if (count >= N)
			realloc(heap_size);
	}

	void clear() {
		realloc(0);
		count = 0;
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
