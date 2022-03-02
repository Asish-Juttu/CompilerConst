#ifndef _PARSER_DEF_
#define _PARSER_DEF_

#define NON_TERMINAL_SIZE 47
#define TERMINAL_SIZE 56

#include "token.h"

typedef enum {
    PROGRAM = 0, MAIN_FUNCTION, OTHER_FUNCTIONS, FUNCTION, INPUT_PAR, OUTPUT_PAR, 
    PARAMETER_LIST, DATATYPE, PRIMITIVE_DATATYPE, CONSTRUCTED_DATATYPE, 
    REMAINING_LIST, STMTS, TYPE_DEFINITIONS, TYPE_DEFINITION, FIELD_DEFINITIONS, 
    FIELD_DEFINITION, MORE_FIELDS, DECLARATIONS, DECLARATION, GLOBAL_OR_NOT, 
    OTHER_STMTS, STMT, ASSIGNMENT_STMT, SINGLE_OR_REC_ID, FUN_CALL_STMT, OUTPUT_PARAMETERS, 
    INPUT_PARAMETERS, ITERATIVE_STMT, CONDITIONAL_STMT,  IO_STMT,
    ARITHMETIC_EXPRESSION, OPERATOR,  BOOLEAN_EXPRESSION, VAR, LOGICAL_OP, RELATIONAL_OP, 
    RETURN_STMT, OPTIONAL_RETURN, ID_LIST, MORE_IDS, ACTUAL_OR_REDEFINED,
    DEFINE_TYPE_STATEMENT, FIELD_TYPE, OPTION_SINGLE_CONSTRUCTED, ONE_EXPANSION, MORE_EXPANSIONS,
    A, ELSE_PART, TERM, EXP_PRIME, LOW_PRECEDENCE_OPERATORS, FACTOR, TERM_PRIME, HIGH_PRECEDENCE_OPERATORS
} NonTerminal;

typedef struct{
    unsigned long long bitMask : NON_TERMINAL_SIZE;
} NonTermSet;

typedef struct{
    unsigned int isTerminal : 1;
    int symbol;
} Symbol;

typedef struct{
    int size;
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
} Grammar;

typedef struct  {
     Rule table[NON_TERMINAL_SIZE][TERMINAL_SIZE];
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

typedef struct {

} ParseTree;



NonTermSet nullNonTermSet();
NonTermSet singletonNonTermSet(NonTerminal nt);
NonTermSet nonTermSetUnion(NonTermSet s1, NonTermSet s2);
int equalsNonTermSet(NonTermSet s1, NonTermSet s2);

int isNullable(Grammar* grammar, NonTerminal nt);
char* nonTermToStr(NonTerminal nt);
#endif