#pragma once


#include <vector>


constexpr auto MAX_TRIANGLES = 1000000; // 1 Million


// Dense, flat, memory efficient, super fast Hash table targeting higher Cache-hit rates.
// Having given that Hash function is collision-free, implementation is heavily optimized for Lookups.

class HashTable
{
public:
	HashTable() = default;
	~HashTable() = default;

	HashTable(const HashTable&) = delete;
	HashTable(HashTable&&) = delete;
	HashTable& operator=(const HashTable&) = delete;
	HashTable& operator=(HashTable&&) = delete;

	// This just builds a Lookup/Index table rather than storing triangles directly.
	void Construct(const std::string& stlFileStr, const uint32_t maxEntries = MAX_TRIANGLES);

	bool Lookup(const Triangle& srcTriangle) const;

private:
	Triangle GetTriangle(std::ifstream& stlFileStream) const;

private:
	using TriangleObjPtr = std::unique_ptr<Triangle>;

	std::vector<TriangleObjPtr> m_TriangleLookupTable;
};