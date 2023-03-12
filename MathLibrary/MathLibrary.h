#pragma once

#ifdef MATHLIBRARY_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

// Laiko matavimo klase (Task 1)
class Timer {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;

public:
	EXPORT Timer() : start{ std::chrono::high_resolution_clock::now() } {}

	EXPORT void reset() { start = std::chrono::high_resolution_clock::now(); }

	EXPORT double elapsed() const {
		return std::chrono::duration<double>(
			std::chrono::high_resolution_clock::now() - start)
			.count();
	}
};

EXPORT void familyControlVec(const std::string& username, std::vector<std::string>& day, std::vector<std::string>& time);

EXPORT void recursiveMkdir(const std::string& name, const std::string& surname);

EXPORT void recursiveRm(const std::string& path);

EXPORT void solveTschirnhausenCubic(const double& F, const double& x0, const double& xn, const double& deltax,
									const std::string& firstname, const std::string& lastname, const bool& BE_VERBOSE = false);

EXPORT int concatFiles(const std::string& name, const std::string& surname, const std::string& prefix);

EXPORT int sortResults(const std::string& filePrefix);