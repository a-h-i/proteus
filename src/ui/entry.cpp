#ifdef DEBUG_ME_SOFTLY
#define CATCH_CONFIG_MAIN 
#include "../tests/catch.hpp"
#else
int main(int argc, char * argv) {
    return 0;
}

#endif