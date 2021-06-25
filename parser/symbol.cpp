#include "symbol.h"
#include "strings.h"

void Symbol::printSymbol()
{
  if (this->getType() == TERM)
    std::cout << terminalStrings[this->getSymbol()] << ' ';
  else
    std::cout << tokenStrings[this->getSymbol()] << ' ';
}

Type Symbol::getType()
{
  return this->type;
}

int Symbol::getSymbol()
{
  return this->symbol;
}