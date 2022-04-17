#ifndef _TYPE_
#define _TYPE_


#include "typeDef.h"
#include "ast_def.h"

TypeExpression functionTypeExpression();
TypeExpression unionTypeExpression();
TypeExpression rnumTypeExpression();
TypeExpression numTypeExpression();
TypeExpression recordTypeExpression();
TypeExpression paramListTypeExpression();

void addToUnion(TypeExpression unionType, TypeExpression t);
void addToFuncInput(TypeExpression functionType, TypeExpression t);
void addToFuncOutput(TypeExpression funcitonType, TypeExpression t);
void addToRecord(TypeExpression recordType, TypeExpression t);

void setRecordName(char* name);

TypeExpression getOutputType(TypeExpression functionType);
TypeExpression getInputType(TypeExpression functionType);

extern const TypeExpression typeError;
TypeExpression typeVoid();
void printTypeExpr(TypeExpression t);

#endif