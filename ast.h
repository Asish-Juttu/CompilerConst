#include "ast_def.h"
#include "parserDef.h"

void computeAst(ParseTree* parseTree);
void handleRule(ParseTreeElement* ptNode);
void assign(AstNode** lhs, AstNode** rhs);
