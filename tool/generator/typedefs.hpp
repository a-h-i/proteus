#pragma once
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
namespace gen {
typedef std::string id_t;
typedef std::string sentence_t;
typedef std::shared_ptr<sentence_t> sentence_ptr_t;
typedef std::unordered_map<gen::sentence_t, gen::id_t> sent_map_t;

struct sentValueHash {
    std::size_t operator()( const sentence_ptr_t &sent ) const {
        return std::hash<std::string>()( *sent );
    };
};

struct sentValueEq {
    bool operator()( const sentence_ptr_t lhs, const sentence_ptr_t rhs ) const {
        return *lhs == *rhs;
    }
};
}