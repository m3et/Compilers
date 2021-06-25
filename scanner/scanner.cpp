#include "scanner.h"

#include <regex>

#include <cctype>

shared_ptr < Token > Scanner::nextToken() {
  while (this -> nextChar()) {
    string s(1, ch);
    std::regex spaceRegex("[[:space:]]+");
    // whitespaces
    if (std::regex_match(s, spaceRegex) && this -> nextChar()) {
      s.push_back(this -> ch);
    }

    bool flag = false;
    //comment
    if (ch == '/') {
      if (this -> nextChar()) {
        /* COMMENT */
        if (this -> ch == '*') {
          char prev = 0;
          char curr = this -> ch;
          while (this -> nextChar() && (prev != '*' || this -> ch != '/')) {
            prev = this -> ch;
            flag = true;
          }
        }
        // //COMMENT
        else if (this -> ch == '/') {
          while (this -> nextChar() && this -> ch != '\n')
          ;
          flag = true;
        } else {
          this -> inputFile.unget();
          this -> ch = '/';
        }
      }
    }

    if (flag)
      this -> nextChar();

    // each character represents itself
    switch (ch) {
		case ';':	case '{':	case '}':	case ',':	case ':':	case '(':	case ')':
		case '[':	case ']':	case '~':	case '*':	case '%':	case '^':	case '?':
		case '/':
			return shared_ptr < Token > (new Token(static_cast < tokenType > (ch), string(1, ch)));
			break;
    }
    //	++ or +
    if (ch == '+') {
      if (this -> nextChar() && ch == '+') {
        return shared_ptr < Token > (new Token(INC_OP, "++"));
      } else {
        this -> inputFile.unget();
        return shared_ptr < Token > (new Token(static_cast < tokenType > ('+'), string(1, '+')));
      }
    }
    //	-- or -> or -
    if (ch == '-') {
      if (this -> nextChar()) {

        if (ch == '>') {
          return shared_ptr < Token > (new Token(PTR_OP, "->"));
        }
        if (this -> ch == '-') {
          return shared_ptr < Token > (new Token(INC_OP, "--"));
        }
      } else {
        this -> inputFile.unget();
        return shared_ptr < Token > (new Token(static_cast < tokenType > ('-'), string(1, '-')));
      }
    }
    //	&& or &
    if (ch == '&') {
      if (this -> nextChar() && ch == '&') {
        return shared_ptr < Token > (new Token(AND_OP, "&&"));
      } else {
        this -> inputFile.unget();
        return shared_ptr < Token > (new Token(static_cast < tokenType > ('&'), string(1, '&')));
      }
    }
    //	|| or |
    if (ch == '|') {
      if (this -> nextChar() && ch == '|') {
        return shared_ptr < Token > (new Token(OR_OP, "||"));
      } else {
        this -> inputFile.unget();
        return shared_ptr < Token > (new Token(static_cast < tokenType > ('|'), string(1, '|')));
      }
    }
    //	<= or <
    if (ch == '<') {
      if (this -> nextChar() && ch == '=') {
        return shared_ptr < Token > (new Token(LE_OP, "<="));
      } else {
        this -> inputFile.unget();
        return shared_ptr < Token > (new Token(static_cast < tokenType > ('<'), string(1, '<')));
      }
    }
    //	>= or >
    if (ch == '>') {
      if (this -> nextChar() && ch == '=') {
        return shared_ptr < Token > (new Token(GE_OP, ">="));
      } else {
        this -> inputFile.unget();
        return shared_ptr < Token > (new Token(static_cast < tokenType > ('>'), string(1, '>')));
      }
    }
    //	== or =
    if (ch == '=') {
      if (this -> nextChar() && ch == '=') {
        return shared_ptr < Token > (new Token(EQ_OP, "=="));
      } else {
        this -> inputFile.unget();
        return shared_ptr < Token > (new Token(static_cast < tokenType > ('='), string(1, '=')));
      }
    }
    //	!= or !
    if (ch == '!') {
      if (this -> nextChar() && ch == '=') {
        return shared_ptr < Token > (new Token(NE_OP, "!="));
      } else {
        this -> inputFile.unget();
        return shared_ptr < Token > (new Token(static_cast < tokenType > ('!'), string(1, '!')));
      }
    }
    // CONSTANT
    if (isdigit(ch) || ch == '.') {
      string constant(1, ch);
      bool flag = true;
      while (flag && this -> nextChar()) {
        if (isdigit(ch) || ch == 'E' || ch == 'e' || ch == '.') {
          constant.push_back(ch);
        } else {
          this -> inputFile.unget();
          flag = false;
        }
      }
      if (std::regex_match(constant, std::regex("\\d+")) ||
        std::regex_match(constant, std::regex("\\d+[eE][+-]?\\d+")) ||
        std::regex_match(constant, std::regex("\\d*\\.\\d+([eE]?[+-]?\\d+)?")) ||
        std::regex_match(constant, std::regex("\\d+\\.\\d*([eE]?[+-]?\\d+)?"))) {
        return shared_ptr < Token > (new Token(CONSTANT, constant));
      } else {
        if (constant == ".") {
          return shared_ptr < Token > (new Token(static_cast < tokenType > ('.'), constant));
        }

        // this->inputFile.unget();
        return shared_ptr < Token > (new Token(ERROR, constant));
      }
    }
    // IDENTIFIER
    if (isalpha(this -> ch) || this -> ch == '_') {
      string identifier(1, ch);
      bool flag = true;
      while (flag && this -> nextChar()) {
        if ((isalpha(this -> ch) || isdigit(this -> ch) || this -> ch == '_'))
          identifier.push_back(this -> ch);
        else {
          this -> inputFile.unget();
          flag = false;
        }
      }

      std::regex identifierRegex("[a-zA-Z_][_[:alnum:]]*");
      if (std::regex_match(identifier, identifierRegex)) {
        shared_ptr < Token > tokenptr = this -> symTab.lookupToken(identifier);
        if (tokenptr != nullptr) {
          shared_ptr < set < int >> setptr = (tokenptr -> getLines());
          if (setptr != nullptr) { // its an varToken
            setptr -> insert(this -> lineno);
            return tokenptr;
          } // its an reserved word Token
          else {
            return tokenptr;
          }
        } else // not in map
        {
          // add varToken to map
          shared_ptr < Token > tokenptr(new varToken(identifier));
          tokenptr -> add_line(this -> lineno);
          this -> symTab.insertToken(identifier, tokenptr);
        }
        return shared_ptr < Token > (new varToken(identifier));
      }
    }
    // char CONSTANT
    if (this -> ch == '\'') {
      char c = 0;
      if (this -> nextChar() && isalpha(this -> ch)) {
        c = this -> ch;
        if (this -> nextChar() && this -> ch == '\'') {
          /* code */
          return shared_ptr < Token > (new Token(CONSTANT, string(1, c)));
        }
      }
      return shared_ptr < Token > (new Token(ERROR, string(1, c)));
    }
    // STRING_LITERAL
    if (this -> ch == '"') {
      bool flag = true;
      std::string str;
      while (flag && this -> nextChar()) {
        if (this -> ch == '"') {
          flag = false;
        } else {
          str.push_back(this -> ch);
        }
      }
      return shared_ptr < Token > (new Token(STRING_LITERAL, str));
    }
  }

  return nullptr;
}