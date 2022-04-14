#include "type.h"
#include <stdlib.h>
#include <stdio.h>

const TypeExpression typeError = (TypeExpression){BTYPE_ERROR, NULL};
const TypeExpression typeVoid = (TypeExpression){BTYPE_VOID, NULL};

void growIfFull(ExpressionList* list){
    if(list->size == list->cap){
        list->cap = (int)(list->cap * 1.4 + 1);
        list->typeExpressionList = realloc(list->typeExpressionList, list->cap * sizeof(TypeExpression));
    }
}

void insertToExpList(ExpressionList* list, TypeExpression t){
    list->typeExpressionList[list->size] = t;
    list->size++;
}

int validate(TypeExpression t, BasicType basicType){
    if(t.basicType != basicType){
        printf("Attempting to use type %s as type %s", basicTypeToString(t.basicType), basicTypeToString(basicType));
        return 0;
    }

    return 1;
}

void addToUnion(TypeExpression unionType, TypeExpression t){
    if(!validate(unionType, BTYPE_UNION)) return;
    insertToExpList(unionType.expList, t);
}

void addToFuncInput(TypeExpression functionType, TypeExpression t){
    if(!validate(functionType, BTYPE_FUNCTION)) return;
    TypeExpression inputType = getInputType(functionType);
    insertToExpList(inputType.expList, t);
}

void addToFuncOutput(TypeExpression functionType, TypeExpression t){
    if(!validate(functionType, BTYPE_FUNCTION)) return;
    TypeExpression outputType = getOutputType(functionType);
    insertToExpList(outputType.expList, t);
}

void addToRecord(TypeExpression recordType, TypeExpression t){
    if(!validate(recordType, BTYPE_RECORD)) return;
    insertToExpList(recordType.expList, t);
}

TypeExpression getOutputType(TypeExpression functionType){
    if(!validate(functionType, BTYPE_FUNCTION)) return typeError;
    return functionType.expList->typeExpressionList[1];
}

TypeExpression getInputType(TypeExpression functionType){
    if(!validate(functionType, BTYPE_FUNCTION)) return typeError;
    return functionType.expList->typeExpressionList[0];
}

