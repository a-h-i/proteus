#include "../arguments.hpp"
#include <iostream>
static void printHelp() {
    static std::string helpString =
    R"(
        Invokation:
        proteus [options] [configuration] [template]
            Order of arguments is not important. 
            Atlease one configuration file must be 
            provided. All configuration files should
            have a .cfg suffix. Any number of tempalte
            files can be provided. All template 
            files must have a .tmpl suffix.
            options:
                -Wall : 
                    Enable all warnings for configuration file.
                -WUnused : 
                    Warn about unused variables in configuration file.
                -WDuplicate :
                    Warn about duplicate sentences in configuration file.
                -WReAssign :
                    Warn on variable re-assignment in configuration file.
                Warning levels can be combined. Default is none.
        )";

    std::cout  << "\n" << helpString << "\n";
}
static void printAbout() {
    static std::string aboutString = 
    R"(
        proteus v1.0
        This software is released under the MIT license.
       )"
}


std::list<std::string> ui::convertArgs( const int argc,
        const char *const *const argv ) {
    // preserves order
    std::list<std::string> args;

    for ( int i = 0; i < argc; i++ ) {
        args.emplace_back( argv[i] );
    }

    return args;
}


std::unordered_multimap<OptionKey, std::string> getConfiguration(std::list<std::string> &args){
    std::unordered_multimap<OptionKey, std::string> config;
    // path contains file name
    boost::filesystem::path execDir = args.front();
    execDir.remove_filename();
    options.emplace(OptionKey::EXECUTABLE_DIRECTORY, execDir.native_string());
    args.pop_front(); // consumed path to exe
    // now we need to scan the input for configuration files
     for ( auto it = args.begin(); it != args.end(); ++it ) {
        if ( boost::ends_with( *it, ".cfg" ) ) {
            // add configuration file
            config.emplace(OptionKey::CONFIGURATION_FILES, *it );
            it = eraseItr( it, args );
        }else if(boost::ends_with( *it, ".tmpl" )) {
            config.emplace(OptionKey::TEMPLATE_FILES, *it );
            it = eraseItr( it, args );
        }else if (*it == "-h" | *it == "--help") {
            printHelp();
            throw proteus::exceptions::NoWorkException();
        }else if(*it == "--about") {
            printAbout();
            throw proteus::exceptions::NoWorkException;
        }else  {
            // unknown argument
            throw proteus::exceptions::InvalidArgs(*it);
        }
    }   

}