#pragma once

#include "array.h"

struct tests
{
	const int length = 5;
	
	array<int> test_data[5] = {
			array<int> {1, 2, 3, 4, 5, 6, 7},
			array<int> {2, 4, 6, 8, 10, -1, -3, -5},
			array<int> {1, 3, 5, 7, -2, -4, -6},
			array<int> {1, 1, 1, 1, 1, 1, 1, 1, 1},
			array<int> {1, 1, 1, 2, 2, 2, 1, 1, 1}
	};

	array<int> results[5] = {
			array<int> {2, 4, 6, 1, 3, 5, 7},
			array<int> {2, 4, 6, 8, 10, -1, -3, -5},
			array<int> {-2, -4, -6, 1, 3, 5, 7},
			array<int> {1, 1, 1, 1, 1, 1, 1, 1, 1},
			array<int> {2, 2, 2, 1, 1, 1, 1, 1, 1}
	};
};
