%{
#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

extern int yylex();
extern FILE *yyin;
void yyerror(const char * s);

vector<string> outputList;	//to store the code
int countLable = 0;		// counter for lables
extern int lineCounter;
int varaiblesNum = 1;

string sourceFile ;
ofstream fileOut("javaByteCode.j");	// the output file


typedef enum {INTEGER_TYPE, FLOAT_TYPE, VOID_TYPE, ERROR_TYPE} types;
map<string, pair<int,types> > symbTab;	//sympol table to map each id to its type ans store it


/*map for javabyte code */
map<string,string> byte_code_inst_helper = {
	{"+", "add"},
	{"-", "sub"},
	{"/", "div"},
	{"*", "mul"},
	{"%", "rem"},
	{"==", "if_icmpeq"},
	{"<=", "if_icmple"},
	{">=", "if_icmpge"},
	{"!=", "if_icmpne"},
	{">", "if_icmpgt"},
	{"<", "if_icmplt"}
};

/* declaring some functions */
void genHeader(void);	// header of the javabytecode
void genFooter(void);	// foooter of the javabytecode

void arithCast(int from , int to, string op);
void genCode(vector<int> *list, int num);
void defineVar(string name, int type);
string getOperationName(string op);
string getLabel(int n);
string generateLabel();
bool checkId(string id);
vector<int> * merge (vector<int> *list1, vector<int>* list2);

void addLine(string x);
void printCode(void);
void printLineNumber(int num);

%}

%code requires {
	#include <vector>
	using namespace std;
}

/* declaring tokens and types */

%token WHILE IF_KEY ELSE_KEY INT_KEY FLOAT_KEY SEMI_COLON COMA BRACKET_OPEN BRACKET_CLOSE CURLY_OPEN CURLY_CLOSE SYSTEM_OUT

%token <intunion> INT
%token <floatunion> FLOAT
%token <stringsunion> RELOP
%token <stringsunion> ASSIGN
%token <stringsunion> ADDOP
%token <stringsunion> MULOP
%token <stringsunion> ID

%type <statement_type> statement
%type <statement_type> statement_list
%type <statement_type> if
%type <statement_type> while
%type <intunion> primitive_type
%type <intunion> new_lable
%type <intunion> goto
%type <bool_type> bool_expression
%type <exp_type> expression

%union{

    int intunion;
    float floatunion;
    char * stringsunion;
    struct {
		vector<int> *nextlist;
	} statement_type;
    
    struct {
		vector<int> *trueList, *falseList;
	} bool_type;
    
    struct {
		int sType;
	}exp_type;
}

%start method_body //the starting statement

/* semantic rules of the grammar */
%%
method_body:
    {genHeader();}
    statement_list
    new_lable
    {
        genCode($2.nextlist, $3);
        genFooter();
    }
    ;
statement_list:
    statement
    |
    statement
    new_lable
    statement_list
    {
        genCode($1.nextlist, $2);
        $$.nextlist = $3.nextlist;
    }
    ;
statement:
    declaration{$$.nextlist = new vector<int>();}
    |
    if{$$.nextlist = $1.nextlist;}
    |
    while{$$.nextlist = $1.nextlist;}
    |
    assignment{$$.nextlist = new vector<int>();}
    |
    system_print {$$.nextlist = new vector<int>();}
    ;
declaration:
    primitive_type ID SEMI_COLON
    {
        string str($2);
	if($1 == INTEGER_TYPE)
	{
		defineVar(str, INTEGER_TYPE);
	}
	else if ($1 == FLOAT_TYPE)
	{
		defineVar(str, FLOAT_TYPE);
	}
    }
    ;
primitive_type:
    INT_KEY {$$ = INTEGER_TYPE;}
    |
    FLOAT_KEY {$$ = FLOAT_TYPE;}
    ;
new_lable:
    {
        $$ = countLable;
        addLine(generateLabel() + ":");
    }
    ;
goto:
    {
	   $$ = outputList.size();
	   addLine("goto ");
    }
    ;
if:
    IF_KEY BRACKET_OPEN bool_expression BRACKET_CLOSE CURLY_OPEN new_lable statement_list goto CURLY_CLOSE ELSE_KEY CURLY_OPEN new_lable statement_list CURLY_CLOSE
    {
        genCode($3.trueList,$6);
        genCode($3.falseList,$12);
        $$.nextlist = merge($7.nextlist, $13.nextlist);
	$$.nextlist->push_back($8);
    }
    ;
while:
    new_lable WHILE BRACKET_OPEN bool_expression BRACKET_CLOSE CURLY_OPEN new_lable statement_list CURLY_CLOSE
    {
        addLine("goto " + getLabel($1));
	genCode($8.nextlist,$1);
	genCode($4.trueList,$7);
	$$.nextlist = $4.falseList;
    }
    ;
assignment:
    ID ASSIGN expression SEMI_COLON
    {
        string str($1);
		if(checkId(str))
		{
			if($3.sType == symbTab[str].second)
			{
				if($3.sType == INTEGER_TYPE)
				{
					addLine("istore " + to_string(symbTab[str].first));
				}
				else if ($3.sType == FLOAT_TYPE)
				{
					addLine("fstore " + to_string(symbTab[str].first));
				}
			} else {
				string err = "identifier and expression are not of the same type";
			        yyerror(err.c_str());
			}
		}else{
			string err = str+" isn't declared in this scope";
			yyerror(err.c_str());
		}
    }
    ;
expression:
    FLOAT 	{$$.sType = FLOAT_TYPE; addLine("ldc "+to_string($1));}
	| INT 	{$$.sType = INTEGER_TYPE; addLine("ldc "+to_string($1));}
	| expression ADDOP expression	{arithCast($1.sType, $3.sType, string($2));}
    | expression MULOP expression	{arithCast($1.sType, $3.sType, string($2));}
	| ID {
		string str($1);
		if(checkId(str))
		{
			$$.sType = symbTab[str].second;
			if(symbTab[str].second == INTEGER_TYPE)
			{
				addLine("iload " + to_string(symbTab[str].first));
			}else if (symbTab[str].second == FLOAT_TYPE)
			{
				addLine("fload " + to_string(symbTab[str].first));
			}
		}
		else
		{
			string err = str+" isn't declared in this scope";
			yyerror(err.c_str());
			$$.sType = ERROR_TYPE;
		}
	}
	| BRACKET_OPEN expression BRACKET_CLOSE {$$.sType = $2.sType;}
	;
bool_expression:
    expression RELOP expression
    {
        string op ($2);
		$$.trueList = new vector<int>();
		$$.trueList ->push_back (outputList.size());
		$$.falseList = new vector<int>();
		$$.falseList->push_back(outputList.size()+1);
		addLine(getOperationName(op)+ " ");
		addLine("goto ");
    }
    ;

system_print:
	SYSTEM_OUT BRACKET_OPEN expression BRACKET_CLOSE SEMI_COLON
	{
		if($3.sType == INTEGER_TYPE)
		{
			/* expression is at top of stack now
			 save it at the predefined temp syso var
			 */			
			addLine("istore " + to_string(symbTab["1syso_int_var"].first));
			/* call syso */			
			addLine("getstatic java/lang/System/out Ljava/io/PrintStream;");
			/*insert param*/
			addLine("iload " + to_string(symbTab["1syso_int_var"].first ));
			/*invoke syso*/
			addLine("invokevirtual java/io/PrintStream/println(I)V");

		}else if ($3.sType == FLOAT_TYPE)
		{
			addLine("fstore " + to_string(symbTab["1syso_float_var"].first));
			/* call syso */			
			addLine("getstatic java/lang/System/out Ljava/io/PrintStream;");
			/*insert param*/
			addLine("fload " + to_string(symbTab["1syso_float_var"].first ));
			/*invoke syso*/
			addLine("invokevirtual java/io/PrintStream/println(F)V");
		}
	}
	;
%%



main (int argv, char * argc[])
{
	FILE *myfile;
	if(argv == 1)
	{
		myfile = fopen("program.txt", "r");
		sourceFile = "program.txt";
	}
	else
	{
		myfile = fopen(argc[1], "r");
		sourceFile = string(argc[1]);
	}
	if (!myfile) {
		printf("Code file does not exist!\n");
		char path[200];
		if (!getcwd(path, sizeof(path)))
		 {
		 return -1;
		 }
		printf("%s\n",path);
		getchar();
		return -1;
	}
	yyin = myfile;
	yyparse();
	printCode();
}

void yyerror(const char * s)
{
	printf("error@%d: %s\n",lineCounter, s);
}

void genHeader()
{
	addLine(".source " + sourceFile);
	addLine(".class public result\n.super java/lang/Object\n");
	addLine(".method public <init>()V");
	addLine("aload_0");
	addLine("invokenonvirtual java/lang/Object/<init>()V");
	addLine("return");
	addLine(".end method\n");
	addLine(".method public static main([Ljava/lang/String;)V");
	addLine(".limit locals 100\n.limit stack 100");

	/* generate temporal vars for syso*/
	defineVar("1syso_int_var",INTEGER_TYPE);
	defineVar("1syso_float_var",FLOAT_TYPE);

	/*generate line*/
	addLine(".line 1");
}

void genFooter()
{
	addLine("return");
	addLine(".end method");
}
void arithCast(int from, int to, string op)
{
	if(from == to)
	{
		if(from == INTEGER_TYPE)
		{
			addLine("i" + getOperationName(op));
		}
		else if (from == FLOAT_TYPE)
		{
			addLine("f" + getOperationName(op));
		}
	}
	else
	{
		string err = "Casting failed";
	        yyerror(err.c_str());
	}
}
string getOperationName(string op)
{
	if(byte_code_inst_helper.find(op) != byte_code_inst_helper.end())
	{
		return byte_code_inst_helper[op];
	}
	return "";
}
bool checkId(string op)
{
	return (symbTab.find(op) != symbTab.end());
}
void defineVar(string name, int type)
{
	if(checkId(name))
	{
		string err = name+" was declared before";
		yyerror(err.c_str());
	}
	else
	{
		if(type == INTEGER_TYPE)
		{
			addLine("iconst_0\nistore " + to_string(varaiblesNum));
		}
		else if ( type == FLOAT_TYPE)
		{
			addLine("fconst_0\nfstore " + to_string(varaiblesNum));
		}
		symbTab[name] = make_pair(varaiblesNum++,(types)type);
	}
}
string generateLabel()
{
	return "L_"+to_string(countLable++);
}

string getLabel(int n)
{
	return "L_"+to_string(n);
}
vector<int> * merge(vector<int> *list1, vector<int> *list2)
{
	if(list1 && list2){
		vector<int> *list = new vector<int> (*list1);
		list->insert(list->end(), list2->begin(),list2->end());
		return list;
	}else if(list1)
	{
		return list1;
	}else if (list2)
	{
		return list2;
	}else
	{
		return new vector<int>();
	}
}
void genCode(vector<int> *lists, int ind)
{
	if(! lists) return;
	for(int i = 0 ; i < lists->size() ; i++)
	{
		outputList[(*lists)[i]] = outputList[(*lists)[i]] + getLabel(ind);
	}
}
void addLine(string x)
{
	outputList.push_back(x);
}

void printCode(void)
{
	for ( int i = 0 ; i < outputList.size() ; i++)
	{
		fileOut<<outputList[i]<<endl;
	}
}
void printLineNumber(int num)
{
	addLine(".line "+ to_string(num));
}
