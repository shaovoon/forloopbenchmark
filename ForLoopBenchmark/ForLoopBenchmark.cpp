// ForLoopBenchmark.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <numeric>
#include <chrono>
#include <algorithm>

class timer
{
public:
	timer() = default;
	void start_timing(const std::string& text_)
	{
		text = text_;
		begin = std::chrono::high_resolution_clock::now();
	}
	void stop_timing(uint64_t sum)
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto dur = end - begin;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		std::cout << std::setw(19) << text << ":" << std::setw(5) << ms << "ms" << ", sum:" << sum <<std::endl;
	}

private:
	std::string text;
	std::chrono::high_resolution_clock::time_point begin;
};

int main(int argc, char *argv[])
{
	const size_t MAX_LOOP = (argc == 2) ? atoi(argv[1]) : 1000;

	std::vector<uint64_t> vec(1000000);
	std::iota(vec.begin(), vec.end(), 1);
	timer stopwatch;

	stopwatch.start_timing("Increment For Loop");
	uint64_t sum = 0;
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		sum = 0;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			sum += vec[i];
		}
	}
	stopwatch.stop_timing(sum);

	stopwatch.start_timing("Range For Loop");
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		sum = 0;
		for (auto n : vec)
		{
			sum += n;
		}
	}
	stopwatch.stop_timing(sum);

	stopwatch.start_timing("Iterator For Loop");
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		sum = 0;
		//auto end_it = vec.cend();
		for (auto it = vec.cbegin(); it != vec.cend(); ++it)
		{
			sum += *it;
		}
	}
	stopwatch.stop_timing(sum);

	stopwatch.start_timing("Accumulator");
	constexpr const uint64_t Zero = 0;
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		sum = std::accumulate(vec.cbegin(), vec.cend(), Zero);
	}
	stopwatch.stop_timing(sum);

    return 0;
}

