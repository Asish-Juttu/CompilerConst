#include "type.h"
#include "ast_def.h"
#include <stdlib.h>
#include <stdio.h>

const TypeExpression typeError = (TypeExpression){BTYPE_ERROR, NULL};
const TypeExpression typeVoid = (TypeExpression){BTYPE_VOID, NULL};

const int INPUT_LIST = 0;
const int OUTPUT_LIST = 1;

ExpressionList *createExpressionList()
{
    ExpressionList *expList = malloc(sizeof(ExpressionList));
    return expList;
}

void growIfFull(ExpressionList *list)
{
    if (list->size == list->cap)
    {
        list->cap = (int)(list->cap * 1.4 + 1);
        list->typeExpressionList = realloc(list->typeExpressionList, list->cap * sizeof(TypeExpression));
    }
}

void insertToExpList(ExpressionList *list, TypeExpression t)
{
    list->typeExpressionList[list->size] = t;
    list->size++;
}

int validate(TypeExpression t, BasicType basicType)
{
    if (t.basicType != basicType)
    {
        printf("Attempting to use type %s as type %s", basicTypeToString(t.basicType), basicTypeToString(basicType));
        return 0;
    }

    return 1;
}

void addToUnion(TypeExpression unionType, TypeExpression t)
{
    if (!validate(unionType, BTYPE_UNION))
        return;
    insertToExpList(unionType.expList, t);
}

void addToFuncInput(TypeExpression functionType, TypeExpression t)
{
    if (!validate(functionType, BTYPE_FUNCTION))
        return;
    TypeExpression inputType = getInputType(functionType);
    insertToExpList(inputType.expList, t);
}

void addToFuncOutput(TypeExpression functionType, TypeExpression t)
{
    if (!validate(functionType, BTYPE_FUNCTION))
        return;
    TypeExpression outputType = getOutputType(functionType);
    insertToExpList(outputType.expList, t);
}

void addToRecord(TypeExpression recordType, TypeExpression t)
{
    if (!validate(recordType, BTYPE_RECORD))
        return;
    insertToExpList(recordType.expList, t);
}

TypeExpression getOutputType(TypeExpression functionType)
{
    if (!validate(functionType, BTYPE_FUNCTION))
        return typeError;
    return functionType.expList->typeExpressionList[OUTPUT_LIST];
}

TypeExpression getInputType(TypeExpression functionType)
{
    if (!validate(functionType, BTYPE_FUNCTION))
        return typeError;
    return functionType.expList->typeExpressionList[INPUT_LIST];
}

TypeExpression typeExpression(BasicType type)
{
    TypeExpression te = (TypeExpression){type, NULL, NULL};
    return te;
}

TypeExpression functionTypeExpression()
{
    TypeExpression funcTE = typeExpression(BTYPE_FUNCTION);
    funcTE.expList = createExpressionList();

    TypeExpression inpTE = typeExpression(BTYPE_PARAM_LIST);
    TypeExpression outTE = typeExpression(BTYPE_PARAM_LIST);

    insertToExpList(funcTE.expList, inpTE);
    insertToExpList(funcTE.expList, outTE);

    return funcTE;
}

TypeExpression unionTypeExpression()
{
    TypeExpression unionTE = typeExpression(BTYPE_UNION);
    unionTE.expList = createExpressionList();

    return unionTE;
}

TypeExpression rnumTypeExpression()
{
    TypeExpression rnumTE = typeExpression(BTYPE_RNUM);
    rnumTE.expList = NULL;

    return rnumTE;
}

TypeExpression numTypeExpression()
{
    TypeExpression numTE = typeExpression(BTYPE_NUM);
    numTE.expList = NULL;

    return numTE;
}

TypeExpression recordTypeExpression(char *name)
{
    TypeExpression recordTE = typeExpression(BTYPE_RECORD);
    recordTE.name = name;
    recordTE.expList = createExpressionList();

    return recordTE;
}

TypeExpression paramListTypeExpression()
{
    TypeExpression paramListTE = typeExpression(BTYPE_PARAM_LIST);
    paramListTE.expList = createExpressionList();

    return paramListTE;
}

// I am basically writing function for each and every ast element separately.

void handleTypeExpressionProgram(Ast_Program *astElement)
{
    handleTypeExpressionFunction(astElement->mainFunction);
    handleTypeExpressionOtherFunctions(astElement->otherFunctions);
    if ((astElement->mainFunction->typeExpr.basicType == BTYPE_ERROR) || (astElement->otherFunctions->typeExpr.basicType == BTYPE_ERROR))
    {
        astElement->typeExpr = typeExpression(BTYPE_ERROR);
    }
    else
    {
        astElement->typeExpr = typeExpression(BTYPE_VOID);
    }
}

void handleTypeExpressionFunction(Ast_Function *astElement)
{
    handleTypeExpressionParameterList(astElement->input_par);
    handleTypeExpressionParameterList(astElement->output_par);
    handleTypeExpressionStmts(astElement->stmts);
    if ((astElement->input_par->typeExpr.basicType == BTYPE_ERROR) || (astElement->output_par->typeExpr.basicType == BTYPE_ERROR) || (astElement->stmts->typeExpr.basicType == BTYPE_ERROR))
    {
        astElement->typeExpr.basicType = BTYPE_ERROR;
        astElement->typeExpr.expList = NULL;
    }
    else
    {
        astElement->typeExpr = functionTypeExpression();
        astElement->typeExpr.expList->typeExpressionList[0] = astElement->input_par->typeExpr;
        astElement->typeExpr.expList->typeExpressionList[1] = astElement->output_par->typeExpr;
    }
}

void handleTypeExpressionOtherFunctions(Ast_OtherFunctions *astElement)
{
    int isError = 0;
    for (int i = 0; i < astElement->functionList->size; i++)
    {
        Ast_Function *f = astElement->functionList->nodes[i]->node.function;
        handleTypeExpressionFunction(f);
        if (f->typeExpr.basicType == BTYPE_ERROR)
        {
            isError = 1;
            astElement->typeExpr.basicType = BTYPE_ERROR;
        }
    }
    if (isError == 0)
    {
        astElement->typeExpr.basicType = BTYPE_VOID;
        astElement->typeExpr.expList = NULL;
    }
}

void handleTypeExpressionParameterList(Ast_ParameterList *astElement)
{
    int isError = 0;
    for (int i = 0; i < astElement->parameterList->size; i++)
    {
        Ast_ParameterDeclaration *paradec = astElement->parameterList->nodes[i]->node.parameterDeclaration;
        handleTypeExpressionParameterDeclaration(paradec);
        if (paradec->typeExpr.basicType == BTYPE_ERROR)
        {
            isError = 1;
        }
    }

    if (isError == 0)
    {
        astElement->typeExpr.basicType = BTYPE_ERROR;
        astElement->typeExpr.expList = NULL;
    }
    else
    {
        astElement->typeExpr.basicType = BTYPE_PARAM_LIST;
        astElement->typeExpr.expList = createExpressionList();
        for (int i = 0; i < astElement->parameterList->size; i++)
        {
            if (astElement->typeExpr.expList->size == cap)
            {
                growIfFull(astElement->typeExpr.expList);
            }
            Ast_ParameterDeclaration *paradec = astElement->parameterList->nodes[i]->node.parameterDeclaration;
            insertToExpList(astElement->typeExpr.expList, paradec->typeExpr);
        }
    }
}

void handleTypeExpressionStmts(Ast_Stmts *astElement)
{
    handleTypeExpressionTypeDefinitions(astElement->typeDefinitions);
    handleTypeExpressionDeclarations(astElement->declarations);
    handleTypeExpressionOtherStmts(astElement->otherStmts);
    handleTypeExpressionIdlist(astElement->returnIds);
    if ((astElement->declarations->typeExpr.basicType == BTYPE_ERROR) || (astElement->typeDefinitions->typeExpr.basicType == BTYPE_ERROR) || (astElement->otherStmts->typeExpr.basicType == BTYPE_ERROR) || (astElement->returnIds->typeExpr.basicType == BTYPE_ERROR))
    {
        astElement->typeExpr.basicType = BTYPE_ERROR;
        astElement->typeExpr.expList = NULL;
    }
    else
    {
        astElement->typeExpr.basicType = BTYPE_VOID;
        astElement->typeExpr.expList = NULL;
    }
}

void handleTypeExpressionParameterDeclaration(Ast_ParameterDeclaration *astElement)
{
    handleTypeExpressionDatatype(astElement->datatype);

    if (astElement->datatype->typeExpr.basicType == BTYPE_ERROR)
    {
        astElement->typeExpr.basicType = BTYPE_ERROR;
        astElement->typeExpr.expList = NULL;
    }
    else
    {
        astElement->typeExpr = astElement->datatype->typeExpr;
    }
}

void handleTypeExpressionDatatype(Ast_Datatype *astElement)
{
    if (astElement->datatype == DT_NUM)
    {
        astElement->typeExpr.basicType = BTYPE_NUM;
        astElement->typeExpr.expList = NULL;
    }
    else if (astElement->datatype == DT_RECORD)
    {
        astElement->typeExpr.basicType = BTYPE_RECORD;
        astElement->typeExpr.expList = NULL; // it should not be null but where can I get the list of fields in the record.
        astElement->typeExpr.name = astElement->name;
    }
    else if (astElement->datatype == DT_RNUM)
    {
        astElement->typeExpr.basicType = BTYPE_RNUM;
        astElement->typeExpr.expList = NULL;
    }
    else if (astElement->datatype == DT_UNION)
    {
        astElement->typeExpr.basicType = BTYPE_UNION;
        astElement->typeExpr.expList = NULL; // it should not be null but where can I get the list of fields in the union.
        astElement->typeExpr.name = astElement->name;
    }
    else
    {
        printf("Error: Datatype does not exist\n");
        astElement->typeExpr.basicType = BTYPE_ERROR;
        astElement->typeExpr.expList = NULL;
        return;
    }
}

void handleTypeExpressionTypeDefinitions(Ast_TypeDefinitions *astElement)
{
    int isError = 0;
    for(int i=0;i<astElement->typeDefintionList->size;i++){
        Ast_TypeDefinition *typedeff  = astElement->typeDefintionList->nodes[i]->node.typeDefinition;
        handleTypeExpressionTypeDefinition(typedeff);
        if(typedeff->typeExpr.basicType == BTYPE_ERROR){
            isError = 1;
        }
    }
    if(isError){
        astElement->typeExpr.basicType = BTYPE_ERROR;
        astElement->typeExpr.expList = NULL;
    }else{
        astElement->typeExpr.basicType = BTYPE_VOID;
        astElement->typeExpr.expList = NULL;
    }
}

void handleTypeExpressionDeclarations(Ast_Declarations *astElement)
{
       int isError = 0;
       for(int i=0;i<astElement->declarationList->size;i++){
           Ast_Declaration *astdec = astElement->declarationList->nodes[i]->node.declaration;
           handleTypeExpressionDeclaration(astdec);
           if(astdec->typeExpr.basicType == BTYPE_ERROR){
               isError = 1;
           }
       }
       if(isError){
           astElement->typeExpr.basicType = BTYPE_ERROR;
           astElement->typeExpr.expList = NULL;
       } else{
           astElement->typeExpr.basicType = BTYPE_VOID;
           astElement->typeExpr.expList = NULL;
       }
}

void handleTypeExpressionOtherStmts(Ast_OtherStmts *astElement)
{
     int isError = 0;
     for(int i=0;i<astElement->otherStmtList->size;i++){
         Ast_Stmt *statement = astElement->otherStmtList->nodes[i]->node.stmt;
         handleTypeExpressionStmt(statement);
         if(statement->type == STMT_ASSIGN){
             if(statement->stmtUnion.assignStmt->typeExpr.basicType == BTYPE_ERROR){
                 isError = 1;
             }
         }
         else if(statement->type == STMT_COND){
             if(statement->stmtUnion.condStmt->typeExpr.basicType == BTYPE_ERROR){
                 isError = 1;
             } 
         }
         else if(statement->type == STMT_ITER){
             if(statement->stmtUnion.iterStmt->typeExpr.basicType == BTYPE_ERROR){
                 isError = 1;
             }
         }
         else if(statement->type == STMT_FUN_CALL){
             if(statement->stmtUnion.funCallStmt->typeExpr.basicType == BTYPE_ERROR){
                 isError = 1;
             } 
         }
         else if(statement->type == STMT_IO){
             if(statement->stmtUnion.ioStmt->typeExpr.basicType == BTYPE_ERROR){
                 isError = 1;
             }
         }
     }
     if(isError){
         astElement->typeExpr.basicType = BTYPE_ERROR;
         astElement->typeExpr.expList = NULL;
     }else{
         astElement->typeExpr.basicType = BTYPE_VOID;
         astElement->typeExpr.expList = NULL;
     }
}

void handleTypeExpressionIdlist(Ast_IdList *astElement)
{
}

void handleTypeExpressionTypeDefinition(Ast_TypeDefinition *astElement){

}

void handleTypeExpressionDeclaration(Ast_Declaration *astElement){

}

void handleTypeExpressionStmt(Ast_Stmt *astElement){

}