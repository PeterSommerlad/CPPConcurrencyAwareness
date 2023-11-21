#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>


struct AtomicCounter {
	void increment() {
		++value;
	}
	int current() const {
		return value;
	}
private:
	std::atomic<int> value{};
};
int main () {
    using namespace std::chrono_literals;
	AtomicCounter counter{};
	auto run = [&counter]{
		for (int i = 0; i < 100'000'000; ++i) {
			counter.increment();
		}
	};
	auto const startTime = std::chrono::high_resolution_clock::now();
	std::thread t1{run};
	std::thread t2{run};
	t1.join();
	t2.join();
	auto const endTime = std::chrono::high_resolution_clock::now();
	auto const duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << "Counter " << counter.current() << " in " << duration/1ms << "ms" << std::endl;
}
