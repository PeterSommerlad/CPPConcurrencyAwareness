/*
 * montecarlo_mult.h
 *
 *  Created on: 21 Nov 2023
 *      Author: sop
 */

#ifndef SRC_MONTECARLO_MULT_H_
#define SRC_MONTECARLO_MULT_H_



#include <cstdint>
#include <random>
#include <atomic>


template<typename INT>
using largerint=std::conditional_t<std::is_same_v<int8_t,INT>||std::is_same_v<int16_t,INT>, int32_t,
          std::conditional_t<std::is_same_v<int32_t,INT>,int64_t,
		  std::conditional_t<std::is_same_v<uint8_t,INT>||std::is_same_v<uint16_t,INT>,uint32_t,
		  std::conditional_t<std::is_same_v<uint32_t,INT>,uint64_t,
		  void>>>>;


static_assert(std::is_same_v<int32_t,largerint<int16_t>>);
static_assert(std::is_same_v<int64_t,largerint<int32_t>>);

struct result {
	size_t overflows;
	size_t multiplications;
	double ratio() const {
		return (0.l+overflows)/multiplications; // compute in long double
	}
	friend result operator+(result l, result r){
		return {l.overflows+r.overflows,l.multiplications+r.multiplications};
	}
};
static std::random_device the_random_seed { };

template<typename testinteger=int16_t>
struct rng_dist {
    std::mt19937 rng;
    std::uniform_int_distribution<testinteger> rand; // distribution in range
    rng_dist()
     :rng{the_random_seed()}
     ,rand{std::numeric_limits<testinteger>::min(),std::numeric_limits<testinteger>::max()}
     {}

     testinteger operator()() {
    	 return rand(rng);
     }

};

static_assert(std::is_copy_constructible_v<rng_dist<>>);

template<typename testinteger=int16_t,typename Counter=size_t>
struct MCClass {
	rng_dist<testinteger> rand;
    using largeint = largerint<testinteger>;
    static constexpr largeint limit { static_cast<largeint>(std::numeric_limits<testinteger>::max())};


    size_t const runs{};
    Counter overflows{};
    Counter multiplications{};

	MCClass(size_t runs,rng_dist<testinteger> random)
	:rand{random}
	,runs{runs}
	{}
	result operator()(){
    	for (size_t n=0; n < runs; ++n){
    		testinteger const x = rand();
    		testinteger const y = rand();
    		largeint const result{ static_cast<largeint>(x)*(y) };
    		if (result > limit || result < -limit-1 ) ++overflows;
    		++multiplications;
    	}
    	return {overflows,multiplications};
	}
};

#endif /* SRC_MONTECARLO_MULT_H_ */
