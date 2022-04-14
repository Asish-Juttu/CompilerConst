/****   GROUP NUMBER: 43
Name: Rahul B
ID: 2019A7PS0134P

Name: Asish Juttu
ID: 2019A7PS0039P

Name: Narasimha Gupta Jangala
ID: 2019A7PS0138P

Name: Praneeth Chaitanya Jonnavithula
ID: 2019A7PS1111P

Name: Damargidda Rohan Raj Goud
ID: 2019A7PS0065P
****/

#ifndef _PARSER_DEF_
#define _PARSER_DEF_

#define NON_TERMINAL_SIZE 53
#define TERMINAL_SIZE 59

#include "token.h"
#include "ast_def.h"

typedef enum {
    PROGRAM, MAIN_FUNCTION, OTHER_FUNCTIONS, FUNCTION, INPUT_PAR, OUTPUT_PAR, 
    PARAMETER_LIST, DATATYPE, PRIMITIVE_DATATYPE, CONSTRUCTED_DATATYPE, 
    REMAINING_LIST, STMTS, TYPE_DEFINITIONS, TYPE_DEFINITION, FIELD_DEFINITIONS, 
    FIELD_DEFINITION, MORE_FIELDS, DECLARATIONS, DECLARATION, GLOBAL_OR_NOT, 
    OTHER_STMTS, STMT, ASSIGNMENT_STMT, SINGLE_OR_REC_ID, FUN_CALL_STMT, OUTPUT_PARAMETERS, 
    INPUT_PARAMETERS, ITERATIVE_STMT, CONDITIONAL_STMT,  IO_STMT,
    ARITHMETIC_EXPRESSION, BOOLEAN_EXPRESSION, VAR, LOGICAL_OP, RELATIONAL_OP, 
    RETURN_STMT, OPTIONAL_RETURN, ID_LIST, MORE_IDS, ACTUAL_OR_REDEFINED,
    DEFINE_TYPE_STATEMENT, FIELD_TYPE, OPTION_SINGLE_CONSTRUCTED, ONE_EXPANSION, MORE_EXPANSIONS,
    A, ELSE_PART, TERM, EXP_PRIME, LOW_PRECEDENCE_OPERATORS, FACTOR, TERM_PRIME, HIGH_PRECEDENCE_OPERATORS
} NonTerminal;

typedef struct{
    unsigned long long bitMask;
} NonTermSet;

typedef struct{
    unsigned int isTerminal : 1;
    int symbol;
} Symbol;

typedef struct{
    int size;
    int ruleNum;
    NonTerminal nt;
    Symbol* symbol;
} Rule;

typedef struct{
    int size;
    Rule* rule;
} RuleArray;

typedef struct {
    int size;
    RuleArray* ruleArray;
    NonTermSet nullableSet;
    int nullableRuleNum[NON_TERMINAL_SIZE];
} Grammar;

typedef struct  {
     Rule** table;
} ParseTable;

typedef struct{
    Token t;
    int ruleNo;
} FirstFollowElement;

typedef struct{
    FirstFollowElement* elements;
    int size;
} FirstFollowArray;

typedef struct {
    int size;
    FirstFollowArray* first;
    FirstFollowArray* follow;
} FirstAndFollow;

typedef struct _ParseTreeElem_{
    Symbol elem;
    int ruleNo;

    int lineNo;
    TokenInfo tinfo;
    AstNode* node_syn;
    AstNode* node_inh;

    int noOfChildren;
    struct _ParseTreeElem_* children;
    //Basically from left to right I am storing. 
} ParseTreeElement;

typedef struct _Stack_{
     ParseTreeElement* current;
     struct _Stack_* next;
} Stack;

typedef struct {
    ParseTreeElement* head;
} ParseTree;

typedef struct {
    NonTermSet ntSet;
    TokenSet tSet;
} FollowHelperSet;

typedef struct {
    NonTerminal nt;
    int ruleNo;
    int index;
} Location;

typedef struct {
    int size;
    Location* location;
} LocationArray;

NonTermSet nullNonTermSet();
NonTermSet singletonNonTermSet(NonTerminal nt);
NonTermSet nonTermSetUnion(NonTermSet s1, NonTermSet s2);
int nonTermSetContains(NonTermSet ntSet, NonTerminal nt);
FollowHelperSet followHelper(Grammar* grammar, NonTerminal nt);
void printSymbols(Symbol* syms, int size);

int equalsNonTermSet(NonTermSet s1, NonTermSet s2);
void printTSet(TokenSet tSet);
void printNTSet(NonTermSet ntSet);

int isNullable(Grammar* grammar, NonTerminal nt);
char* nonTermToStr(NonTerminal nt);

LocationArray* getLocationArray();
#endif