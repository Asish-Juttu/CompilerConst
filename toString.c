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

#include "token.h"
#include "parserDef.h"
#include "type.h"

char* tokToStr(Token t){
    switch(t){
        case  TK_ASSIGNOP: return "TK_ASSIGNOP";
        case  TK_OP: return "TK_OP";
        case  TK_AND: return "TK_AND";
        case  TK_AS: return "TK_AS";
        case  TK_CALL: return "TK_CALL";
        case  TK_CL: return "TK_CL";
        case  TK_COLON: return "TK_COLON";
        case  TK_COMMA: return "TK_COMMA";
        case  TK_COMMENT: return "TK_COMMENT";
        case  TK_DEFINETYPE: return "TK_DEFINETYPE";
        case  TK_DIV: return "TK_DIV";
        case  TK_DOT: return "TK_DOT";
        case  TK_ELSE: return "TK_ELSE";
        case  TK_END: return "TK_END";
        case  TK_ENDIF: return "TK_ENDIF";
        case  TK_ENDRECORD: return "TK_ENDRECORD";
        case  TK_ENDUNION: return "TK_ENDUNION";
        case  TK_ENDWHILE: return "TK_ENDWHILE";
        case  TK_FIELDID: return "TK_FIELDID";
        case  TK_FUNID: return "TK_FUNID";
        case  TK_GLOBAL: return "TK_GLOBAL";
        case  TK_GT: return "TK_GT";
        case  TK_ID: return "TK_ID";
        case  TK_IF: return "TK_IF";
        case  TK_INPUT: return "TK_INPUT";
        case  TK_INT: return "TK_INT";
        case  TK_LE: return "TK_LE";
        case  TK_LIST: return "TK_LIST";
        case  TK_LT: return "TK_LT";
        case  TK_MAIN: return "TK_MAIN";
        case  TK_MINUS: return "TK_MINUS";
        case  TK_MUL: return "TK_MUL";
        case  TK_NE: return "TK_NE";
        case  TK_NOT: return "TK_NOT";
        case  TK_NUM: return "TK_NUM";
        case  TK_OR: return "TK_OR";
        case  TK_OUTPUT: return "TK_OUTPUT";
        case  TK_PARAMETER: return "TK_PARAMETER";
        case  TK_PARAMETERS: return "TK_PARAMETERS";
        case  TK_PLUS: return "TK_PLUS";
        case  TK_READ: return "TK_READ";
        case  TK_REAL: return "TK_REAL";
        case  TK_RECORD: return "TK_RECORD";
        case  TK_RETURN: return "TK_RETURN";
        case  TK_RNUM: return "TK_RNUM";
        case  TK_RUID: return "TK_RUID";
        case  TK_SEM: return "TK_SEM";
        case  TK_SQL: return "TK_SQL";
        case  TK_SQR: return "TK_SQR";
        case  TK_THEN: return "TK_THEN";
        case  TK_TYPE: return "TK_TYPE";
        case  TK_UNION: return "TK_UNION";
        case  TK_WHILE: return "TK_WHILE";
        case  TK_WITH: return "TK_WITH";
        case  TK_WRITE: return "TK_WRITE";
        case  EPSILON: return "EPSILON";
        case  TK_GE: return "TK_GE";
        case  TK_EQ: return "TK_EQ";
        case ERROR_TOKEN: return "ERROR_TOKEN";
    }

    return "UNKNOWN";
} 

char* nonTermToStr(NonTerminal nt){
    switch(nt){
        case PROGRAM : return "<PROGRAM>";
        case MAIN_FUNCTION : return "<MAIN_FUNCTION>";
        case OTHER_FUNCTIONS : return "<OTHER_FUNCTIONS>";
        case FUNCTION : return "<FUNCTION>";
        case INPUT_PAR : return "<INPUT_PAR>";
        case OUTPUT_PAR : return "<OUTPUT_PAR>";
        case PARAMETER_LIST : return "<PARAMETER_LIST>";
        case DATATYPE : return "<DATATYPE>";
        case PRIMITIVE_DATATYPE : return "<PRIMITIVE_DATATYPE>";
        case CONSTRUCTED_DATATYPE : return "<CONSTRUCTED_DATATYPE>";
        case REMAINING_LIST : return "<REMAINING_LIST>";
        case STMTS : return "<STMTS>";
        case TYPE_DEFINITIONS : return "<TYPE_DEFINITIONS>";
        case TYPE_DEFINITION : return "<TYPE_DEFINITION>";
        case FIELD_DEFINITIONS : return "<FIELD_DEFINITIONS>";
        case FIELD_DEFINITION : return "<FIELD_DEFINITION>";
        case MORE_FIELDS : return "<MORE_FIELDS>";
        case DECLARATIONS : return "<DECLARATIONS>";
        case DECLARATION : return "<DECLARATION>";
        case GLOBAL_OR_NOT : return "<GLOBAL_OR_NOT>";
        case OTHER_STMTS : return "<OTHER_STMTS>";
        case STMT : return "<STMT>";
        case ASSIGNMENT_STMT : return "<ASSIGNMENT_STMT>";
        case SINGLE_OR_REC_ID : return "<SINGLE_OR_REC_ID>";
        case FUN_CALL_STMT : return "<FUN_CALL_STMT>";
        case OUTPUT_PARAMETERS : return "<OUTPUT_PARAMETERS>";
        case INPUT_PARAMETERS : return "<INPUT_PARAMETERS>";
        case ITERATIVE_STMT : return "<ITERATIVE_STMT>";
        case CONDITIONAL_STMT : return "<CONDITIONAL_STMT>";
        case IO_STMT : return "<IO_STMT>";
        case ARITHMETIC_EXPRESSION : return "<ARITHMETIC_EXPRESSION>";
        case BOOLEAN_EXPRESSION : return "<BOOLEAN_EXPRESSION>";
        case VAR : return "<VAR>";
        case LOGICAL_OP : return "<LOGICAL_OP>";
        case RELATIONAL_OP : return "<RELATIONAL_OP>";
        case RETURN_STMT : return "<RETURN_STMT>";
        case OPTIONAL_RETURN : return "<OPTIONAL_RETURN>";
        case ID_LIST : return "<ID_LIST>";
        case MORE_IDS : return "<MORE_IDS>";
        case ACTUAL_OR_REDEFINED : return "<ACTUAL_OR_REDEFINED>";
        case DEFINE_TYPE_STATEMENT: return "<DEFINE_TYPE_STATEMENT>";
        case FIELD_TYPE: return "<FIELD_TYPE>";
        case OPTION_SINGLE_CONSTRUCTED : return "<OPTION_SINGLE_CONSTRUCTED>";
        case ONE_EXPANSION : return "<ONE_EXPANSION>";
        case MORE_EXPANSIONS : return "<MORE_EXPANSIONS>";
        case A : return "<A>";
        case ELSE_PART : return "<ELSE_PART>";
        case TERM : return "<TERM>";
        case EXP_PRIME : return "<EXP_PRIME>";
        case LOW_PRECEDENCE_OPERATORS : return "<LOW_PRECEDENCE_OPERATORS>";
        case FACTOR : return "<FACTOR>";
        case TERM_PRIME : return "<TERM_PRIME>";
        case HIGH_PRECEDENCE_OPERATORS : return "<HIGH_PRECEDENCE_OPERATORS>";
    }

    return "<UNKNOWN>";
}

char* basicTypeToString(BasicType bt){
    switch(bt){
        case BTYPE_RNUM : return "RNUM";
        case BTYPE_NUM : return "NUM";
        case BTYPE_UNION : return "UNION";
        case BTYPE_RECORD : return "RECORD";
        case BTYPE_PARAM_LIST : return "PARAMETER LIST";
        case BTYPE_FIELD : return "FIELD";
        case BTYPE_FUNCTION : return "FUNCTION";
        case BTYPE_ERROR : return "TYPE ERROR";
        case BTYPE_TAGGED_UNION : return "TYPE TAGGED UNION";

        case BTYPE_VOID : return "VOID";
    }

    return "INVALID TYPE";
}

char* dtypeToStr(Datatype t){
    switch(t){
        case DT_PRIMITIVE : return "DT_PRIMITIVE";
        case DT_UNION : return "DT_UNION";
        case DT_RECORD : return "DT_RECORD";
        case DT_NUM : return "DT_NUM";
        case DT_RNUM  : return "DT_RNUM";
        case DT_REC_OR_UNION : return "DT_REC_OR_UNION";
        case DT_NONE : return "DT_NONE";
    }
    return "INVALID DATATYPE";
}