#include "../repeating_emission.hpp"

void emitter::RepeatingEmission::emit() {
    if(file->good()) {

        while(!range.empty()) {
            boost::regex_replace(std::ostream_iterator<char>(*file), mold.begin(), mold.end(), pattern, *range.front());
            range.pop_front();
        }

    }else {
        throw proteus::exceptions::EmissionError("File not good - RepeatingEmission");
    }
}