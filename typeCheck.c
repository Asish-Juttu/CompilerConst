#include "typeCheck.h"

int isTypeError(TypeExpression t){
    return t.basicType == BTYPE_ERROR;
}

void tcTypeDefinitions(Ast_TypeDefinitions* typeDefinitions){

}
void tcTypeDefinitions (Ast_TypeDefinitions* typeDefinitions);
void tcOtherStmts (Ast_OtherStmts* otherStmts);
void tcDeclarations (Ast_Declarations* declarations);
void tcIdList (Ast_IdList* idList);
void tcOtherFunctions (Ast_OtherFunctions* otherFunctions){
    
}
void tcParameterList (Ast_ParameterList* parameterList);
void tcFieldDefinitions (Ast_FieldDefinitions* fieldDefinitions);
void tcSingleOrRecId (Ast_SingleOrRecId* singleOrRecId);
void tcOptionSingleConstructed (Ast_OptionSingleConstructed* optionSingleConstructed);
void tcDatatype (Ast_Datatype* datatype);
void tcFieldDefinition (Ast_FieldDefinition* fieldDefinition);
void tcDefineTypeStmt (Ast_DefineTypeStmt* defineTypeStmt);
void tcAst_A(Ast_A*a);
void tcNum (Ast_Num*num);
void tcRnum (Ast_Rnum* rnum);
void tcDeclaration (Ast_Declaration* declaration){
    
}

void tcStmts (Ast_Stmts* stmts){
    int isTE = isTypeError(stmts->declarations->typeExpr) || isTypeError(stmts->otherStmts->typeExpr) || 
                        isTypeError(stmts->returnIds->typeExpr) || isTypeError(stmts->typeDefinitions->typeExpr);

    return isTE ? typeError : typeVoid;
}

void tcMain (Ast_Main* main){
    main->typeExpr = main->stmts->typeExpr;
}
void tcProgram (Ast_Program* program){
    program->typeExpr = (isTypeError(program->otherFunctions->typeExpr) ? typeError : program->mainFunction->typeExpr);
}
void tcFunction (Ast_Function* function);
void tcParameterDeclaration (Ast_ParameterDeclaration* parameterDeclaration);
void tcPrimitiveDatatype (Ast_PrimitiveDatatype* primitiveDatatype);
void tcConstructedDatatype (Ast_ConstructedDatatype* constructedDatatype);
void tcTypeDefinition (Ast_TypeDefinition* typeDefinition);
void tcArithmeticExpression (Ast_ArithmeticExpression* arithmeticExpression);
void tcAssignmentStmt (Ast_AssignmentStmt* assignmentStmt);
void tcFunCallStmt (Ast_FunCallStmt*funCallStmt);
void tcVar (Ast_Var*var);
void tcIterativeStmt (Ast_IterativeStmt*iterativeStmt);
void tcConditionalStmt (Ast_ConditionalStmt* conditionalStmt);
void tcIoStmt (Ast_IoStmt* ioStmt);
void tcElsePart (Ast_ElsePart* elsePart);