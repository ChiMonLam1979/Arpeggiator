#pragma once
#include <vector>
#include <algorithm>

template<typename T>
void erase(std::vector<T>& vector, T const& value)
{
	vector.erase(std::remove(begin(vector), end(vector), value), end(vector));
}

template<typename T>
bool inRange(T value, T minExclusive, T maxInclusive)
{
	return (value > minExclusive && value <= maxInclusive);
}