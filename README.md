# forloopbenchmark
C++ Summation For Loop Benchmark Results

Machine: Intel i7 6700 at 3.4 GHz

## Visual C++ 2017 (15.4 Update) result 

```
 Increment For Loop:  599ms, sum:500000500000
     Range For Loop:  446ms, sum:500000500000
  Iterator For Loop:  558ms, sum:500000500000
        Accumulator:  437ms, sum:500000500000
```

Investigations shown multiplication in this below assembly could be the culprit in the slowdown in the __Increment For Loop__ . As for the __Iterator For Loop__ poor result, my guess is the iterator overhead.

```
movdqu   xmm0, XMMWORD PTR vec$[rsp+rax*8]
```

## Cygwin clang++ 3.9.1 results. 

clang++ generate the similar code for all 4 for loops. Godbolt showed clang++ vectorized the for loop with SSE2.


```
# clang++ ForLoopBenchmark.cpp -O2 -std=c++14
 Increment For Loop:  392ms, sum:500000500000
     Range For Loop:  406ms, sum:500000500000
  Iterator For Loop:  381ms, sum:500000500000
        Accumulator:  391ms, sum:500000500000
```

## Cygwin g++ 5.4 results. 

g++ also generate the similar code for all 4 for loops but it did not vectorize for loop

```
# g++ ForLoopBenchmark.cpp -O2 -std=c++14
 Increment For Loop:  558ms, sum:500000500000
     Range For Loop:  552ms, sum:500000500000
  Iterator For Loop:  542ms, sum:500000500000
        Accumulator:  544ms, sum:500000500000
```

## Code

Input below code into (Godbolt Online C++ Compiler)[https://godbolt.org/] to see the generated assembly code.

```C++
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <iterator>

const size_t LEN = 1000000;

uint64_t func1()
{
	uint64_t vec[LEN];

	uint64_t sum = 0;
	for (size_t i = 0; i < LEN; ++i)
	{
		sum += vec[i];
	}
	return sum;
}

uint64_t func2()
{
	uint64_t vec[LEN];

	uint64_t sum = 0;
	for (auto n : vec)
	{
		sum += n;
	}
	return sum;
}

uint64_t func3()
{
	uint64_t vec[LEN];

	uint64_t sum = 0;
	for (auto it = std::cbegin(vec); it != std::cend(vec); ++it)
	{
		sum += *it;
	}
	return sum;
}

uint64_t func4()
{
	uint64_t vec[LEN];

	uint64_t sum = 0;
	const uint64_t Zero = 0;

	sum = std::accumulate(std::cbegin(vec), std::cend(vec), Zero);
	return sum;
}

```