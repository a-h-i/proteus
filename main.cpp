#include "lib.hpp"
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
int main(int argc, char * argv[]) {
	boost::filesystem::path lmPath(argv[0]);
	lmPath.remove_filename();
	Recognizer rec(lmPath.generic_string());

	
		std::string hyp = rec.recognize_one();
		std::cout << "You Said" << hyp;
		if(hyp == "Exit") {
			return 0;
		}
	
}