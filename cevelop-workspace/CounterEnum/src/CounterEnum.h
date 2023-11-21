#ifndef SRC_COUNTERENUM_H_
#define SRC_COUNTERENUM_H_

#include <cstdint>

namespace CounterEnum {

template<typename INTBASE=uint64_t>
struct CounterT {

enum class Counter: INTBASE{};

friend constexpr
auto value(Counter count){
	return static_cast<INTBASE>(count);
}

friend constexpr
Counter & operator++(Counter &count){
	count = static_cast<Counter>(value(count)+1);
	return count;
}
};

inline namespace literals{
consteval
auto operator""_ci8(unsigned long long init){
	return static_cast<CounterT<int8_t>::Counter>(init);
}
consteval
auto operator""_cu8(unsigned long long init){
	return static_cast<CounterT<uint8_t>::Counter>(init);
}
consteval
auto operator""_ci16(unsigned long long init){
	return static_cast<CounterT<int16_t>::Counter>(init);
}
consteval
auto operator""_cu16(unsigned long long init){
	return static_cast<CounterT<uint16_t>::Counter>(init);
}
consteval
auto operator""_ci32(unsigned long long init){
	return static_cast<CounterT<int32_t>::Counter>(init);
}
consteval
auto operator""_cu32(unsigned long long init){
	return static_cast<CounterT<uint32_t>::Counter>(init);
}
consteval
auto operator""_ci64(unsigned long long init){
	return static_cast<CounterT<int64_t>::Counter>(init);
}
consteval
auto operator""_cu64(unsigned long long init){
	return static_cast<CounterT<uint64_t>::Counter>(init);
}
} // literals
namespace simple {
enum class Counter:uint64_t{};
constexpr
auto value(Counter count){
	return static_cast<uint64_t>(count);
}

constexpr
Counter & operator++(Counter &count){
	count = static_cast<Counter>(value(count)+1ull);
	return count;
}
inline namespace literals{
consteval
auto operator""_c(unsigned long long init){
	return static_cast<Counter>(init);
}

}

}

template<typename INTBASE>
using Counter=CounterT<INTBASE>::Counter;

}



#endif /* SRC_COUNTERENUM_H_ */
