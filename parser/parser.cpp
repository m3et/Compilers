#include "parser.h"

void Parser::parse()
{
	// Push END and First Rule to stack
	stack.push_back(Symbol(END, TERM));
	stack.push_back(Symbol(E, NONTERM));
	// let a be first token from file
	terminal a = this->nextToken();
	// let X be top of stack = First Rule E
	Symbol X = stack.back();

	// stack !isEmpty()
	while (X.getSymbol() != END)
	{
		// if( X = a )
		if (X.getType() == TERM && X.getSymbol() == static_cast<int>(a))
		{
			printLM();
			// push to accepted input input
			this->accepted.push_back(X);
			// pop the stack
			this->stack.pop_back();
			// let a be next token from file
			a = this->nextToken();
		}
		else if (X.getType() == TERM)
		{
			// Ho no you are not making any sense
			printLM();
			std::cout << "syntax error" << std::endl;
			return;
		}
		else if (table.at(X.getSymbol()).at(static_cast<int>(a)) == -1)
		{
			// Ho no you are not obeying the rules
			printLM();
			std::cout << "syntax error" << std::endl;
			return;
		}
		else
		{
			// output the production X -> Y1Y2...Yk
			printLM();
			stack.pop_back();
			// push YkYk-1...Y1 into stack
			for (auto &it : rules.at(table.at(X.getSymbol()).at(static_cast<int>(a))))
			{
				stack.push_back(it);
				// remove EPSILON from stack
				if (it.getType() == TERM && it.getSymbol() == EPSILON)
					stack.pop_back();
			}
		}

		X = stack.back();
	}
	printLM();
}

void Parser::printLM()
{
	// output the stack production Y1Y2...Yk
	for (auto it : accepted)
		it.printSymbol();

	std::cout << "| ";

	// output the accepted production YkYk-1...Y1
	for (auto it = stack.rbegin(); it != stack.rend() - 1; ++it)
		it->printSymbol();

	std::cout << std::endl;
}

terminal Parser::nextToken()
{
	std::string word;
	this->inputFile >> word;

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