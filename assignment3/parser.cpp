#include "parser.h"

void Parser::parse()
{
	terminal a = this->nextToken();

	stack.push_back(Symbol(terminal::END, TERM));
	stack.push_back(Symbol(nonTerminal::E, NONTERM));
	Symbol X = stack.back();

	/*
	const string terminalStrings[] = { "id" , "+" , "*" , "(" , ")" , "$" , "e" };
	const string tokenStrings[] = { "E" , "E_" , "T" , "T_" , "F" };

	enum terminal { ID , PLUS , ASTERISK , LPAREN , RPAREN , END , EPSILON };
	enum nonTerminal { E , E_ , T , T_ , F , ERROR };
	enum Type { TERM , NONTERM };

	*/

	while (X.getSymbol() != terminal::END)
	{
		// cout << "X is " << X.getSymbol() << "\t Symbole is ";
		// cout << endl;
		// cout << "a is " << a << endl;

		if (X.getType() == TERM && X.getSymbol() == static_cast<int>(a))
		{
			printLM();
			accepted.push_back(X);
			stack.pop_back();
			a = this->nextToken();
		}
		else if (X.getType() == TERM)
		{
			printLM();
			std::cout << "syntax error" << std::endl;
			return;
		}
		else if (table.at(X.getSymbol()).at(static_cast<int>(a)) == -1)
		{
			printLM();
			std::cout << "syntax error" << std::endl;
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
	printLM();
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
		return terminal::LPAREN;
	else if (word == ")")
		return terminal::RPAREN;
	else if (word == "e")
		return terminal::EPSILON;

	return terminal::END;
}