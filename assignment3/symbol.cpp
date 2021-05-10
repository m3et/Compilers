#include "symbol.h"
#include "strings.h"

Type Symbol::getType(){
    return this->type;
}

int Symbol::getSymbol(){
    return this->symbol;
}

void Symbol::printSymbol(){
    if(this->getType() == NONTERM)
        std::cout << tokenStrings[this->getSymbol()] << ' ';
    else
        std::cout << terminalStrings[this->getSymbol()] << ' ';
}

