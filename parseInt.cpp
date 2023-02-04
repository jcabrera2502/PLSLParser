/* Implementation of Recursive-Descent Parser
 * parse.cpp
 * Programming Assignment 2
 * Spring 2022
*/

#include "parseInt.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;
map<string, bool> defVar; // stores declared variables
map<string, Token> SymTable; // variable with it's identifier
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //points at values
//unsuccessful interpretation needs separate error message
namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

//bool IdentList(istream& in, int& line, vector<string> &IdList);

//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
	bool f1, f2;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			
			tok = Parser::GetNextToken(in, line);
			if (tok.GetToken() == SEMICOL) {
				f1 = DeclBlock(in, line); 
			
				if(f1) {
					f2 = ProgBody(in, line);
					if(!f2)
					{
						ParseError(line, "Incorrect Program Body.");
						return false;
					}
					
					return true;//Successful Parsing is completed
				}
				else
				{
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
			}
			else
			{
				//Parser::PushBackToken(tok);
				ParseError(line-1, "Missing Semicolon.");
				return false;
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	ParseError(line, "Missing PROGRAM.");
	return false;
}

bool ProgBody(istream& in, int& line){
	bool status;
		
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == BEGIN) {
		
		status = Stmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Statement.");
				return false;
			}
			
			status = Stmt(in, line);
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok == END )
		{
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Program Body.");
		return false;
	}	
}//End of ProgBody function


bool DeclBlock(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	//cout << "in Decl" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	if(t == VAR)
	{
		status = DeclStmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Declaration Statement.");
				return false;
			}
			status = DeclStmt(in, line);
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok == BEGIN )
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Declaration Block.");
		return false;
	}
	
}//end of DeclBlock function

bool DeclStmt(istream& in, int& line)
{
	LexItem t;
	bool status = IdentList(in, line);
	
	if (!status)
	{
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return status;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t == COLON)
	{
		t = Parser::GetNextToken(in, line);
		if(t == INTEGER || t == REAL || t == STRING)
		{
			for(auto i : defVar)
			{
				SymTable.insert(pair<string, Token>(i.first, t.GetToken()));
			}
			return true;
		}
		else
		{
			ParseError(line, "Incorrect Declaration Type.");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(t);
		return false;
	}
	
}//End of DeclStmt

//IdList:= IDENT {,IDENT} also checks if variable was defined already
bool IdentList(istream& in, int& line) {
	bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = IdentList(in, line);
	}
	else if(tok == COLON)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;
	
}//End of IdentList
	

//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt = AssigStmt | IfStmt | WriteStmt | ForStmt 
bool Stmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case WRITELN:
		status = WriteLnStmt(in, line);
		
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		
		break;		
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of Stmt


//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
	ValQue = new queue<Value>;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	
	//Evaluate: print out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;

	return ex;
}

//IfStmt:= if (Expr) then Stm} [Else Stmt]
bool IfStmt(istream& in, int& line) {
	bool ex=false, status ; 
	LexItem t;
	Value eval;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	ex = LogicExpr(in, line, eval);

	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != THEN)
	{
		ParseError(line, "If-Stmt Syntax Error");
		return false;
	}

	// skip to ElSE statement if condition was false

	if (eval.GetBool() == false)
	{
		while (t != ELSE)
		{
			t = Parser::GetNextToken(in, line);
		}
	}
	status = Stmt(in, line);
	if(!status)
	{
		ParseError(line, "Missing Statement for If-Stmt Then-Part");
		return false;
	}
	t = Parser::GetNextToken(in, line);

	// skip to end if if-statement was correct

	if (eval.GetBool() == true)
	{
		while (t != SEMICOL)
		{
			t = Parser::GetNextToken(in, line);
		}
	}

	if( t == ELSE ) {
		status = Stmt(in, line);
		if(!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Else-Part");
			return false;
		}
		//cout << "in IFStmt status of Stmt true" << endl;
		return true;
	}
		
	Parser::PushBackToken(t);
	return true;
}//End of IfStmt function

//Var:= ident
bool Var(istream& in, int& line, LexItem & idtok)
{
	//called only from the AssignStmt function
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT){
		identstr = tok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}	
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	
	bool varstatus = false, status = false;
	LexItem t = Parser::GetNextToken(in, line);
	Parser::PushBackToken(t);
	Value eval = Value();
	string lval = t.GetLexeme();

	// chosing the type of the values
	if (SymTable.find(lval) -> second == INTEGER)
	{
		eval.SetType(VINT);
	}
	if (SymTable.find(lval) -> second == REAL)
	{
		eval.SetType(VREAL);
	}
	if (SymTable.find(lval) -> second == STRING)
	{
		eval.SetType(VSTRING);
	}
	//add bool if needed

	TempsResults.insert(pair<string,Value>(lval, eval));

	varstatus = Var(in, line, t);

	if (varstatus){
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP){
			status = Expr(in, line, eval);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	if (TempsResults[lval].GetType() != eval.GetType())
	{
		if (TempsResults[lval].GetType() == VSTRING || eval.GetType() == VSTRING)
		{
			ParseError(line, "Illegal Assignment Operation");
			return false;
		}
		// checks for type conversions
		if (SymTable.find(lval) -> second == REAL && eval.GetType() == VINT)
		{
			eval.SetReal(eval.GetInt());
			eval.SetInt(0);
			eval.SetType(VREAL);
		}
		else if (SymTable.find(lval) -> second == INTEGER && eval.GetType() == VREAL)
		{
			float val = eval.GetReal();
			int newval = static_cast<int>(val);
			eval.SetInt(newval);
			eval.SetReal(0);
			eval.SetType(VINT);
		}
	}
	TempsResults[lval] = eval;
	return status;	
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value eval;

	status = Expr(in, line, eval);

	// Insert value into the list

	ValQue->push(eval);

	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value & retVal) {
	Value val1, val2;

	//check, may cause problems with simple numbers

	val1 = retVal;

	//cout << "in Expr" << endl;
	bool t1 = Term(in, line, val1);
	LexItem tok;

	if( !t1 ) {
		return false;
	}
	retVal = val1;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	//Evaluate: evaluate the expression for addition or subtraction
	while ( tok == PLUS || tok == MINUS ) 
	{
		t1 = Term(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
		if(tok == PLUS)
		{
			retVal = retVal + val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal addition operation.");
				//cout << "(" << tok.GetLexeme() << ")" << endl;		
				return false;
			}
		}
		else if(tok == MINUS)
		{
			retVal = retVal - val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal subtraction operation.");
				return false;
			}
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}				
	}
	Parser::PushBackToken(tok);
	return true;
}

//Term:= SFactor {(*|/) SFactor}
bool Term(istream& in, int& line, Value & retVal) {
	Value m1;
	bool t1 = SFactor(in, line, retVal);
	LexItem tok;
	m1 = retVal;
	if( !t1 ) {
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	// TODO Handle Multiplication
	while ( tok == MULT || tok == DIV  )
	{
		t1 = SFactor(in, line, retVal);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

		// Perform multiplication
		if (tok == MULT)
		{
			retVal = m1 * retVal;
		}
		// Perform division
		if (tok == DIV)
		{
			if (retVal.GetType() == VINT && retVal.GetInt() == 0)
			{
				ParseError(line, "Run-Time Error-Illegal Division by Zero");
				return false;
			}
			if (retVal.GetType() == VREAL && retVal.GetReal() == 0)
			{
				ParseError(line, "Run-Time Error-Illegal Division by Zero");
				return false;
			}
			retVal = m1 / retVal;
		}
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

//SFactor = Sign Factor | Factor
bool SFactor(istream& in, int& line, Value & retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		if (retVal.GetType() == VSTRING)
		{
			ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
		}
		sign = -1;
	}
	else if(t == PLUS)
	{
		if (retVal.GetType() == VSTRING)
		{
			ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
		}
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
		
	status = Factor(in, line, sign, retVal);
	return status;
}

//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream& in, int& line, Value & retVal)
{
	bool t1 = Expr(in, line, retVal);
	LexItem tok;
	Value lside = retVal;
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == GTHAN  || tok == EQUAL  || tok == LTHAN)
	{
		t1 = Expr(in, line, retVal);
		Value rside = retVal;

		//checks for incompatible types

		if ((lside.GetType() == VSTRING && rside.GetType() != VSTRING) ||
			(rside.GetType() == VSTRING && lside.GetType() != VSTRING))
		{
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands for a Logic Expression");
			return false;
		}
		//Compares sides
			if (tok == GTHAN)
			{
				retVal = lside > rside;
			}
			if (tok == LTHAN)
			{
				retVal = lside < rside;
			}
			if (tok == EQUAL)
			{
				retVal = lside == rside;
			}
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
		return true;
	}
	Parser::PushBackToken(tok);
	return true;
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, int sign, Value & retVal) {
	LexItem tok = Parser::GetNextToken(in, line);
	bool assiop = false;
	//choose variable type and set value

	if (retVal.GetType() == VINT && tok != LPAREN && tok != IDENT)
	{
		//convert lexeme to integer
		int val;
		istringstream str(tok.GetLexeme());
		str >> val;

		//check for negative
		if (sign == -1)
		{
			val = val * -1;
		}

		retVal = Value(val);
		assiop = true;
	}
	if (retVal.GetType() == VREAL && tok != LPAREN && tok != IDENT)
	{
		//convert lexeme to decimal
		float val;
		istringstream str(tok.GetLexeme());
		str >> val;

		//check for negative
		if (sign == -1)
		{
			val = val * -1;
		}

		retVal = Value(val);
		assiop = true;
	}

	if( tok == IDENT ) {
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undefined Variable");
			return false;	
		}
		//Checks value's existence
		if ((TempsResults.find(lexeme)->second).GetType() == VERR || 
			TempsResults.find(lexeme) == TempsResults.end())
		{
			ParseError(line, "Undefined Variable");
			return false;	
		}
		retVal = TempsResults.find(lexeme)->second;
		//check for negative
		if (sign == -1)
		{
			retVal = retVal * -1;
		}

		return true;
	}
	else if( tok == ICONST ) {
		// if the number is not a result of an assignment operation
		if (!assiop)
		{
			//convert lexeme to integer
			int val;
			istringstream str(tok.GetLexeme());
			str >> val;

			//check for negative
			if (sign == -1)
			{
				val = val * -1;
			}

			retVal = Value(val);
			assiop = false;
		}
		return true;
	}
	else if( tok == SCONST ) {
		retVal = Value(tok.GetLexeme());
		return true;
	}
	else if( tok == RCONST ) {
		// if the number is not a result of an assignment operation
		if (!assiop)
		{
			//convert lexeme to decimal
			float val;
			istringstream str(tok.GetLexeme());
			str >> val;

			//check for negative
			if (sign == -1)
			{
				val = val * -1;
			}

			retVal = Value(val);
			assiop = false;
		}
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after (");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing ) after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}



