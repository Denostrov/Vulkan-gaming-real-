#pragma once
#include <vector>

template<class T>
class ObjectPool
{
public:
	ObjectPool()
		:objects(1024), parentIndices(1024), count(0)
	{}
	ObjectPool(ObjectPool const&) = delete;

	std::size_t add(T const& newObject, std::size_t* parentIndex)
	{
		parentIndices[count] = parentIndex;
		objects[count] = newObject;
		return count++;
	}

	void remove(std::size_t index)
	{
		parentIndices[index] = parentIndices[count - 1];
		objects[index] = std::move(objects[count - 1]);
		*parentIndices[index] = index;
		count--;
	}
	std::size_t size() const { return count; }
	T* data() { return objects.data(); }

private:
	std::vector<T> objects;
	std::vector<std::size_t*> parentIndices;
	std::size_t count;
};