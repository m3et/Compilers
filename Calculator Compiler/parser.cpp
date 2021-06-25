#include "scanner.h"
#include "symtab.h"
// globals
Token_stream ts;
Symbol_table st;
//map<string, int> names;

int statement()
{
    Token t = ts.get(); // get the next token from token stream
    switch (t.kind)
    {
    case INT:
        return declaration();
        break;
    case IF:
        if (primary())
        {
            int exp = expression();
            while(t.kind != EOL)t = ts.get();
            ts.putback(t);
            return exp;
        }
        else
        {
            while (t.kind != ELSE)
            {
                t = ts.get();
            }
            return expression();
        }
    default:
        ts.putback(t); // put t back into the token stream
        return expression();
    }
}

int declaration()
{
    Token t = ts.get();
    if (t.kind != ID)
        throw runtime_error("name expected in declaration");
    string name = t.name;
    st.declare(name, 0);
    return 0;
}

// | and &
bool logicExpression()
{
    return true;
}

// + and -
int expression()
{
    int left = term();  // read and evaluate a Term
    Token t = ts.get(); // get the next token from token stream

    while (true)
    {
        switch (t.kind)
        {
        case '+':
            left += term(); // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term(); // evaluate Term and subtract
            t = ts.get();
            break;
        case '<':
            left = left < term();
            t = ts.get();
            break;
        case '>':
            left = left > term();
            t = ts.get();
            break;
        case '&':
            return left && expression();
        case '|':
            return left || expression();
        case '!':
            expression();
        case ELSE:
            return left;
        default:
            ts.putback(t); // put t back into the token stream
            return left;   // finally: no more + or -: return the answer
        }
    }
}

// * and /
int term()
{
    int left = primary();
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
        {
            int d = primary();
            if (d == 0)
                throw runtime_error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        case '>':
            return left > primary();
        case '<':
            return left < primary();
        default:
            ts.putback(t);
            return left;
        }
    }
}

int primary()
{
    Token t = ts.get();
    int result;
    switch (t.kind)
    {
    case '(': // handle '(' expression ')'
    {
        int d = expression();
        t = ts.get();
        if (t.kind != ')')
            throw runtime_error("')' expected");
        return d;
    }
    case '!':
        return !expression();
    case '-':
        return -primary();
    case '+':
        return primary();
    case '<':
        return primary();
    case '>':
        return primary();
    case '|':
        return term();
    case '&':
        return term();
    case NUM:
        return t.value; // return the number value
    case ID:
    {
        string n = t.name;
        Token next = ts.get();
        if (next.kind == '=')
        { // name = expression
            int d = expression();
            st.set(n, d);
            return d; // return the assignment value
        }
        else
        {
            ts.putback(next);            // not an assignment
            return st.get(t.name).value; // return the id value
        }
    }
    default:
        throw runtime_error("primary expected");
    }
}
