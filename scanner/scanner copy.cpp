#include "scanner.h"
#include <regex>
#include <cctype>

shared_ptr<Token> Scanner::nextToken()
{
	char prev = 0;
	while (this->nextChar())
	{
		string token(1, ch);
		// skip whitespaces
		if (std::regex_match(token, std::regex("/s")))
		{
			std::string s(1, ch);
			while (std::regex_match(s, std::regex("/s")) && this->nextChar())
				;
			{
			}
			this->inputFile.unget();
		}

		// check for comment opening
		// // skip comment of type /* COMMENT */ from COMMENT start postion until the end
		// // skip comment of type // COMMENT from //^ to ENDLINE
		if (ch == '/')
		{
			prev = this->ch;
			char commentType;
			if (this->nextChar() && (ch == '*' || ch == '/'))
			{
				if (ch == '*')
					commentType = '*';
				else if (ch == '/')
					commentType = '/';

				char curr, prev = 0;
				bool flag = true;
				while (this->nextChar() && flag)
				{
					curr = this->ch;
					if (commentType == '*' && prev == '*' && curr == '/')
						flag = false;

					if (commentType == '/' && curr == '\n')
						flag = false;

					prev = curr;
				}
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
		//	-- or -> or -
		else if (ch == '-')
		{
			if (this->nextChar())
			{

				if (this->ch == '-')
				{
					return shared_ptr<Token>(new Token(INC_OP, "--"));
				}
				if (ch == '>')
				{
					return shared_ptr<Token>(new Token(PTR_OP, "->"));
				}
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
			bool flag = true;
			while (flag && this->nextChar())
			{
				if (isdigit(ch) || ch == 'E' || ch == 'e' || ch == '.')
				{
					constant.push_back(ch);
				}
				else
				{
					this->inputFile.unget();
					flag = false;
				}
			}
			if (std::regex_match(constant, std::regex("\\d+")) ||
				std::regex_match(constant, std::regex("\\d+[eE][+-]?\\d+")) ||
				std::regex_match(constant, std::regex("\\d*\\.\\d+([eE]?[+-]?\\d+)?")) ||
				std::regex_match(constant, std::regex("\\d+\\.\\d*([eE]?[+-]?\\d+)?")))
			{
				return shared_ptr<Token>(new Token(CONSTANT, constant));
			}
			else
			{
				if (constant == ".")
				{
					return shared_ptr<Token>(new Token(static_cast<tokenType>('.'), constant));
				}

				// this->inputFile.unget();
				return shared_ptr<Token>(new Token(ERROR, constant));
			}
		}
		// IDENTIFIER
		else if (isalpha(this->ch) || this->ch == '_')
		{
			string identifier(1, ch);
			bool flag = true;
			while (flag && this->nextChar())
			{
				if ((isalpha(this->ch) || isdigit(this->ch) || this->ch == '_'))
					identifier.push_back(this->ch);
				else
				{
					this->inputFile.unget();
					flag = false;
				}
			}

			if (std::regex_match(identifier, std::regex("[a-zA-Z_][a-zA-Z0-9_]*")))
			{
				shared_ptr<Token> tokenptr = this->symTab.lookupToken(identifier);
				if (tokenptr)
				{
					shared_ptr<set<int>> setptr = (tokenptr->getLines());
					if (setptr)
					{ // its an varToken
						setptr->insert(this->lineno);
						return tokenptr;
					} // its an reserved word Token
					else
					{
						return tokenptr;
					}
				}
				else // not in map
				{
					// add varToken to map
					shared_ptr<Token> tokenptr(new varToken(identifier));
					tokenptr->add_line(this->lineno);
					this->symTab.insertToken(identifier, tokenptr);
				}
				return shared_ptr<Token>(new varToken(identifier));
			}
		}
		else if (this->ch == '\'')
		{
			char c = 0;
			if (this->nextChar() && isalpha(this->ch))
			{
				c = this->ch;
				if (this->nextChar() && this->ch == '\'')
				{
					/* code */
					return shared_ptr<Token>(new Token(CONSTANT, string(1, c)));
				}
			}
			return shared_ptr<Token>(new Token(ERROR, string(1, c)));
		}
		else if (this->ch == '"')
		{
			bool flag = true;
			std::string str;
			while (flag && this->nextChar())
			{
				if (this->ch == '"')
				{
					flag = false;
				}
				else
				{
					str.push_back(this->ch);
				}
			}
			return shared_ptr<Token>(new Token(STRING_LITERAL, str));
		}
	}

	return nullptr;
}

