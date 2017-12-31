# C++ Summing For Loop Benchmark

The initial motivation is to find out the overhead of different for-loop types in C++

__Test Machine:__ Intel i7 6700 at 3.4 GHz

## Visual C++ 2017 (15.4 Update) result 

Please ignore the sum result. I display resultant sum to prevent compiler from optimizing away for loop.

```
 Increment For Loop:  599ms, sum:500000500000
     Range For Loop:  446ms, sum:500000500000
  Iterator For Loop:  558ms, sum:500000500000
        Accumulator:  437ms, sum:500000500000
```

Investigations shown multiplication in this below assembly could be the culprit in the slowdown in the __Increment For Loop__ . 

```
movdqu   xmm0, XMMWORD PTR vec$[rsp+rax*8]
```

As for the __Range For Loop__, the address is incremented by 16 (8 * 2 because of loop unrolling), there is no multiplication to calculate the address. Accumulator use the same tactics. Earlier in the decade, C programmers are baffled as to why std::accumulate is faster than for loop. Now we know the reason.


As for the __Iterator For Loop__ poor result, my guess is the iterator overhead.

## Cygwin clang++ 3.9.1 result

clang++ generate the similar code for all 4 for loops. Godbolt showed clang++ vectorized the for loop with SSE2.

```
# clang++ ForLoopBenchmark.cpp -O2 -std=c++14
 Increment For Loop:  392ms, sum:500000500000
     Range For Loop:  406ms, sum:500000500000
  Iterator For Loop:  381ms, sum:500000500000
        Accumulator:  391ms, sum:500000500000
```

## Cygwin g++ 5.4 result 

g++ also generate the similar code for all 4 code but it did not vectorize the loops.

```
# g++ ForLoopBenchmark.cpp -O2 -std=c++14
 Increment For Loop:  558ms, sum:500000500000
     Range For Loop:  552ms, sum:500000500000
  Iterator For Loop:  542ms, sum:500000500000
        Accumulator:  544ms, sum:500000500000
```

## Code

Input below code into [Godbolt Online C++ Compiler](https://godbolt.org/) to see the generated assembly code.

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

## "Is this information even useful?"

There is this [FIX Protocol](https://en.wikipedia.org/wiki/Financial_Information_eXchange#Checksum) which makes use of summing to calculate checksum of its message.

