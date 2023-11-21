
#include "CounterEnum.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

using Counter=CounterEnum::Counter<int8_t>;

void CounterDefaultIsZeroValue() {
	Counter const c{};
	ASSERT_EQUAL(0ull,value(c));
}

void CounterIncrementedIsOneValue(){
	Counter c{};
	++c;
	ASSERT_EQUAL(1ull,value(c));
}
void CounterCanBeComparedAndValueGivenAtConstruction(){
	Counter c{1};
	++c;
	ASSERT_EQUAL(Counter{2},c);
}
void CounterCanBeComparedLess(){
	Counter const c{1},d{2};
	ASSERT_LESS(c,d);
}

void CounterCanBeComparedLessEqual(){
	Counter const c{1};
	ASSERT_LESS_EQUAL(c,c);
}

void CounterCanBeComparedGreater(){
	Counter const c{1},d{2};
	ASSERT_GREATER(d,c);
}

void CounterCanBeComparedGreaterEqual(){
	Counter const c{1};
	ASSERT_GREATER_EQUAL(c,c);
}

void CounterUDL(){
	using namespace CounterEnum::literals;
	auto const c{1_ci8};
	ASSERT_EQUAL(Counter{1},c);
}

namespace simpletest{
using Counter=CounterEnum::simple::Counter;

void CounterDefaultIsZeroValue() {
	Counter const c{};
	ASSERT_EQUAL(0ull,value(c));
}

void CounterIncrementedIsOneValue(){
	Counter c{};
	++c;
	ASSERT_EQUAL(1ull,value(c));
}
void CounterCanBeComparedAndValueGivenAtConstruction(){
	Counter c{1};
	++c;
	ASSERT_EQUAL(Counter{2},c);
}
void CounterCanBeComparedLess(){
	Counter const c{1},d{2};
	ASSERT_LESS(c,d);
}

void CounterCanBeComparedLessEqual(){
	Counter const c{1};
	ASSERT_LESS_EQUAL(c,c);
}

void CounterCanBeComparedGreater(){
	Counter const c{1},d{2};
	ASSERT_GREATER(d,c);
}

void CounterCanBeComparedGreaterEqual(){
	Counter const c{1};
	ASSERT_GREATER_EQUAL(c,c);
}

void CounterUDL(){
	using namespace CounterEnum::simple::literals;
	auto const c{1_c};
	ASSERT_EQUAL(Counter{1},c);
}

}


bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(CounterDefaultIsZeroValue));
	s.push_back(CUTE(CounterIncrementedIsOneValue));
	s.push_back(CUTE(CounterCanBeComparedAndValueGivenAtConstruction));
	s.push_back(CUTE(CounterCanBeComparedLess));
	s.push_back(CUTE(CounterCanBeComparedLessEqual));
	s.push_back(CUTE(CounterCanBeComparedGreater));
	s.push_back(CUTE(CounterCanBeComparedGreaterEqual));
	s.push_back(CUTE(CounterUDL));
	s.push_back(CUTE(simpletest::CounterDefaultIsZeroValue));
	s.push_back(CUTE(simpletest::CounterIncrementedIsOneValue));
	s.push_back(CUTE(simpletest::CounterCanBeComparedAndValueGivenAtConstruction));
	s.push_back(CUTE(simpletest::CounterCanBeComparedLess));
	s.push_back(CUTE(simpletest::CounterCanBeComparedLessEqual));
	s.push_back(CUTE(simpletest::CounterCanBeComparedGreater));
	s.push_back(CUTE(simpletest::CounterCanBeComparedGreaterEqual));
	s.push_back(CUTE(simpletest::CounterUDL));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
