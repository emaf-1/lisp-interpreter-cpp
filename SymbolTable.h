//
//  SymbolTable.h
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#ifndef SymbolTable_h
#define SymbolTable_h

#include <string>
#include <sstream>
#include <unordered_map>
#include "Exceptions.h"

class SymbolTable {
    
public:
    SymbolTable() = default;
    ~SymbolTable() = default;
    
    SymbolTable(const SymbolTable& other) = delete;
    SymbolTable& operator=(const SymbolTable& other) = delete;

    //Recover integer value with the variable name, throws EvaluationError if the variable is undeclared
    int64_t getValue(std::string const& key) const {
        auto itr = map.find(key);
        if (itr == map.end()) {
            std::stringstream temp;
            temp << "Undefined variable " << key;
            throw EvaluationError{ temp.str() };
        }
        return (*map.find(key)).second;
    }
    //Inserts a new variable or updates the value of an existing variable
    void setValue(std::string const& key, int64_t value) {
        map[key] = value;
    }
    
private:
    std::unordered_map<std::string, int64_t> map;
};

#endif /* SymbolTable_h */
