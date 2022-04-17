#include "ast.h"
#include "ast_def.h"

char* basicTypeToString(BasicType bt);
void handleTypeExpressionProgram(Ast_Program *astElement);
void handleTypeExpressionFunction(Ast_Function *astElement);
void handleTypeExpressionOtherFunctions(Ast_OtherFunctions *astElement);
void handleTypeExpressionParameterList(Ast_ParameterList *astElement);
void handleTypeExpressionStmts(Ast_Stmts *astElement);
void handleTypeExpressionParameterDeclaration(Ast_ParameterDeclaration *astElement);
void handleTypeExpressionDatatype(Ast_Datatype *astElement);
void handleTypeExpressionTypeDefinitions(Ast_TypeDefinitions *astElement);
void handleTypeExpressionDeclarations(Ast_Declarations *astElement);
void handleTypeExpressionOtherStmts(Ast_OtherStmts *astElement);
void handleTypeExpressionIdlist(Ast_IdList *astElement);
void handleTypeExpressionTypeDefinition(Ast_TypeDefinition *astElement);
void handleTypeExpressionDeclaration(Ast_Declaration *astElement);
int checkifEqual(TypeExpression exp1, TypeExpression exp2);
void handleTypeExpressionStmt(Ast_Stmt *astElement);
void handleTypeExpressionArithmeticExpression(Ast_ArithmeticExpression *astElement);
void handleTypeExpressionSingleOrRecId(Ast_SingleOrRecId *astElement);
void handleExpressionVar(Ast_Var *astElement);
void handleTypeExpressionBooleanExpression(Ast_BooleanExpression *astElement);
void handleTypeExpressionMain(Ast_Main* astElement);