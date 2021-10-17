#include "catch.hpp"
#include "small_vector.hpp"
#include <cstddef>
#include <iostream>
#include <vector>

TEST_CASE("Small Vector no realloc") {
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

TEST_CASE("Small Vector realloc") {
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
}

