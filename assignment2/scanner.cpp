#include "scanner.h"
#include <regex>

shared_ptr<Token> Scanner::nextToken(){

	// 
	while (this->nextChar())
	{	
		std:string token(1,this->ch);
		// skip whitespaces
		if(std::regex_match (token, std::regex("/s"))){
			skipWhitespace();
		}
		// check for comment opening
		if(this->ch == '/'){
			if(this->nextChar() && this->ch == '*'){



			}else{
				// creat a token 
				return shared_ptr<Token>
					(new Token(static_cast<tokenType>(this->ch), token));
				this->inputFile.unget();
			}


		}

	}
	
}

// skip all whitespaces from postion until a char appears
void Scanner::skipWhitespace(){
	std::string s(1, this->ch);
	while (std::regex_match (s, std::regex("/s"))
	{
		this->nextChar();
	}
	this->inputFile.unget();
}

void Scanner::skipComment()
{
	char c, prev = 0; 
	while (c = input())
	{
		if (c == '/' && prev == '*')
			return;
		prev = c;
	}
}
