#include <algorithm>
#include <fstream>
#include <sstream>

#include "Triangle.h"
#include "Hash.h"
#include "HashTable.h"


constexpr auto OUTER_LOOP_STR = "outer loop";


void HashTable::Construct(const std::string& stlFileStr, const uint32_t maxEntries)
{
	std::ifstream stlFileStream(stlFileStr);
	if (!stlFileStream)
	{
		throw std::runtime_error("Unable to open given STL file.");
	}

	// Allocates space to construct index/lookup table rather than storing values directly.
	m_TriangleLookupTable.resize(maxEntries);

	std::string line;
	while (std::getline(stlFileStream, line))
	{
		if (line.find(OUTER_LOOP_STR) != std::string::npos)
		{
			_ConstructInternal(std::make_shared<Triangle>(_GetTriangle(stlFileStream)));
		}
	}
}

bool HashTable::Lookup(const Triangle& srcTriangle) const
{
	const auto index = hashTriangle(std::get<0>(srcTriangle), std::get<1>(srcTriangle), std::get<2>(srcTriangle));

	auto triangleExists = false;

	if (index < m_TriangleLookupTable.size())
	{
		const auto& dstTriangleObjPtr = m_TriangleLookupTable[index];
		triangleExists = dstTriangleObjPtr && AreTrianglesEqual(srcTriangle, *dstTriangleObjPtr);
	}

	return triangleExists;
}

void HashTable::_ConstructInternal(TriangleObjPtr&& triangleObjPtr)
{
	const auto index = hashTriangle(std::get<0>(*triangleObjPtr), std::get<1>(*triangleObjPtr), std::get<2>(*triangleObjPtr));

	if (index >= m_TriangleLookupTable.size())
	{
		throw std::runtime_error("Index out of range: " + std::to_string(index));
	}

	if (!m_TriangleLookupTable[index])
	{
		m_TriangleLookupTable[index] = std::move(triangleObjPtr);
	}

#ifdef _DEBUG
	//std::cout << "Triangle inserted at index: " << index << std::endl;
#endif
}

Triangle HashTable::_GetTriangle(std::ifstream& stlFileStream) const
{
	Point3 p1, p2, p3;
	std::string dummy;

	auto parseSuccess = stlFileStream &&
		(stlFileStream >> dummy >> p1.x >> p1.y >> p1.z &&
		stlFileStream >> dummy >> p2.x >> p2.y >> p2.z &&
		stlFileStream >> dummy >> p3.x >> p3.y >> p3.z);

	if (!parseSuccess)
	{
		throw std::runtime_error("Failed to parse given STL file.");
	}

	return { p1, p2, p3 };
}

