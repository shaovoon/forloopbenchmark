# forloopbenchmark
C++ Summation For Loop Benchmark

Visual C++ 2017 (15.4 Update) on Intel i7 6700 at 3.4 GHz

```
 Increment For Loop:  472ms, sum:500000500000
     Range For Loop:  340ms, sum:500000500000
  Iterator For Loop:  487ms, sum:500000500000
        Accumulator:  331ms, sum:500000500000
```

```C++
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
	for (auto it = vec.cbegin(); it!=vec.cend(); ++it)
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
```