#include "type.h"
#include <stdlib.h>
#include <stdio.h>

const TypeExpression typeError = (TypeExpression){BTYPE_ERROR, NULL};
const TypeExpression typeVoid = (TypeExpression){BTYPE_VOID, NULL};

const int INPUT_LIST = 0;
const int OUTPUT_LIST = 1;

ExpressionList* createExpressionList(){
    ExpressionList* expList = malloc(sizeof(ExpressionList));
    return expList;
}

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
    return functionType.expList->typeExpressionList[OUTPUT_LIST];
}

TypeExpression getInputType(TypeExpression functionType){
    if(!validate(functionType, BTYPE_FUNCTION)) return typeError;
    return functionType.expList->typeExpressionList[INPUT_LIST];
}

TypeExpression typeExpression(BasicType type){
    TypeExpression te = (TypeExpression){type, NULL, NULL};
    return te;
}

TypeExpression functionTypeExpression(){
    TypeExpression funcTE = typeExpression(BTYPE_FUNCTION);
    funcTE.expList = createExpressionList();

    TypeExpression inpTE = typeExpression(BTYPE_PARAM_LIST);
    TypeExpression outTE = typeExpression(BTYPE_PARAM_LIST);
    
    insertToExpList(funcTE.expList, inpTE);
    insertToExpList(funcTE.expList, outTE);

    return funcTE;
}

TypeExpression unionTypeExpression(){
    TypeExpression unionTE = typeExpression(BTYPE_UNION);
    unionTE.expList = createExpressionList();

    return unionTE;
}

TypeExpression rnumTypeExpression(){
    TypeExpression rnumTE = typeExpression(BTYPE_RNUM);
    rnumTE.expList = NULL;

    return rnumTE;
}

TypeExpression numTypeExpression(){
    TypeExpression numTE = typeExpression(BTYPE_NUM);
    numTE.expList = NULL;

    return numTE;
}

TypeExpression recordTypeExpression(char* name){
    TypeExpression recordTE = typeExpression(BTYPE_RECORD);
    recordTE.name = name;
    recordTE.expList = createExpressionList();

    return recordTE;
}

TypeExpression paramListTypeExpression(){
    TypeExpression paramListTE = typeExpression(BTYPE_PARAM_LIST);
    paramListTE.expList = createExpressionList();

    return paramListTE;
}