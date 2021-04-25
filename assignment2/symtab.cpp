#include "symtab.h"

void SymbolTable::initReserved()
{
	std::ifstream infile("reserved.txt");
	string word;
	int value;
	shared_ptr<Token> token;

	// go over reserved.txt file line by line
	while (infile >> word >> value)
	{
		token = make_shared<Token>(Token(static_cast<tokenType>(value), word)); // creat a token from reserved word
		this->insertToken(word, token);											// insert token to map
	}
}

shared_ptr<Token> SymbolTable::lookupToken(string text)
{
	auto it = this->symMap.find(text);
	return it->second;
}

void SymbolTable::insertToken(string text, shared_ptr<Token> tokenp)
{
	// cout << "Insert " << text << endl;
	this->symMap.insert(std::pair<string, shared_ptr<Token>>(text, tokenp));
}

void SymbolTable::xref()
{
	// go over map elemnts
	for (auto elem : this->symMap)
	{
		shared_ptr<set<int>> s = (elem.second->getLines());
		// std::cout << elem.first << " " << elem.second->printToken() << " " << "\n";
		if (s)
		{
			cout << elem.first << "\t";
			for (auto f : *s)
			{
				// use f here
				cout << f << " ";
			}
			cout << endl;
		}
	}
}
