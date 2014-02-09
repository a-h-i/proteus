#pragma once
#include <cstdint>
#include <vector>

namespace filters {
typedef std::function< std::vector<std::int16_t> ( const std::int16_t *, const std::size_t )> filter_t;


/****************************************************************
 * out[i] = (1/M) Σ in[i + j]  : start from j = -(M/2) to (M/2) *
 * M must be odd.     										    *
 * i.e if M = 4 kernel is :                                     *
 * [1/10, 1/10, 1/10, 1/10, 10/10] kernel[2] is the center      *
 * which is aligned with in[i]                                  *
 ****************************************************************/
class  SymmetricalMovingAverageFilter {
    const int half;
    const int weight;
public:
    SymmetricalMovingAverageFilter( int M ): half((M-1) /2), weight(M) {
    	if(M % 2  != 1) {
    		throw std::logic_error("Kernel size must be odd");
    	}
    }
    std::vector<std::int16_t> operator()( const std::int16_t *data, const std::size_t length ) const {

    	std::size_t outLength = length - weight - 1;
    	std::vector<std::int16_t> out(outLength, 0);


    	// for all out value
    	for(int i = half; i < (length - half); i++) {

    		// each value is the average of M  input values startring at data[i - half] to data[i + half]
    		std::int16_t value = 0;
    		for(int j = -half; j <= half; j++) {
    			value += data[i + j];
    		}
    		value /= weight;
    		out[i - half] = value;
    	}
    	return out;
    }
};
}