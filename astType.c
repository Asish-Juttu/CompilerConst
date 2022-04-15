#include "ast.h"
#include "string.h"

int equals(char* x, char*y){
    return strcmp(x, y) == 0;
}
AstNodeType toAstType(char* name){
    if(equals(name, "typeDefinitions")) return AST_TYPEDEFINITIONS;
    else if(equals(name, "otherStmts")) return AST_OTHERSTMTS;
    else if(equals(name, "declarations")) return AST_DECLARATIONS;
    else if(equals(name, "idList")) return AST_IDLIST;
    else if(equals(name, "otherFunctions")) return AST_OTHERFUNCTIONS;
    else if(equals(name, "parameterList")) return AST_PARAMETERLIST;
    else if(equals(name, "fieldDefinitions")) return AST_FIELDDEFINITIONS;
    else if(equals(name, "singleOrRecId")) return AST_SINGLEORRECID;
    else if(equals(name, "optionSingleConstructed")) return AST_OPTIONSINGLECONSTRUCTED;
    else if(equals(name, "datatype")) return AST_DATATYPE;
    else if(equals(name, "fieldDefinition")) return AST_FIELDDEFINITION;
    else if(equals(name, "defineTypeStmt")) return AST_DEFINETYPESTMT;
    else if(equals(name, "a")) return AST_A;
    else if(equals(name, "num")) return AST_NUM;
    else if(equals(name, "rnum")) return AST_RNUM;
    else if(equals(name, "declaration")) return AST_DECLARATION;
    else if(equals(name, "stmts")) return AST_STMTS;
    else if(equals(name, "mainFunction")) return AST_MAIN;
    else if(equals(name, "program")) return AST_PROGRAM;
    else if(equals(name, "function")) return AST_FUNCTION;
    else if(equals(name, "parameterDeclaration")) return AST_PARAMETERDECL;
    else if(equals(name, "primitiveDatatype")) return AST_PRIMITIVEDT;
    else if(equals(name, "constructedDatatype")) return AST_CONSTRDT;
    else if(equals(name, "typeDefinition")) return AST_TYPEDEFINITION;
    else if(equals(name, "arithmeticExporession")) return AST_ARITHMETICEXPR;
    else if(equals(name, "assignmentStmt")) return AST_ASSIGNMENTSTMT;
    else if(equals(name, "funCallStmt")) return AST_FUNCALLSTMT;
    else if(equals(name, "var")) return AST_VAR;
    else if(equals(name, "iterativeStmt")) return AST_ITERATIVESTMT; 
    else if(equals(name, "conditionalStmt")) return AST_CONDITIONALSTMT;
    else if(equals(name, "ioStmt")) return AST_IOSTMT;
    else if(equals(name, "elsePart")) return AST_ELSEPART;
    else return AST_UNKNOWN;
}

char* astToStr(AstNodeType type){
    switch(type){
        case AST_TYPEDEFINITIONS : return "AST_TYPEDEFINITIONS";
        case AST_OTHERSTMTS : return "AST_OTHERSTMTS";
        case AST_DECLARATIONS : return "AST_DECLARATIONS";
        case AST_IDLIST : return "AST_IDLIST";
        case AST_OTHERFUNCTIONS : return "AST_OTHERFUNCTIONS";
        case AST_PARAMETERLIST : return "AST_PARAMETERLIST";
        case AST_FIELDDEFINITIONS : return "AST_FIELDDEFINITIONS";
        case AST_SINGLEORRECID : return "AST_SINGLEORRECID";
        case AST_OPTIONSINGLECONSTRUCTED : return "AST_OPTIONSINGLECONSTRUCTED";
        case AST_DATATYPE : return "AST_DATATYPE";
        case AST_FIELDDEFINITION : return "AST_FIELDDEFINITION";
        case AST_DEFINETYPESTMT : return "AST_DEFINETYPESTMT";
        case AST_A : return "AST_A";
        case AST_NUM : return "AST_NUM";
        case AST_RNUM : return "AST_RNUM";
        case AST_DECLARATION : return "AST_DECLARATION";
        case AST_STMTS : return "AST_STMTS";
        case AST_MAIN : return "AST_MAIN";
        case AST_PROGRAM : return "AST_PROGRAM";
        case AST_FUNCTION : return " AST_FUNCTION";
        case AST_PARAMETERDECL : return "AST_PARAMETERDECL";
        case AST_PRIMITIVEDT : return "AST_PRIMITIVEDT";
        case AST_CONSTRDT : return "AST_CONSTRDT";
        case AST_TYPEDEFINITION : return "AST_TYPEDEFINITION";
        case AST_ARITHMETICEXPR : return "AST_ARITHMETICEXPR";
        case AST_ASSIGNMENTSTMT : return "AST_ASSIGNMENTSTMT";
        case AST_FUNCALLSTMT : return "AST_FUNCALLSTMT";
        case AST_VAR : return "AST_VAR";
        case AST_ITERATIVESTMT : return "AST_ITERATIVESTMT";
        case AST_CONDITIONALSTMT : return "AST_CONDITIONALSTMT";
        case AST_IOSTMT : return "AST_IOSTMT";
        case AST_ELSEPART : return "AST_ELSEPART";
                                          
    }

    return "AST_UNKNOWN";
}