#include "lib.hpp"
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
int main(int argc, char * argv[]) {
	boost::filesystem::path lmPath(argv[0]);
	lmPath.remove_filename();
	Recognizer rec(lmPath.generic_string());

		std::cout << "\nSpeak Now : \n";
		std::string hyp = rec.recognize_one();
		std::cout << "\nYou Said : "<< hyp 
		 << "\n";
		if(hyp == "Exit") {
			return 0;
		}
	
}