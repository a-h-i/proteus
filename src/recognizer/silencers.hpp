#pragma once
#include <cmath>
#include <cstdint>
#include <functional>

/************************************
 * Silence detectors                *
 * Inappropriately called silencers *
 *                                  *
 ********************************** */

namespace silencers {
    typedef std::function<bool (const std::int16_t *, const std::size_t)> silencer_t;

/*********************************************************
 * Calculates amplitude gain using RMS for averaging.    *
 * I am not sure the previous statement makes any sense. *
 *                                                       *
 *********************************************************/
    class RMSSilencer {
        const double threshold;
    public:
        RMSSilencer(double threshold) : threshold(threshold){} 
        bool operator()(const std::int16_t *data, const std::size_t length) const {
            double squareSum =0.0;
            for(std::size_t i = 0; i < length; i++ ) {
                squareSum += std::pow(data[i], 2.0); // safe promotion                
            }
            squareSum /= length;
            double rms = std::sqrt(squareSum);
            
            // calculate decibel
            // max db is 0
            double db =  20.0 * std::log10(rms / 32767.0);
            return db <= threshold; 

        }
    };
}

