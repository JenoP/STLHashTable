#include <iostream>
#include <chrono>
#include <filesystem>

#include "Triangle.h"
#include "HashTable.h"


constexpr auto CMD_LINE_USAGE_STR = "JenoSTLLookup.exe wavy_surface_100000.stl {{0.000000e+00,0.000000e+00,0.000000e+00}{1.000000e+00,1.000000e+00,6.646931e-01}{0.000000e+00,1.000000e+00,0.000000e+00}}";


static void PrintUsage()
{
	std::cout << "Invalid input arguments." << std::endl;

	std::cout << "Usage ==> JenoSTLLookup.exe <STL file> <Triangle to lookup>" << std::endl << std::endl;
	std::cout << "Example ==> " << "\"" << CMD_LINE_USAGE_STR << "\"" << std::endl;
}

static std::optional<Triangle> GetTriangle(const std::string& triangleStr)
{
	std::stringstream triangleStrStream(triangleStr);

	Point3 p1, p2, p3;
	char dummy;

	auto parseSuccess = triangleStrStream &&
		(triangleStrStream >> dummy >> dummy >> p1.x >> dummy >> p1.y >> dummy >>
		p1.z >> dummy >> dummy >> p2.x >> dummy >> p2.y >> dummy >> p2.z >>
		dummy >> dummy >> p3.x >> dummy >> p3.y >> dummy >> p3.z);

	if (!parseSuccess)
	{
		std::cerr << "Failed to parse triangle coordinates from command line input." << std::endl;

		return {};
	}

	return std::optional<Triangle>({ p1, p2, p3 });
}

static bool AreValidInputArgs(const std::string& stlFileStr, const std::optional<Triangle>& triangle)
{
	return std::filesystem::exists(stlFileStr) && triangle.has_value();
}


int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			PrintUsage();
			return EXIT_FAILURE;
		}

		const std::string& stlFileStr = argv[1];
		const auto& triangle = GetTriangle(argv[2]);

		if (!AreValidInputArgs(stlFileStr, triangle))
		{
			PrintUsage();
			return EXIT_FAILURE;
		}

		HashTable hashTable;

		auto start = std::chrono::steady_clock::now();
		hashTable.Construct(stlFileStr);
		auto end = std::chrono::steady_clock::now();

		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Time taken for Hash table construction: " << ms << " MILLIseconds" << std::endl;

		start = std::chrono::steady_clock::now();
		auto triangleExists = hashTable.Lookup(*triangle);
		end = std::chrono::steady_clock::now();

		std::cout << (triangleExists ? "Given triangle exists." : "Given triangle NOT exists.") << std::endl;

		ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		std::cout << "Time taken for given triangle Lookup: " << ms << " NANOseconds" << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception occurred -- " << ex.what() << std::endl;

		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

