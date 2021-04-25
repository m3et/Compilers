#include "scanner.h"
#include <regex>
#include <cctype>

shared_ptr<Token> Scanner::nextToken()
{
	char prev = 0;
	//
	while (this->nextChar())
	{
		string token(1, ch);
		// skip whitespaces
		if (std::regex_match(token, std::regex("/s")))
		{
			skipWhitespace();
		}
		// check for comment opening
		else if (ch == '/')
		{
			prev = this->ch;
			if (this->nextChar())
			{
				if (ch == '*')
					this->skipComment('*');
				else if (ch == '/')
					this->skipComment('/');
			}
			else
			{
				this->ch = prev;
				this->inputFile.unget();
			}
			// creat a token
			// return shared_ptr<Token>(new Token(static_cast<tokenType>(ch), token));
		}
		switch (ch) // each character represents itself
		{
		case ';':
		case '{':
		case '}':
		case ',':
		case ':':
		case '(':
		case ')':
		case '[':
		case ']':
		case '~':
		case '*':
		case '%':
		case '^':
		case '?':
		case '/':
			return shared_ptr<Token>(new Token(static_cast<tokenType>(ch), string(1, ch)));
			break;
		}
		//	++ or +
		if (ch == '+')
		{
			if (this->nextChar() && ch == '+')
			{
				return shared_ptr<Token>(new Token(INC_OP, "++"));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(static_cast<tokenType>('+'), string(1, '+')));
			}
		}
		//	-- or -
		else if (ch == '-')
		{
			if (this->nextChar() && ch == '-')
			{
				return shared_ptr<Token>(new Token(INC_OP, "--"));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(static_cast<tokenType>('-'), string(1, '-')));
			}
		}
		//	-> or -
		else if (ch == '-')
		{
			if (this->nextChar() && ch == '>')
			{
				return shared_ptr<Token>(new Token(PTR_OP, "->"));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(static_cast<tokenType>('-'), string(1, '-')));
			}
		}
		//	&& or &
		else if (ch == '&')
		{
			if (this->nextChar() && ch == '&')
			{
				return shared_ptr<Token>(new Token(AND_OP, "&&"));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(static_cast<tokenType>('&'), string(1, '&')));
			}
		}
		//	|| or |
		else if (ch == '|')
		{
			if (this->nextChar() && ch == '|')
			{
				return shared_ptr<Token>(new Token(OR_OP, "||"));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(static_cast<tokenType>('|'), string(1, '|')));
			}
		}
		//	<= or <
		else if (ch == '<')
		{
			if (this->nextChar() && ch == '=')
			{
				return shared_ptr<Token>(new Token(LE_OP, "<="));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(static_cast<tokenType>('<'), string(1, '<')));
			}
		}
		//	>= or >
		else if (ch == '>')
		{
			if (this->nextChar() && ch == '=')
			{
				return shared_ptr<Token>(new Token(GE_OP, ">="));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(static_cast<tokenType>('>'), string(1, '>')));
			}
		}
		//	== or =
		else if (ch == '=')
		{
			if (this->nextChar() && ch == '=')
			{
				return shared_ptr<Token>(new Token(EQ_OP, "=="));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(static_cast<tokenType>('='), string(1, '=')));
			}
		}
		//	!= or !
		else if (ch == '!')
		{
			if (this->nextChar() && ch == '=')
			{
				return shared_ptr<Token>(new Token(NE_OP, "!="));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(static_cast<tokenType>('!'), string(1, '!')));
			}
		}
		// CONSTANT
		else if (isdigit(ch) || ch == '.')
		{
			string constant(1, ch);
			while (this->nextChar() && (isdigit(ch) || ch == 'E' || ch == 'e' || ch == '.'))
			{
				constant.push_back(ch);
			}
			if (std::regex_match(constant, std::regex("\\d+")) ||
				std::regex_match(constant, std::regex("\\d+[eE][+-]?\\d+")) ||
				std::regex_match(constant, std::regex("\\d*\\.(\\d+[eE]?[+-]?\\d+)?")) ||
				std::regex_match(constant, std::regex("\\d+\\.(\\d*[eE]?[+-]?\\d+)?")))
			{
				return shared_ptr<Token>(new Token(CONSTANT, constant));
			}
			else
			{
				this->inputFile.unget();
				return shared_ptr<Token>(new Token(ERROR, constant));
			}
		}
		else if (isalpha(this->ch) || this->ch == '_')
		{
			string identifier(1,ch);
			while (this->nextChar() && (isalpha(this->ch) || this->ch == '_'))
			{
				identifier.push_back(this->ch);
			}
			
			if(std::regex_match(identifier, std::regex("[a-zA-Z_][a-zA-Z0-9_]*")))
				
				return shared_ptr<Token>(new Token(IDENTIFIER, identifier));
			else


		}
		
	}

	return NULL;
}

// skip all whitespaces from postion until a char appears
void Scanner::skipWhitespace()
{
	std::string s(1, ch);
	while (std::regex_match(s, std::regex("/s")) && this->nextChar())
		;
	{
	}
	this->inputFile.unget();
}

// skip comment of type /* COMMENT */ from COMMENT start postion until the end
// skip comment of type // COMMENT from //^ to ENDLINE
void Scanner::skipComment(char commentType)
{
	char curr, prev = 0;
	while (this->nextChar())
	{
		curr = ch;
		if (commentType == '*' && curr == '/' && prev == '*')
			return;

		if (commentType == '/' && curr == '\n')
			return;
		prev = curr;
	}
}
