// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <process.h>
#include <limits.h>
#include <iostream>
#include <chrono>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <array>
#include <windows.h>
#include "MathLibrary.h"

// Declare helpers
bool IsAdmin();

// Task 2.1
// Studento numeris: 2110269 % 3 = 0 + 1 = 1
void familyControlVec(const std::string& username, std::vector<std::string>& day, std::vector<std::string>& time)
{
	if (!IsAdmin()) {
		std::cout << "The program is not running as an adminstrator.\n"
			<< "You won't be able to use family control.\n"
			<< "Run the program as an administrator for this step to work.\n";
		return;
	}

	if (day.size() != time.size()) {
		std::cout << "Both vectors should have the same size.\n";
		system("pause");
		exit(1);
	}

	std::string datetime = "";
	for (int i = 0; i < day.size(); i++) {
		datetime = datetime + day.at(i) + "," + time.at(i) + ";";
	}

	std::string command = "net user " + username + " /time:" + datetime;
	std::cout << "The comand that will be run: " << command << std::endl;

	// Actually run commands
	system("net user vaikas");
	system(command.c_str());
	system("net user vaikas");
}

// Task 3
void recursiveRm(const std::string& path)
{
	system(("rmdir /s /q " + path + " 2>nul").c_str());
}

void recursiveMkdir(const std::string& name, const std::string& surname)
{
	// Create root "PAVARDENIS" folder
	std::string rootFolder = surname;

	recursiveRm(rootFolder); // Remove the folder if it exists already

	system(("mkdir " + rootFolder).c_str());

	for (int i = 1; i <= 3; i++) {
		// Next level folder should be named Vardenis1, Vardenis2, Vardenis3
		std::string firstNest = rootFolder + "\\" + name + std::to_string(i);

		system(("mkdir " + firstNest).c_str());

		for (int j = 1; j <= 3; j++) {
			// Last level should be named like Vardenis1Vardenis1
			std::string secondNest = firstNest + "\\" + name + std::to_string(i) + name + std::to_string(j);
			system(("mkdir " + secondNest).c_str());
		}
	}
}

// Task 4
double tschirnhausenCubic(const double& x, const double& F) {
	return sqrt(pow(x, 3) + 3 * pow(x, 2) - F);
}

void solveTschirnhausenCubic(const double& F, const double& x0, const double& xn, const double& deltax,
	const std::string& firstname, const std::string& lastname, const bool& BE_VERBOSE)
{
	// Open all 9 files at the same time
	std::array<std::stringstream, 9> buffers;

	int i = 0;
	for (double curx = x0; curx <= xn; curx += deltax)
	{
		double tempy = tschirnhausenCubic(curx, F);
		if (tempy != tempy) continue;

		if (BE_VERBOSE) std::cout << "Folder ID: " << i << ", current x: " << curx << " function value: " << tempy << '\n';
		buffers[i] << curx << ' ' << tempy << '\n';
		i = (i == 8) ? 0 : i + 1;

		// Every solution has two roots except for zero
		if (tempy != 0) {
			if (BE_VERBOSE) std::cout << "Folder ID: " << i << ", current x: " << curx << " function value: " << -tempy << '\n';
			buffers[i] << curx << ' ' << -tempy << '\n';
			i = (i == 8) ? 0 : i + 1;
		}
	}

	for (int64_t i = 1; i <= 3; i++) {
		for (int64_t j = 1; j <= 3; j++) {
			std::string filename = lastname + "\\" + firstname + std::to_string(i) + "\\" + firstname + std::to_string(i) + firstname + std::to_string(j) + "\\solutions.txt";
			std::ofstream file(filename);
			file << buffers[(i - 1) * 3 + (j - 1)].str(); // write buffer to file
			file.close();
		}
	}
}

// Task 5
int concatFiles(const std::string& name, const std::string& surname, const std::string& prefix)
{
	std::ifstream solutionsFile;
	std::ofstream resultFile("results" + prefix + ".txt");
	if (!resultFile.good()) {
		std::cout << "Results file can't be createad.\n";
		return 1;
	}

	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			std::string filepath = surname + "\\" + name + std::to_string(i) + "\\" + name + std::to_string(i) + name + std::to_string(j) + "\\solutions.txt";

			solutionsFile.open(filepath);
			if (!solutionsFile.good()) {
				std::cout << "File does not exist. Filepath: " << filepath << std::endl;
				return 1;
			}

			resultFile << solutionsFile.rdbuf();
			solutionsFile.close();
		}
	}

	resultFile.close();
	return 0;
}

struct Point {
	double x, y;
};

int sortResults(const std::string& filePrefix)
{
	// Get points from a file
	std::ifstream inputFile("results" + filePrefix + ".txt");
	if (!inputFile) {
		std::cerr << "Failed to open input file. Filepath: " << "results" + filePrefix + ".txt" << std::endl;
		return 1;
	}
	std::vector<Point> points;
	Point temp{};
	// TODO: I don't need to read the file here. It is really slow.
	while (inputFile >> temp.x >> temp.y) {
		points.push_back(temp);
	}
	inputFile.close();
	std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
		return p1.x < p2.x;
		});

	// Convert vector to string for faster writing
	std::stringstream ss;
	for (const auto& p : points)
	{
		ss << p.x << " " << p.y << '\n';
	}

	// Write sorted results to a file
	std::ofstream outputFile("results" + filePrefix + ".txt");
	if (!outputFile) {
		std::cerr << "Failed to open output file. Filepath: " << "results" + filePrefix + ".txt" << std::endl;
		return 1;
	}
	outputFile << ss.str();
	outputFile.close();

	std::cout << "Sorted results were written to a file.\n";
	return 0;
}

// Helpers:
bool IsAdmin()
{
	BOOL isAdmin = FALSE;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	if (AllocateAndInitializeSid(&NtAuthority, 2,
		SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0, &AdministratorsGroup))
	{
		if (!CheckTokenMembership(NULL, AdministratorsGroup, &isAdmin))
		{
			isAdmin = FALSE;
		}
		FreeSid(AdministratorsGroup);
	}
	return (isAdmin != FALSE);
}