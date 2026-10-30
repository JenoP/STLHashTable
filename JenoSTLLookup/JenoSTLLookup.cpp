#include <iostream>
#include <chrono>
#include <filesystem>

#include "Triangle.h"
#include "HashTable.h"


constexpr auto CMD_LINE_USAGE_STR = "JenoSTLLookup.exe wavy_surface_100000.stl {{0.000000e+00,0.000000e+00,0.000000e+00} {1.000000e+00,1.000000e+00,6.646931e-01} {0.000000e+00,1.000000e+00,0.000000e+00}}";


static void PrintUsage()
{
	std::cout << "Invalid input arguments. Please check the usage." << std::endl;

	std::cout << "Usage ==> JenoSTLLookup.exe <STL file> <Triangle to lookup>" << std::endl;
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

static bool AreValidInputArgs(const std::string& stlFileStr, const std::string& triangleStr)
{
	const auto& triangle = GetTriangle(triangleStr);

	return std::filesystem::exists(stlFileStr) && triangle.has_value();
}


int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3 || !AreValidInputArgs(argv[1], argv[2]))
		{
			PrintUsage();

			return EXIT_FAILURE;
		}

		HashTable hashTable;

		auto start = std::chrono::steady_clock::now();
		hashTable.Construct(argv[1]);
		auto end = std::chrono::steady_clock::now();

		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Time taken for Hash table construction: " << ms << " MILLIseconds" << std::endl;

		start = std::chrono::steady_clock::now();
		auto triangleExists = hashTable.Lookup(GetTriangle(argv[2]).value());
		end = std::chrono::steady_clock::now();

		std::cout << (triangleExists ? "Given triangle exists." : "Given triangle NOT exists.") << std::endl;

		ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout << "Time taken for given triangle Lookup: " << ms << " MICROseconds" << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception occurred -- " << ex.what() << std::endl;

		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

