#include "montecarlo_mult.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include <type_traits>
#include <thread>
#include <future>
#include <functional>


void testMCClassCreate(){
	rng_dist rand;
	MCClass mc{1000,rand};
	mc();
	ASSERT_EQUAL(1000,mc.runs);
	
}


void CounterDefaultIsZeroValue() {
	rng_dist rand;

	MCClass mc{1000,rand};
	ASSERT_EQUAL(0ull,mc.overflows);
	ASSERT_EQUAL(0ull,mc.multiplications);
}

void RunInThread(){
	rng_dist rand;

	MCClass mc{1000,rand};
	{ std::jthread t{std::ref(mc)};
	}
	ASSERT_EQUAL(1000ull,mc.runs);
	ASSERT_EQUAL(1000ull,mc.multiplications);
	ASSERT_LESS_EQUAL(mc.overflows,1000ull);
}

void RunWithAsync(){
	rng_dist rand;
	MCClass mc{1000,rand};

	auto future = std::async(std::launch::async,std::ref(mc)); // cannot copy atomics
	auto result= future.get();
	ASSERT_EQUAL(1000,result.multiplications);
	ASSERT_LESS_EQUAL(result.overflows,1000);
	ASSERT_LESS(0.99,result.ratio());
}

void RunWithMultipleAsyncs(){
	rng_dist rand;
	size_t const runs{1000};
	int const times{10};
	static_assert(std::is_move_constructible_v<MCClass<>>);

	std::vector<std::future<result>> v{};
	v.reserve(times);
	generate_n(back_inserter(v),times,[rand](){return std::async(std::launch::async,MCClass{runs,rand});});

	result r{};
	for(auto &f:v){
		r = r + f.get();
	}
	ASSERT_EQUAL(runs*times,r.multiplications);
	ASSERT_LESS_EQUAL(r.overflows,r.multiplications);
}

void RunWithMultipleAsyncsAndSharedAtomicCounter(){
	rng_dist rand;
	size_t const runs{1000};
	MCClass<int16_t,std::atomic<size_t>> mc{runs,rand};
	int const times{10};
	static_assert(not std::is_move_constructible_v<MCClass<int16_t,std::atomic<size_t>>>); // must share counters...

	std::vector<std::future<result>> v{};
	v.reserve(times);
	std::generate_n(back_inserter(v),times, [&mc](){return std::async(std::launch::async,std::ref(mc));});

	result r{};
	for(auto &f:v){
		r = r + f.get();
	} // computes the wrong result, because counters are shared
	ASSERT_LESS(mc.overflows,runs*times);
	ASSERT_EQUAL(runs*times,mc.multiplications);
}
void RunWithMultipleThreadsAndSharedAtomicCounter(){
	rng_dist rand;
	size_t const runs{1000};
	MCClass<int16_t,std::atomic<size_t>> mc{runs,rand};
	int const times{10};
	static_assert(not std::is_move_constructible_v<MCClass<int16_t,std::atomic<size_t>>>); // must share counters...

	std::vector<std::thread> v{};
	v.reserve(times);
	std::generate_n(back_inserter(v),times,[&mc](){return std::thread{std::ref(mc)};});
	for(auto &t:v){
		t.join();
	}
	ASSERT_LESS(mc.overflows,runs*times);
	ASSERT_EQUAL(runs*times,mc.multiplications);
}




bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(CounterDefaultIsZeroValue));
	s.push_back(CUTE(testMCClassCreate));
	s.push_back(CUTE(RunInThread));
	s.push_back(CUTE(RunWithAsync));
	s.push_back(CUTE(RunWithMultipleAsyncs));
	s.push_back(CUTE(RunWithMultipleAsyncsAndSharedAtomicCounter));
	s.push_back(CUTE(RunWithMultipleThreadsAndSharedAtomicCounter));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
