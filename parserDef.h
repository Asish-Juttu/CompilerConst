#ifndef _PARSER_DEF_
#define _PARSER_DEF_

#define NON_TERMINAL_SIZE 41
#define TERMINAL_SIZE 56

enum NonTerminal {
    PROGRAM, MAIN_FUNCTION, OTHER_FUNCTIONS, FUNCTION, INPUT_PAR, OUTPUT_PAR, 
    PARAMETER_LIST, DATATYPE, PRIMITIVE_DATATYPE, CONSTRUCTED_DATATYPE, 
    REMAINING_LIST, STMTS, TYPE_DEFINITIONS, TYPE_DEFINITION, FIELD_DEFINITIONS, 
    FIELD_DEFINITION, MORE_FIELDS, DECLARATIONS, DECLARATION, GLOBAL_OR_NOT, 
    OTHER_STMTS, STMT, ASSIGNMENT_STMT, SINGLE_OR_REC_ID, FUN_CALL_STMT, OUTPUT_PARAMETERS, 
    INPUT_PARAMETERS, ITERATIVE_STMT, CONDITIONAL_STMT,  IO_STMT, ARITHMETICEXPRESSION, 
    ARITHMETIC_EXPRESSION, OPERATOR,  BOOLEAN_EXPRESSION, VAR, LOGICAL_OP, RELATIONAL_OP, 
    RETURN_STMT, OPTIONAL_RETURN, ID_LIST, MORE_IDS
};


typedef struct{
    unsigned int isTerminal : 1;
    int symbol;
} Symbol;

typedef struct{
    int size;
    Symbol* symbols;
} Rule;

typedef struct{
    int size;
    Rule* rule;
} RuleArray;

typedef struct {
    int size;
    RuleArray* ruleArray;
} Grammar;

typedef struct  {
     Rule table[NON_TERMINAL_SIZE][TERMINAL_SIZE];
} ParseTable;

typedef struct {
    Rule first[NON_TERMINAL_SIZE];
    Rule follow[NON_TERMINAL_SIZE];
} FirstAndFollow;

typedef struct {

} ParseTree;

typedef struct{
    unsigned int bitMask : NON_TERMINAL_SIZE;
} NonTermSet;

NonTermSet nullNonTermSet();
NonTermSet singletonNonTermSet(NonTerm nt);
NonTermSet nonTermSetUnion(NonTermSet s1, NonTermSet s2);

#endif