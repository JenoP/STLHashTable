#pragma once

struct Point3 
{ 
	double x = 0.0, y = 0.0, z = 0.0;

	bool operator==(const Point3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
};

typedef std::tuple<Point3, Point3, Point3> Triangle;


inline bool AreTrianglesEqual(const Triangle& src, const Triangle& dst)
{
	const auto& [p1Src, p2Src, p3Src] = src;
	const auto& [p1Dst, p2Dst, p3Dst] = dst;

	return p1Src == p1Dst && p2Src == p2Dst && p3Src == p3Dst;
}