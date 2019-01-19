#include "JackTokenizer.h"
#include <iostream>
#include <sys/stat.h>
#include <string>
#include <set>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>     /* atoi */
#include <sstream>

using namespace std;

    template <typename T>
    string to_string(T value)
    {
      //create an output string stream
      std::ostringstream os ;

      //throw the value into the string stream
      os << value ;

      //convert the string stream into a string and return
      return os.str() ;
    }

	JackTokenizer::JackTokenizer (string fname){
		arq.unsetf(ios_base::skipws);
		arq.open(fname.c_str());
		currentChar=0;


		string a[] = {string("int"),string("class"),string("constructor"),string("function"),string("method"),string("field"),string("static"),string("var"),string("char"),string("boolean"),string("void"),string("true"),string("false"),string("null"),string("this"),string("let"),string("do"),string("if"),string("else"),string("while"),string("return")};
  		std::set<string>  s1(a,a+21);

  		keywords = s1;

	}

	void JackTokenizer::advance(){
		currentToken = nextToken();
	//	cout << "token" << int(*currentToken.rbegin()) << "fim" << (currentcurrentToken == "") << endl; 
		while (int(*currentToken.rbegin())  == 0 and hasMoreTokens())  // gambiarra
			currentToken = nextToken();
		
	}
	bool JackTokenizer::hasMoreTokens(){
		return !arq.eof();
	}

	bool JackTokenizer::isKeyword(string t) {
		bool x = keywords.find(t) != keywords.end();
		//cout << (t == "let") << "--" << t << "--" << endl;
		//cout << x << t << endl;
		return x;
	}

	bool JackTokenizer::isStringConst(string t) {
		//cout << *currentToken.rbegin() << currentToken.rend() == '\"') << endl;
		//cout << t[t.length()-1]  << endl;
		return (t[0] == '\"') && (t[t.length()-1] == '\"');
	}

	bool JackTokenizer::isIntConst(string t) {
		return isdigit(*t.rbegin());
	}

	bool JackTokenizer::isIdentifier (string t) {

	if (isdigit(*t.rbegin()))
        return false;



    string::const_iterator it = t.begin();


    while (it != t.end() && (isalpha(*it) || isdigit(*it) || *it == '_'))
        ++it;

	return it == t.end();

	}

	TokenType JackTokenizer::tokenType(){

	if (isKeyword(currentToken))
        return tKEYWORD;
    else if (isSymbol(currentToken))
        return tSYMBOL;
    else if (isStringConst(currentToken))
        return tSTRING_CONST;
    else if (isIntConst(currentToken))
        return tINT_CONST;
    else if (isIdentifier(currentToken))
     return tIDENTIFIER;
 	else
		return tUNKNOWN;
		
	}

	bool JackTokenizer::isSymbol(char t) {
		string symbols = "{}()[].,;+-*/&|<>=~";
		return symbols.find(t) != -1;
	}

	bool JackTokenizer::isSymbol(string t) {
		
		return isSymbol(*currentToken.rbegin());
	}

	Keyword JackTokenizer::keyword(){

	if (currentToken == "class")
        return kCLASS;
    if (currentToken == "method")
        return kMETHOD;
    if (currentToken == "function")
        return kFUNCTION;
    if (currentToken == "constructor")
        return kCONSTRUCTOR;
    if (currentToken == "int")
        return kINT;
    if (currentToken == "boolean")
        return kBOOLEAN;
    if (currentToken == "char")
        return kCHAR;
    if (currentToken == "void")
        return kVOID;
    if (currentToken == "var")
        return kVAR;
    if (currentToken == "static")
        return kSTATIC;
    if (currentToken == "field")
        return kFIELD;
    if (currentToken == "let")
        return kLET;
    if (currentToken == "do")
        return kDO;
    if (currentToken == "if")
        return kIF;
    if (currentToken == "else")
        return kELSE;
    if (currentToken == "while")
        return kWHILE;
    if (currentToken == "return")
        return kRETURN;
    if (currentToken == "true")
        return kTRUE;
    if (currentToken == "false")
        return kFALSE;
    if (currentToken == "null")
        return kNULL;

return kTHIS;
		
	}

	char JackTokenizer::symbol(){
		return currentToken[0];
	}

	string JackTokenizer::identifier(){
		return currentToken;
	}

	int JackTokenizer::intVal(){
		return atoi(currentToken.c_str());
	}

	string JackTokenizer::stringVal(){
		string s = currentToken;
		s.erase(s.length()-1,1);
		s.erase (0,1);
		return s;
		
	}

	char JackTokenizer::nextChar () {
	//char token;


	if (! arq.eof()) {
		arq >> currentChar; 
	}
	else  
		currentChar = 0;
	return currentChar;
		
}

string JackTokenizer::nextToken () {

   //char ch;
    string s = "";
	//cout << "char:" << currentChar << endl;



	
   while (hasMoreTokens ()) {
   	s = "";
	
 	//cout << currentChar << "---" << endl;

	switch (currentChar) {

		case '/':
			s = s + currentChar;
			nextChar();
			//cout << "quase" << endl;
			if (currentChar == '/') {
			//	cout << "ignore" << endl;
				ignoreComments("//");
			}else if (currentChar == '*') {
				ignoreComments("/*");
			} else {
				return s;
			}
			break;

		// jogar para um if inicial
		case ' ':
			nextChar();
			//return nextToken();
			break;

		case '\t':
			nextChar();
			break;

		case '\n':
			nextChar();
			break;

		case '\r':
			nextChar();
			break;

		default:
			s = concatToken(currentChar);
			return s;
			break;
    	} 
    }

	//return nextToken();
}



string JackTokenizer::concatToken (char ch) {
	string s = "";
//	cout << "--" << endl;

	s = s + ch;
	
	if (isSymbol(ch)){
		//cout << "achei simbolo" << ch << endl;
		nextChar();
		return s;
	}
	//while (!arq.eof() && (ch=nextChar()) != ' ' && ch != '\n'  && ch != '\r' && !isSymbol(ch)) 

	if (ch == '"') {
		while (!arq.eof() && (ch=nextChar()) && ch != '\"' && ch != '\n'  && ch != '\r' && !isSymbol(ch)) 
			s = s + ch;

		s = s + '\"';
		nextChar();
	} else {
		while (!arq.eof() && (ch=nextChar()) != ' ' && ch != '\n'  && ch != '\r' && !isSymbol(ch)) 
			s = s + ch;
	}
	
	//cout << "char 2:" << currentChar << endl;
	//cout << s << endl;
	return s;
}


void JackTokenizer::ignoreComments(const char *t) {
	//int curline=line_;
	int endComment=0;
	char look = 0;
	if (strcmp(t, "//" ) == 0)
	        while (look != '\n')
		    	look =nextChar();
	else
	{
		while (!endComment)
		{
			//if (arq.eof())
				//error(0,EXPEC,"*/");

			//if (look == '\n')
			//	line_++;				
	
			look = nextChar();
			if (look == '*')
			{
			    while (look == '*')
					look = nextChar();
			    if (look == '/')
			    {
				    endComment=1;
				    look = nextChar();
			    }		
			}
		}
	}
}


	string JackTokenizer::tagToken()
{
    switch (tokenType()) {

        case tKEYWORD:
            return "<keyword> " + currentToken + " </keyword>";
            break;

        case tSYMBOL:
            if (symbol() == '<')
                return "<symbol> &lt; </symbol>";
            else if (symbol() == '>')
                return "<symbol> &gt; </symbol>";
            else if (symbol() == '\"')
                return "<symbol> &quot; </symbol>";
            else if (symbol() == '&')
                return "<symbol> &amp; </symbol>";
            else {
                string sym(1, symbol());
                return "<symbol> " + sym + " </symbol>";
            }
            break;

        case tIDENTIFIER:
            return "<identifier> " + identifier() + " </identifier>";
            break;

        case tINT_CONST:
            return "<integerConstant> " + to_string(intVal()) +" </integerConstant>";
            break;

        case tSTRING_CONST:
            return "<stringConstant> " + stringVal() + " </stringConstant>";
            break;

        default:
            return "<ERROR> " + currentToken + " </ERROR>";
            break;
}
}
	
