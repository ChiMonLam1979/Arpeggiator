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

template<typename T>
void transposeNotes(std::vector<T>& vector, T const& noteValue)
{
	T firstNote = vector[0];

	T transposeValue = (noteValue - firstNote);

	for (T& note : vector)
	{
		note = note + transposeValue;
	}
}

template<typename T>
std::vector<T> assignVectorPlayCount(std::vector<T>& vector, const int numberOfTimesToPlay)
{
	std::vector<T> result;

	const int sizeOfVector = vector.size();

	for(int i = 0; i < numberOfTimesToPlay; i++)
	{
		result.insert(result.end(), vector.begin(), vector.begin() + sizeOfVector);
	}

	return result;
}