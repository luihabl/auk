#pragma once

#include <cstring>

namespace auk 
{
	template <typename T, size_t N>
	class SVector
	{

	public:
		SVector() = default;
		SVector(size_t n) { count = n; }

		SVector& operator=(const SVector& other)
		{
			count = other.count;
			std::memcpy(d, other.d, count * sizeof(T));
			return *this;
		}

		T& operator[](const size_t& i) { return d[i]; }
		const T& operator[](const size_t& i) { return d[i]; }

		T pop() { return d[count--]; }
		void push(const T& v) { d[count++] = v; }
		void remove(index_t i) { d[i] = d[count--]; }
		void clear() { count = 0; }
		
		const size_t& size() { return count; }

	private:
		T d[N];
		size_t count = 0;
	};
}