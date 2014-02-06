#pragma once
#include <string>
#include <memory>
#include <functional>
namespace gen {
typedef std::string id_t;
typedef std::shared_ptr<std::string> sentence_t;
struct sentValueHash {
    std::size_t operator()( const sentence_t &sent ) const {
        return std::hash<std::string>()( *sent );
    };
};

struct sentValueEq {
    bool operator()( const sentence_t lhs, const sentence_t rhs ) const {
        return *lhs == *rhs;
    }
};
}