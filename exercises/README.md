## optional exercise Algorithms Trivia


Use Cevelop to "Import Existing Projects into Workspace" to obtain the project **algorithm_trivia** in the folder src. To allow using it without cloning the repository, you can download [algorithm_trivia.zip](https://github.com/PeterSommerlad/CPPCourseIntroduction/raw/main/src/algorithm_trivia.zip) and unpack it locally. And then import the project in the directory. 

For using the existing STL algorithms effectively you have to familiarize yourself with available functionality. We have prepared a CUTE test project (`algorithm_trivia`) with a large set of test cases which require you to insert the correct STL algorithm in order to get the test green. It contains several test suites, each containing some test cases. Usually, those cases fail with the current implementation. We have replaced the original calls with dummy functions (`xxx`, `xxxx`, `xxxxx`, `xxxxxx`) that satisfy the interface. You don't have to worry about these helper functions. Your task is to fix the test cases by calling the correct STL algorithm (instead of the helper function).

Since there are quite a few algorithms you don't have to solve all algorithms at one. We suggest to solve 2-3 test suites each week. Each suite contains a hint header that lists the algorithms that have to be used in the corresponding suite file. Each algorithm is required once. Read the description of the algorithms first and then try to insert the correct calls. Some algorithms require a predicate. Most of the time we have used an `is_prime` function that just checks whether the parameter is prime.

Have Fun!

A direct link to godbolt compiler explorer is [here](https://godbolt.org/z/vMjhdbWP8). Note, remove "Find Algorithms" to run all trivia tests, or replace it by the name of a test suite as spelled in Test.cpp including the quotes. Sometimes compiler explorer might timeout, change something to try again.


## Explore Counting - async future

On the following compiler explorer link [https://godbolt.org/z/cd3czdcMv](https://godbolt.org/z/cd3czdcMv) you can observe how to count using `std::async` and `std::future`

core:

```cpp
	auto const startTime = std::chrono::high_resolution_clock::now();

	std::vector<std::future<result>> v{};
	v.reserve(times);
	generate_n(back_inserter(v),times,[rand](){return std::async(std::launch::async,MCClass{runs,rand});});
	result r{};
	for(auto &f:v){
		r = r + f.get();
	}
	auto const endTime = std::chrono::high_resolution_clock::now();
	auto const duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << "ratio overflows " << r.ratio() << " in " << duration / 1ms << "ms" << std::endl;
```


Modify the output to actually check the actual counts of the number of multiplications and overflows. 

Side Question: Why is the probability for signed integer overflow on multiplication near 100%? 

## Explore Counting - thread atomic

On the following compiler explorer link [https://godbolt.org/z/5o1Mb8jG4](https://godbolt.org/z/5o1Mb8jG4) you can observe how a similar code with an atomic counter behaves by using a thread pool and shared atomic counters.

core:

```cpp
	auto const startTime = std::chrono::high_resolution_clock::now();

	rng_dist rand;
	MCClass<int16_t,std::atomic<size_t>> mc{runs,rand};
	static_assert(not std::is_move_constructible_v<MCClass<int16_t,std::atomic<size_t>>>); // must share counters...

	std::vector<std::thread> v{};
	v.reserve(times);
	std::generate_n(back_inserter(v),times,[&mc](){return std::thread{std::ref(mc)};});
	for(auto &t:v){
		t.join();
	}
    result r{mc.overflows,mc.multiplications};
	auto const endTime = std::chrono::high_resolution_clock::now();
	auto const duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << "ratio overflows " << r.ratio() << " in " << duration / 1ms << "ms" << std::endl;
```

## Find Smells and Suggest Refactorings

Unfortunately some of the code of CERN is quite old. In your exercise breakout team, either chose a code piece you are familiar with or start with [AlgResourcePool.cpp](https://gitlab.cern.ch/gaudi/Gaudi/-/blob/master/GaudiHive/src/AlgResourcePool.cpp).

The first two steps will be time boxed based on advancement in the training (suggest 10min)

* look for code smells. Find at least 3 code smells (or more) 
* take notes of the found code smells
* share those smells with the other groups. 
* Can you also suggest refactorings to improve the situation?

A tip: If I want to find problematic cases, I often use a recursive grep to problematic things.

`find . -name "*.[ch]*" -exec fgrep -H '.unlock()' {} \;`

looks for explicit calls to unlock (a mutex). This can be an indication that the Scoped Locking pattern is not applied.

or

`find . -name "*.[ch]*" -exec grep -H "[ \t:]new " '{}' \;`

to look for explicit heap allocation with `new` instead of `make_unique()`

Example strings I search for in code:

* `const_cast` - cheating
* `reinterpret_cast` - only a few valid cases
* `dynamic_cast` - means often a design error in OO hierarchy
* `mutable` - if not for mutex or synchronization, big red flag
* `delete` - not using unique_ptr
* `throw;` - rethrows

Also checking for the use of synchronization primitives can be helpful. For example, in Rec and Goudi sources, that I checked out, I found 91 occurences of `scoped_lock`, 4 `unique_lock` and 24 of the now obsolete `lock_guard`, but also 238 uses of `mutex`.

When doing such counts it is also important to look at the found source to check if the match was not in a comment or larger word.
