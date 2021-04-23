#include <map>
#include "token.h"

class SymbolTable {
	map<string, shared_ptr<Token>> symMap;
	void initReserved();
public:
	SymbolTable () { initReserved(); }
	shared_ptr<Token> lookupToken(string text);
	void insertToken(string text, shared_ptr<Token> tokenp);
	void xref();
};

void SymbolTable::initReserved(){
	std::ifstream infile("reserved.txt");
	string word;
	int value;
	shared_ptr<Token> token;
	while(infile >> word >> value){
		// cout << "word: " << word << " value: " << value << endl;
		token = new Token(static_cast<tokenType>(value), word);
		this->insertToken(word, token);
	}
	
}


shared_ptr<Token> SymbolTable::lookupToken(string text){
	auto it = this->symMap.find(text);
	return it->second;

}

void SymbolTable::insertToken(string text, shared_ptr<Token> tokenp){
	this->symMap.insert(std::pair<string, shared_ptr<Token>>(text, tokenp));
}

void SymbolTable::xref(){
	for(auto elem : myMap)
	{
   		std::cout << elem.first << " " << elem.second << " " << "\n";
	}
}
