#include "../template.hpp"
#include "../repeating_emission.hpp"
#include "../plain_text_emission.hpp"
#include <memory>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/filesystem/fstream.hpp>


static const boost::regex fileBlockBegin(
    R"(\s*file\s*([a-zA-Z0-9]+)\s*\{\s*)" ); // match[1] is name
static const boost::regex fileBlockEnd( R"(\s*USING_BRACES_WAS_A_GREAT_IDEA\}\s*)" );
static const boost::regex wantTheDictionary( "THE_DICT" );
static const boost::regex wantTheGrammar( "THE_GRAMMAR" );
static const boost::regex idOfCommand(
     R"(ID_OF_COMMAND\(\s*([a-zA-Z0-9 ]+)\s*\))" ); // match[1] is command
static const boost::regex beginExpandBlock( R"(\s*EXPAND_ME_BEGIN\s*)" );
static const boost::regex endExpandBlock( R"(\s*EXPAND_ME_END\s*)" );
static const boost::regex putCommandHere("COMMAND_EX");

using namespace emitter;

class Stuff {

    std::list<gen::sentence_t> sentences_;
    bool sentencesInitialzed;
public:
    const bfs::path dict, grammar;
    bfs::ifstream input;
    std::list<std::unique_ptr<Emission>> seq;
    const sent_map_t *const map;
    const logger_ptr_t logger;
    

    Stuff( const bfs::path &dict, const bfs::path &grammar,
           const bfs::path templateFile, const sent_map_t * const map,
           const logger_ptr_t logger ) :sentencesInitialzed(false), dict( dict ),
        grammar( grammar ), input( templateFile ), map( map ), logger( logger ) {}

    std::list<gen::sentence_t> & sentences() {
        // initialize if needed
        if(sentencesInitialzed) {
            return sentences_;
        }else {
            for(auto &pair : *map) {
                sentences_.push_back(pair.first); // already a shredptr
            }
            sentencesInitialzed = true;
            return sentences_;
        }
    }

};

static std::string replaceVariables( Stuff &data, const std::string &text ) {
    std::string out =  boost::regex_replace( text, wantTheGrammar,
                       data.grammar.generic_string() );
    out = boost::regex_replace( out, wantTheDictionary,
                                data.dict.generic_string() );
    auto formatter = [&data]( const boost::smatch & s ) -> std::string {
        if ( data.map->count( s[1] ) != 0 ) {
            return data.map->at( s[1] );
        } else {
            *( data.logger ) << "\nWARNING: COULD NOT FIND ID-OF : " << s[1] << '\n';
            return s[0]; // return original
        }

    };
    return boost::regex_replace( out, idOfCommand, formatter );

}

static void handleExpandBlock(Stuff &data, std::shared_ptr<bfs::ofstream> &writer) {
    std::string mold;
    mold.reserve(1024);
    for(std::string line; std::getline(data.input, line); ) {
        if(boost::regex_match(line, endExpandBlock)) {
            break;
        }else {
            mold += line + '\n';
        }
    }
    mold.shrink_to_fit();
    data.seq.emplace_back(new RepeatingEmission(writer, mold, putCommandHere, data.sentences() ));

}

void handleFile( Stuff &data, const std::string fileName ) {
    std::shared_ptr<bfs::ofstream> writer( new bfs::ofstream( fileName ) );
    std::ostringstream buffer;
    bool matchedClose = false ;
    for ( std::string line; std::getline( data.input, line ); ) {
        if ( boost::regex_match( line, fileBlockEnd ) ) {
            matchedClose = true;
            break; // we are done here
        } else if ( boost::regex_match( line, beginExpandBlock ) ) {
            data.seq.emplace_back( new PlainTextEmission( writer, replaceVariables( data,
                                    buffer.str() ) ) );
            buffer.str("");
            buffer.clear(); // reset
            handleExpandBlock( data, writer ); // consumes end expand block
        } else {
            // plain text
            buffer << line << '\n'; // restore newline
        }
    }
    // might be empty
    data.seq.emplace_back( new PlainTextEmission( writer, replaceVariables( data,
                            buffer.str() ) ) );
    if(!matchedClose) {
        *data.logger << "\nWarning: Did not match closing file declaration for file : " << fileName << '\n';
    }
    
}

std::list<std::unique_ptr<Emission>>  emitter::createSequence(
                                      const bfs::path &templateFile, const bfs::path &dict,
                                      const bfs::path &grammar, const sent_map_t *const map,
                                      logger_ptr_t logger ) {
    Stuff  data( dict, grammar, templateFile, map, logger ); // We need our stuffz

    //We begin parsing ! currently only file blocks are supported
    for ( std::string line; std::getline( data.input, line ) ; ) {
        // search for begining of a file block
        boost::smatch result;

        if ( boost::regex_match( line, result, fileBlockBegin ) ) {
            handleFile( data, result[1] ); // handle file consumes end of file block
        }
    }

    return std::move(data.seq);

}