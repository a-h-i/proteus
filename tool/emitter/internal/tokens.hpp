#pragma once
#include <boost/regex.hpp>
namespace emitter {

namespace internal {
namespace tokens {
extern const boost::regex fileBlockBegin; // match[1] is name
extern const boost::regex fileBlockEnd;
extern const boost::regex wantTheDictionary;
extern const boost::regex wantTheGrammar;
extern const boost::regex idOfCommand; // match[1] is command
extern const boost::regex beginExpandBlock;
extern const boost::regex endExpandBlock;
extern const boost::regex putCommandHere;
extern const boost::regex putIDHere;
}
}
}