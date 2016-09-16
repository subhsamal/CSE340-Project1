/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 *
 *Project implemented by Subhadarshi Samal, CSE 340
 *Summer '16, ASU ID - 1210421105
 */
 
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

int isDot = 0;

Token prev_num;
string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM", "BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "   
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";    //  -- tmp is an object of Token class inside LexicalAnalyzer class.
    tmp.line_no = 1;
    tmp.token_type = ERROR;  // -- tmp access the token_type variable inside Token class of enum type TokenType.
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;
    input.GetChar(c);   //-- GetChar() of InputBuffer Class
    line_no += (c == '\n');  //  unless \n is encountered line_no remains unchanged.

    while (!input.EndOfInput() && isspace(c)) {  
        space_encountered = true;
        input.GetChar(c);   // -- calls GetChar() of inputBuffer class. , input is an object of InputBuffer class inside LexicalAnalyzer class.
        line_no += (c == '\n'); 
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s) 
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);  
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;

    input.GetChar(c);
    if (isdigit(c)) {    
          if (c == '0' && !isDot) {
            tmp.lexeme = "0";
         
        }
            
        else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) { 
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
        tmp.token_type = NUM;
        tmp.line_no = line_no;  
     
        return tmp;
    } 
    else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;        
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);
    
        if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);  
            int i = 0; 
            Token tmp2;          
            if(tmp.lexeme[i] == 'x' && tmp.lexeme[i+1] == '0' && tmp.lexeme[i+2] == '8'){
            input.UngetChar(c);
            tmp2.lexeme = "x08";
            tmp2.token_type=ID;
            tmp2.line_no = line_no;          
            return tmp2; 
            }             
            else {
            }            
        }            
         
           
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }    
    
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;  
}

Token LexicalAnalyzer::GetToken()
{
    char c;
    Token num;
    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {  //sumu-- tokens vector inside class LexicalAnalyzer of type Token
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace(); //-- calls SkipSpace() of LexicalAnalyzer Class.
    tmp.lexeme = "";
    tmp.line_no = line_no;  
    input.GetChar(c); //-- calls GetChar() of InputBuffer class inside inputbuf.cc
    switch (c) {
        case '.':
            tmp.token_type = DOT;            
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;            
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if(c == ' '){
            return tmp;
            }
            if (isdigit(c)) {              
                input.UngetChar(c);
                num = ScanNumber();
                prev_num = num;               
                Token ch;
                Token ch1;
                Token ch2;
                Token b08;
                Token b16;         
                ch = GetToken();        
                string temp="";
                string temp1 = "";
                //string temp2 = "";
              if(ch.token_type == ID){
                 
                      if(ch.lexeme == "x08" && !isDot){                                          
                        
                         if(num.lexeme == "0"){                       
                         b08.lexeme += num.lexeme;
                         b08.lexeme += ch.lexeme.substr(0,3);
                         b08.token_type = BASE08NUM;
                         b08.line_no = line_no;
                         return b08;
                       } 
                     else if (num.lexeme.length() == 0){
                     return ch;
                     }  
                  
                   else{           
                     for (int i =num.lexeme.length()-1 ; i >= 0; i--){  
                         
                        if(num.lexeme[i]-'7' <= 0 && num.lexeme[i]-'0' >= 0){                                      
                        temp += num.lexeme[i];                       
                                         
                        }
                        else{
                          if(temp.length() == 0){
                          num.token_type = NUM;
                          num.line_no = line_no;                                             
                         
                         for (int i = ch.lexeme.length()-1; i >=0;i--){
                         input.UngetChar(i);}                        
                          return num; }
              
                         ch1.lexeme = num.lexeme.substr(0,i+1);
                         ch1.token_type = NUM;
                         ch1.line_no = line_no;                       
                         for (i = temp.length()-1; i >=0; i--){
                         c= temp[i];
                         input.UngetChar(c);                
                         }
                         return ch1;                       
                         }
                       }
                      }
                      std::reverse(temp.begin(),temp.end());
                      ch2.lexeme = temp;                    
                      b08.lexeme += ch2.lexeme;
                      b08.lexeme += ch.lexeme;
                      b08.token_type = BASE08NUM;
                      b08.line_no = line_no;               
                      return b08;
                       
                  }  
                  
                  else if (ch.lexeme == "x16"){
                       if (num.lexeme.length() != 0){                      
                                       
                       b16.lexeme = num.lexeme;
                       b16.lexeme += ch.lexeme;
                       b16.token_type = BASE16NUM;
                       b16.line_no = line_no;;
                       return b16;
                       }
                       
                        else {                        
                        return ch;
                        }
                     
                   }
                  else {
                  
                        UngetToken(ch);  
                       //                     
                        return num;   
                       //return ch;                 
                     }              
                                                             
               }
               else if(ch.token_type == DOT){
                    UngetToken(ch);                 
                    isDot = 1; 
               
                    return ScanRealNumber(num.lexeme);                      
                }
                else if (ch.token_type == BASE16NUM){  
                         
                if(num.lexeme[0] != '0' && num.lexeme.length() != 0){            
                b16.lexeme = num.lexeme;
                b16.lexeme += ch.lexeme;
                b16.token_type = BASE16NUM;
                b16.line_no = line_no;               
                return b16;
                }
                else if (num.lexeme == ""){
      
                ch.token_type = ID;
                ch.line_no = line_no;
                return ch;
                }
                else{
                
                UngetToken(ch);
                return num;
                }
               }
               
                else{
                   UngetToken(ch);                
                   return num;
                 }
                
                    
            } else if (isalpha(c)) {   
                string temp;
                Token Tok2;
                Token Tok3;   
                char c1;
                char c2;         
           
                if(c == 'A' || c == 'B'|| c== 'C'|| c== 'D'|| c== 'E'||c =='F'){                 
                            
                input.UngetChar(c) ;
                Tok3 = Base16Num();
           
                if (prev_num.lexeme == ""){
                    Tok3.token_type = ID;
                    return Tok3;
                    }else{
                    return Tok3;
                    }
                }
                
               else {           
                input.UngetChar(c);
            
                return ScanIdOrKeyword();
                }
               
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}
Token LexicalAnalyzer::ScanRealNumber(string num){
    char c;
    Token tok1 = GetToken();
   	
    if (tok1.token_type == DOT) { 
            
           Token tok2 = GetToken();
       
            isDot = 0;
         
          if (tok2.token_type == NUM) { 

            tmp.lexeme = num;
            tmp.lexeme += ".";
            tmp.lexeme+= tok2.lexeme;
            input.GetChar(c);
   
            while (!input.EndOfInput() && isdigit(c)) {  
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }	
           tmp.token_type = REALNUM;
           tmp.line_no = tok2.line_no; 
       
            return tmp;
          } 
         else{
             UngetToken(tok2);
             UngetToken(tok1);
             tmp.lexeme = num;
             tmp.token_type = NUM;
             tmp.line_no = line_no;
             return tmp;
           }
       } 
   }
    
    Token LexicalAnalyzer::Base16Num(){
    char c,x,d,e;
    string temp = "" ;
    Token Tok2;
    Token Tok3;
    input.GetChar(c);
    //temp = c;
     while (!input.EndOfInput()) {  
                if(!isalnum(c)){
                input.UngetChar(c);
                break;
                }                
                temp += c;
                input.GetChar(c);
                
                if (c == 'x'){
                temp += c;
                  input.GetChar(d);
                  if(d == '1'){
                  temp +=d;
                  input.GetChar(e);
                   if( e == '6'){
                   temp += e;                                   
                   Tok2.lexeme = temp;
                   Tok2.token_type = BASE16NUM;
                   Tok2.line_no = line_no;    
                             
                   return Tok2;
                   } 
                   else {
                    input.UngetChar(e);
                    input.UngetChar(d);
                
                   }
                  } 
                  else {
                  input.UngetChar(d);
              
                  }
                }
                else {
                //input.UngetChar(c);
                continue;
                }
            }
         input.UngetChar(c);
         
    
         Tok3.lexeme = temp;
         Tok3.token_type = ID;
         Tok3.line_no = line_no;         
         return Tok3;
    
    }

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
