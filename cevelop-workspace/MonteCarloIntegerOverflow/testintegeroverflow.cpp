#include <iostream>
#include <cstdint>
#include <climits>
#include <limits>
#include <random>
#include <iomanip>
#include <thread>
using largeint=  _ExtInt(128); // _BitInt(128) on more modern clang
using size_type = size_t;
using testinteger = int16_t;
constexpr largeint limit { static_cast<largeint>(std::numeric_limits<testinteger>::max())};
constexpr size_type iterations{100'000'000};


void checkaddition(){
    size_type overflows{};
    size_type additions{};
    std::cout << "testing... up to " << static_cast<unsigned long long>(limit) << std::endl;
    constexpr auto from= std::numeric_limits<testinteger>::min();
    if constexpr (sizeof(testinteger) < 4){
    for(largeint x{from}; x <= limit; x+=1 ){
        for(largeint y{from}; y <= limit; y+=1 ){
        	auto const result { static_cast<largeint>(x)+(y)  };
    		if (result > limit || result < -limit-1 ) ++overflows;
            ++additions;


        }
//        std::cout << ((x%80)?'.':'\n') << std::flush ;
    }
    } else { // monte-carlo attempt
    	std::cout << "monte carlo\n";
		std::random_device dev { };
        std::mt19937 rng{dev()};
        std::uniform_int_distribution<std::mt19937::result_type> rand(std::numeric_limits<testinteger>::min(),std::numeric_limits<testinteger>::max()); // distribution in range
    	for (size_type n=0; n < iterations; ++n){
			const testinteger x = rand(rng);
			const testinteger y = rand(rng);
    		auto result = static_cast<largeint>(x)+(y);
    		if (result > limit || result < -limit-1 ) ++overflows;
    		++additions;
    	}

    }
    std::cout << "\noverflows : " << static_cast<unsigned long long>(overflows) << '\n';
    std::cout << "additions : " << static_cast<unsigned long long>(additions) << '\n';
    std::cout << "ratio: " << std::setprecision(15)<< (double)overflows / additions << '\n';

}

void checkmultiplication(){
    size_type overflows{};
    size_type multiplications{};
    std::cout << "testing... up to " << static_cast<unsigned long long>(limit) << std::endl;
    if constexpr (sizeof(testinteger) < 4){
    for(largeint x{}; x < limit; x+=1 ){
        for(largeint y{}; y < limit; y+=1 ){
            if (static_cast<largeint>(x)*(y) > limit) ++overflows;
            ++multiplications;


        }
 //       std::cout << ((x%80)?'.':'\n') << std::flush ;
    }
    } else { // monte-carlo attempt
    	std::cout << "monte carlo\n";
		std::random_device dev { };
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> rand(std::numeric_limits<testinteger>::min(),std::numeric_limits<testinteger>::max()); // distribution in range
    	for (size_type n=0; n < iterations; ++n){
    		testinteger const x = rand(rng);
    		testinteger const y = rand(rng);
    		largeint const result{ static_cast<largeint>(x)*(y) };
    		if (result > limit || result < -limit-1 ) ++overflows;
    		++multiplications;
    	}

    }
    std::cout << "\noverflows : " << static_cast<unsigned long long>(overflows) << '\n';
    std::cout << "multiplications : " << static_cast<unsigned long long>(multiplications) << '\n';
    std::cout << "ratio: " << std::setprecision(15)<< (double)overflows / multiplications << '\n';

}

auto numberofvalidmultiplications16u(){
	uint64_t counter{};
	for (auto x = 0ull; x < 0x1'0000ull ; ++x){
		for (auto y = 0ull; y < 0x1'0000ull ; ++y){
			if (x*y <= std::numeric_limits<uint16_t>::max())
				++counter;
		}
	}
	return counter;
}
auto numberofvalidmultiplications16signed(){
	auto validrange = [](int64_t mul) { return mul < std::numeric_limits<int16_t>::max()
	                                        && mul >= std::numeric_limits<int16_t>::min();};
	uint64_t counter{};
	for (int64_t x = std::numeric_limits<int16_t>::min(); x <= std::numeric_limits<int16_t>::max() ; ++x){
		for (int64_t y = 0; y <= std::numeric_limits<int16_t>::max()+1 ; ++y){

			auto mul1 = x * y;
			auto mul2 = x * -y;
			if (validrange(mul2))
				++counter;
			if ( y <= std::numeric_limits<int16_t>::max() && validrange(mul1) )
				++counter;
			if (not validrange(mul2) && not validrange(mul1)) break;
		}
	}
	return counter;
}

auto numberofvalidmultiplications32signed(){
	auto validrange = [](largeint mul) { return mul < std::numeric_limits<int32_t>::max()
	                                        && mul >= std::numeric_limits<int32_t>::min();};
	uint64_t counter{};
	for (largeint x = std::numeric_limits<int32_t>::min(); x <= std::numeric_limits<int32_t>::max() ; ++x){
		for (largeint y = 0; y <= std::numeric_limits<int32_t>::max()+1ull ; ++y){

			auto mul1 = x * y;
			auto mul2 = x * -y;
			if (validrange(mul2))
				++counter;
			if ( y <= std::numeric_limits<int32_t>::max() && validrange(mul1) )
				++counter;
			if (not validrange(mul2) && not validrange(mul1)) break;
		}
	}
	return counter;
}

int main(){
	std::cout << "counting valid 16bit multiplications: ";
	auto count =  numberofvalidmultiplications16u();
	std::cout << count << std::endl;
	std::cout << "out of :" << 0x1'0000'0000 << std::endl;
	std::cout << std::setprecision(20) << count/(long double)0x1'0000'0000 << std::endl;
	std::cout << std::setprecision(20) << 1.l - count/(long double)0x1'0000'0000ull << std::endl;


	auto countsigned16 = numberofvalidmultiplications16signed();
	std::cout << "-------------------\nnumber of 16bit signed multiplications: " << countsigned16 << '\n'
	          << "hex : "<< std::hex << countsigned16 << std::dec<<'\n';
	std::cout << "out of :" << 0x1'0000'0000 << std::endl;
	std::cout << std::setprecision(20) << 1.l - countsigned16/(long double)0x1'0000'0000ull << std::endl;


	auto countsigned32 = 198788215489; // numberofvalidmultiplications32signed(); precomputed
	std::cout << "-------------------\nnumber of 32bit signed multiplications: " << countsigned32 << '\n'
	          << "hex : "<< std::hex << countsigned32 << std::dec<<'\n';
	largeint numberofmultiplications = std::numeric_limits<uint64_t>::max();
	numberofmultiplications+=1;// 2**64
	countsigned32 >>= 1;
	numberofmultiplications >>=1;
	std::cout << std::setprecision(20) << 1.l - (long double)countsigned32/(long double)numberofmultiplications << std::endl;


//	std::thread add{checkaddition};
//	std::thread mul{checkmultiplication};
//
//	std::cout << "\ndivision by zero prob:" << std::fixed<< std::setprecision(20)<< 2.0l / ((long double)0x1'0000'0000ull ) << '\n';
//	add.join();
//	mul.join();
}
