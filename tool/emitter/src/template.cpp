#include "../template.hpp"
#include "../internal/template_parser.hpp"
void emitter::processTemplate( const std::string
                                  &templateFile, const std::string &dict,
                                  const std::string &grammar, const sent_map_t * const map , 
                                  logger_ptr_t logger ) {
    internal::TemplateParser tp(templateFile, dict, grammar, map, logger );
    tp.parse();
}