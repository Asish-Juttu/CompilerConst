#include "ast.h"
#include "string.h"

int equals(char* x, char*y){
    return strcmp(x, y) == 0;
}
AstNodeType toAstType(char* name){
    if(equals(name, "typedefintion")) return AST_TYPEDEFINITION;
    else if(equals(name, "otherStmts")) return AST_OTHERSTMTS;
    else if(equals(name, "declarations")) return AST_DECLARATION;
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
}

