#include "scanner.h"

// putback() puts its argument back into the Token_stream's buffer
void Token_stream::putback(Token t)
{
    if (full)
        throw runtime_error("putback into a full buffer");
    buffer = t;  // copy t to buffer
    full = true; // buffer is now full
}

Token Token_stream::get()
{
    if (full)
    {                 // do we already have a Token ready?
        full = false; // remove token from buffer
        return buffer;
    }
    char ch, prev;
    cin.get(ch); // cin.get() does NOT skip whitespace
    while (isspace(ch) || ch == '\\')
    {
        if (ch == '\n' && prev != '\\')
            return Token(EOL);
        prev = ch;
        cin.get(ch);
    }
    switch (ch)
    {
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '=':
    case '<':
    case '>':
    case '|':
    case '&':
    case '!':
    case 'q':
        return Token(ch); // let each character represent itself
    default:
        if (isdigit(ch))
        {
            string s;
            s += ch;
            while (cin.get(ch) && isdigit(ch))
                s += ch;
            cin.unget();
            //	return Token(number, stoi(s));
            return Token(NUM, stoi(s));
        }
        if (isalpha(ch) || ch == '_')
        {
            string s;
            s += ch;
            while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_'))
            {
                s += ch;
            }
            cin.unget();
            if (s == "int")
            {
                return Token(INT);
            }
            else if (s == "if")
            {
                return Token(IF);
            }
            else if (s == "else")
            {
                return Token(ELSE);
            }
            else if (regex_match(s, regex("[A-Za-z][A-Za-z0-9]*")))
            {
                return Token(ID, s);
            }
            else
                throw runtime_error("Illegal name");
        }
        throw runtime_error("Bad token");
    }
}
