#include "catch.hpp"
#include "small_vector.hpp"
#include <cstddef>
#include <iostream>
#include <vector>

TEST_CASE("push/pop no realloc") {
	small_vector<int, 10> vec;
	REQUIRE(vec.size() == 0);
	REQUIRE(vec.capacity() == 10);

	for (size_t i = 0; i < 10; i++) {
		vec.push(i);
		REQUIRE(vec[i] == (int)i);
		REQUIRE(vec.size() == i + 1);
		REQUIRE(vec.capacity() == 10);
	}

	for (int i = 9; i >= 0; i--) {
		vec.pop();
		REQUIRE(vec.size() == (size_t)i);
		REQUIRE(vec.capacity() == 10);
	}

	REQUIRE(vec.size() == 0);
	REQUIRE(vec.capacity() == 10);
	vec.pop();
	REQUIRE(vec.size() == 0);
	REQUIRE(vec.capacity() == 10);
}

TEST_CASE("push/pop with realloc") {
	small_vector<int, 10> vec;
	for (size_t i = 0; i < 10; i++) {
		vec.push(i);
		REQUIRE(vec[i] == (int)i);
		REQUIRE(vec.size() == i + 1);
		REQUIRE(vec.capacity() == 10);
	}
	for (size_t i = 10; i < 20; i++) {
		vec.push(i);
		REQUIRE(vec[i] == (int)i);
		REQUIRE(vec.size() == i + 1);
		REQUIRE(vec.capacity() > 10);
	}

	auto cap = vec.capacity();
	for (size_t i = 19; i > 0; i--) {
		vec.pop();
		REQUIRE(vec[i] == (int)i);
		REQUIRE(vec.size() == i);
		REQUIRE(vec.capacity() == cap);
	}
}

TEST_CASE("vector shrink() without heap") {
	auto length = GENERATE(take(1, random(0, 25)));
	small_vector<int, 25> vec;
	for (int i = 0; i < length; i++) {
		vec.push(i);
	}
	auto og_size = vec.size();
	auto og_cap = vec.capacity();
	vec.shrink();
	REQUIRE(og_size == vec.size());
	REQUIRE(og_cap == vec.capacity());
	REQUIRE(vec.size() <= vec.capacity());
}

TEST_CASE("vector shrink() with heap") {
	auto length = GENERATE(take(1, random(26, 1000)));
	small_vector<int, 25> vec;
	for (int i = 0; i < length; i++) {
		vec.push(i);
	}
	auto og_size = vec.size();
	auto og_cap = vec.capacity();
	vec.shrink();
	REQUIRE(og_size == vec.size());
	REQUIRE(og_cap >= vec.capacity());
	REQUIRE(vec.size() == vec.capacity());
}

TEST_CASE("vector clear()") {
	auto length = GENERATE(take(1, random(0, 1000)));
	small_vector<int, 25> vec;
	for (int i = 0; i < length; i++) {
		vec.push(i);
	}
	vec.clear();
	REQUIRE(vec.size() == 0);
	REQUIRE(vec.capacity() == 25);
}

TEST_CASE("vector resize() no delete items") {
	auto length = GENERATE(take(1, random(0, 99)));
	small_vector<int, 50> vec;
	for (int i = 0; i < length; i++) {
		vec.push(i);
	}
	auto og_size = vec.size();
	auto og_cap = vec.capacity();
	auto lo_cap = GENERATE_COPY(
			take(1, random(og_size, og_cap - 1)));
	auto hi_cap = GENERATE_COPY(
			take(1, random(og_cap, (size_t)200)));

	vec.resize(lo_cap);
	REQUIRE(vec.size() == og_size);
	REQUIRE(vec.capacity() <= og_cap);
	vec.resize(hi_cap);
	REQUIRE(vec.size() == og_size);
	REQUIRE(vec.capacity() >= og_cap);
}

TEST_CASE("vector resize() delete items") {
	auto length = GENERATE(take(1, random(0, 99)));
	small_vector<int, 50> vec;
	for (int i = 0; i < length; i++) {
		vec.push(i);
	}
	auto og_size = vec.size();
	auto og_cap = vec.capacity();
	auto new_cap = GENERATE_COPY(
			take(1, random((size_t)0, og_size - 1)));

	vec.resize(new_cap);
	REQUIRE(vec.size() < og_size);
	REQUIRE(vec.capacity() <= og_cap);
}

TEST_CASE("Iterator") {
	small_vector<int, 20> vec;
	int count = 0;
	for (auto i : vec) {
		i++;
		count++;
	}
	REQUIRE(count == 0);
	REQUIRE(vec.begin() == vec.end());

	for (int i = 0; i < 100; i++) {
		vec.push(i);
	}

	for (auto i : vec) {
		REQUIRE(i == count);
		count++;
	}
	REQUIRE(count == 100);
	REQUIRE(vec.begin() != vec.end());
}
