/*
Arthur: Raphael Gozlan
Date : 14/04/21 
*/
#include "token.h"

using namespace std;

// Token Class 
tokenType Token::getType(){
    return this->type;
}
string Token::getText(){
    return this->text;
}

void Token::printToken(){
    cout << this->type <<'\t'<<this->text<<endl;
}

//vatToken class
void varToken::add_line(int l){
    this->lines->insert(l);
}

shared_ptr<set<int>> varToken::getLines(){
    return this->lines;
}