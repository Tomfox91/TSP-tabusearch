#pragma once

#include <vector>

/*
			count = 6
	  __0___1___2___3___4___5__ j
	0|	.	.	.	.	.	.
	1|	x	.	.	.	.	.
	2|	x	x	.	.	.	.
	3|	x	x	x	.	.	.
	4|	x	x	x	x	.	.
	5|	x	x	x	x	x	.
	i
*/

template <typename T>
class TriMatrix {
	std::vector<T> vector;

	static inline constexpr unsigned int offset(
		const unsigned int i, const unsigned int j) {
		return ((i-1)*i/2) + j; // j < i
	}

public:
	TriMatrix(const unsigned int count)
		: vector(1 + offset(count-1, count-2)) {}

	TriMatrix(const unsigned int count, const T fillValue)
		: vector(1 + offset(count-1, count-2), fillValue) {}

	inline T& operator() (const unsigned int i, const unsigned int j) {
		return vector[j < i ? offset(i, j) : offset(j, i)];
	}

	inline T  operator() (const unsigned int i, const unsigned int j) const {
		return vector[j < i ? offset(i, j) : offset(j, i)];
	}

};