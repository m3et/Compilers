#include "parser.h"

void Parser::parse()
{
	terminal a = this->nextToken();

	stack.push_back(Symbol(terminal::END, TERM));
	stack.push_back(Symbol(nonTerminal::E, NONTERM));
	Symbol X = stack.back();

	/*
	enum terminal { ID , PLUS , ASTERISK , LPAREN , RPAREN , END , EPSILON };
	enum nonTerminal { E , E_ , T , T_ , F , ERROR };
	enum Type { TERM , NONTERM };

	*/
	while (X.getSymbol() != terminal::END)

	{
		if (X.getType() == TERM && X.getSymbol() == static_cast<int>(a))
		{
			accepted.push_back(X);
			stack.pop_back();
			a = this->nextToken();
		}
		else if (X.getType() == TERM)
		{
			std::cout << "syntax error 1" << std::endl;
			return;
		}
		else if (table.at(X.getSymbol()).at(static_cast<int>(a)) == -1)
		{
			std::cout << "syntax error 2" << std::endl;
			return;
		}
		else
		{
			printLM();
			stack.pop_back();
			// for (auto it = rules.at(tableVal).begin(); it != stack.end(); ++it)
			// {
			// 	stack.push_back(*it)
			// }
			for (auto &it : rules.at(table.at(X.getSymbol()).at(static_cast<int>(a))))
			{
				stack.push_back(it);
				if (it.getType() == TERM && it.getSymbol() == terminal::EPSILON)
					stack.pop_back();
			}
		}
		X = stack.back();
	}
}

void Parser::printLM()
{
	/*
	for (auto it = s.begin(); it != s.end(); it++) {
		cout << *it.printSymbol() << endl;
	}

	for (vector<Symbol>::reverse_iterator i = my_vector.rbegin(); i != my_vector.rend(); ++i ) { 
		print
	}
} 
*/
	for (auto it = accepted.begin(); it != accepted.end(); ++it)
		it->printSymbol();

	std::cout << "| ";

	for (auto it = stack.rbegin(); it != stack.rend() - 1; ++it)
		it->printSymbol();

	std::cout << std::endl;
}
// enum terminal { ID , PLUS , ASTERISK , LPAREN , RPAREN , END , EPSILON };
terminal Parser::nextToken()
{
	std::string word;
	this->inputFile >> word;
	int tokenVal;

	if (word == "id")
		return terminal::ID;
	else if (word == "+")
		return terminal::PLUS;
	else if (word == "*")
		return terminal::ASTERISK;
	else if (word == "(")
		return terminal::RPAREN;
	else if (word == ")")
		return terminal::LPAREN;
	else if (word == "$")
		return terminal::END;

	return terminal::EPSILON;
}