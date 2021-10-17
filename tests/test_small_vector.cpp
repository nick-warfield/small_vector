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

TEST_CASE("Capacity shrink without heap") {
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

TEST_CASE("Capacity shrink with heap") {
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

TEST_CASE("clear") {
	auto length = GENERATE(take(1, random(0, 1000)));
	small_vector<int, 25> vec;
	for (int i = 0; i < length; i++) {
		vec.push(i);
	}
	vec.clear();
	REQUIRE(vec.size() == 0);
	REQUIRE(vec.capacity() == 25);
}
