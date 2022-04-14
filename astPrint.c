#include "astPrint.h"
#include <stdio.h>
void printTabs(int n){
    for(int i = 0; i < n; i++) printf("\t");
}

void print(void str, int n){
    printTabs(n);
    printf("%s", str);
}

void printDataType(Datatype dt, int n){

}
void printTypeDefinition(Ast_TypeDefinition* t, int n){
    print("Ast_TypeDefinition", n);
    n = n + 1;
    print(t->id1, n);
    printf("\n");
    print(t->id2, n);
    printf("\n");
    printDataType(t->datatype, n);
    printFieldDefinitions(t->fieldDefinitions, n);
}

void printOtherStmts(Ast_OtherStmts* t, int n){
    print("Ast_OtherStmts",n);
    n=n+1;
    AstList* list = t->otherStmtList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
        printStmts(nodeToAst(list->nodes[i], stmts), n);
    }



}

void printDeclarations(Ast_Declarations* t, int n){
    print("Ast_Declarations",n);
    n=n+1;
    AstList* list = t->declarationList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
        printDeclaration(nodeToAst(list->nodes[i], declaration), n);
    }

}

void printIdList(Ast_IdList* t, int n){
    print("Ast_IdList",n);
    n=n+1;
    AstList* list = t->idList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
        printId(nodeToAst(list->nodes[i], id), n);
    }


}

void printOtherFunctions(Ast_OtherFunctions* t, int n){
    print("Ast_OtherFunctions",n);
    n=n+1;
    AstList* list = t->functionList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
        printTypeDefinition(nodeToAst(list->nodes[i], function), n);
    }


}

void printParameterList(Ast_ParameterList* t, int n){
    print("Ast_ParameterList",n);
    n=n+1;
    AstList* list = t->parameterList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
        printParameterDeclaration(nodeToAst(list->nodes[i], parameterDeclaration), n);
    }

}

void printFieldDefinitions(Ast_FieldDefinitions* t, int n){
    print("Ast_FieldDefinitions",n);
    n=n+1;
    AstList* list = t->fieldDefinitionList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
        printFieldDefinition(nodeToAst(list->nodes[i], fieldDefinition), n);
    }

}

void printSingleOrRecId(Ast_SingleOrRecId* t,int n){
    print("Ast_SingleOrRecId",n);
    n = n + 1;
    print(t->id, n);
    printf("\n");
     AstList* list = t->fieldNameList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
       // print
    }
}

void printOptionSingleConstructed(Ast_OptionSingleConstructed* t, int n){
    print("Ast_OptionSingleConstructed",n);
    //print


}

void printDatatype(Ast_Datatype* t,int n){
     print("Ast_Datatype",n);
     print(t->name,n);
     printf("\n");

}

void printFieldDefinition(Ast_FieldDefinition* t, int n){
    print("Ast_FieldDefinition",n);
    printDataType(t->fieldType, n);
    print(t->id,n);
    printf("\n");

}

void printDefineTypeStmt(Ast_DefineTypeStmt* t, int n){
    print("Ast_FieldDefinition",n);
    printDataType(t->datatype, n);
    print(t->idOld,n);
    printf("\n");
    print(t->idNew,n);
    printf("\n");


    

}

void printA(Ast_A* t, int n){
    print("Ast_A",n);
    printDataType(t->datatype, n);


}

void printNum(Ast_Num* t, int n){
    print("Ast_Num",n);
    print(t->val,n);
    printf("\n");
    

}

void printRnum(Ast_Rnum* t, int n){
    print("Ast_Rnum",n);
    print(t->val,n);
    printf("\n");
    

}

void printDeclaration(Ast_Declaration* t, int n){
    print("Ast_Declaration",n);
    printDataType(t->type, n);
    print(t->id,n);
    printf("\n");
    print(t->isGlobal,n);
    printf("\n");




}

void printStmts(Ast_Stmts* t, int n){
    print("Ast_Stmts",n);
    printTypeDefinitions(t->typeDefinitions, n);
    printDeclarations(t->declarations, n);
    printOtherStmts(t->otherStmts, n);
    printId(t->returnIds,n);




}

void printMain(Ast_Main* t, int n){
    print("Ast_Main",n);
    printStmts(t->stmts,n);


}

void printProgram(Ast_Program* t, int n){
    print("Ast_Program",n);
    printOtherFunctions(t->otherFunctions,n);
    printMain(t->mainFunction,n);

}

void printFunction(Ast_Function* t, int n){
    print("Ast_Function",n);
    printParameterList(t->input_par,n);
    printParameterList(t->output_par,n);
    printStmts(t->stmts,n);


}

void printParameterDeclaration(Ast_ParameterDeclaration* t, int n){
    print("Ast_ParameterDeclaration",n);
    printDataType(t->datatype, n);
    print(t->id,n);

}

void printPrimitiveDatatype(Ast_PrimitiveDatatype* t, int n){
    print("Ast_PrimitiveDatatype",n);
    printDataType(t->datatype, n);

}

void printConstructedDatatype(Ast_ConstructedDatatype* t, int n){
    print("Ast_ConstructedDatatype",n);
    printDataType(t->datatype, n);


}

void printTypeDefinitions(Ast_TypeDefinitions* t, int n){
    print("Ast_TypeDefinitions", n);
    AstList* list = t->typeDefintionList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
        printTypeDefinition(nodeToAst(list->nodes[i], typeDefinition), n);
    }

}

void printArithmeticExpression(Ast_ArithmeticExpression* t, int n){
    print("Ast_ArithmeticExpression", n);
    printArithmeticExpression(t->left,n);
    printArithmeticExpression(t->right,n);
    printArithmeticOperator(t->op,n);





}

void printBooleanExpression(Ast_BooleanExpression* t, int n){
    print("Ast_ArithmeticExpression", n);
    printArithmeticExpression(t->left,n);
    printArithmeticExpression(t->right,n);


}

void printAssignmentStmt(Ast_AssignmentStmt* t, int n){
    print("Ast_AssignmentStmt", n);
    printSingleOrRecId(t->singleOrRecId,n);
    printArithmeticExpression(t->expression,n);

}

void printFunCallStmt(Ast_FunCallStmt* t, int n){
    print("Ast_FunCallStmt", n);
    printIdList(t->outputIdList,n);
    printIdList(t->inputIdList,n);
    print(t->funId,n);
    printf("\n");

}

void printVar(Ast_Var* t, int n){
    print("Ast_Var", n);
    
//

}

void printIterativeStmt(Ast_IterativeStmt* t, int n){
    print("Ast_IterativeStmt", n);
    printBooleanExpression(t->condition,n);
    printOtherStmts(t->body,n);


}

void printConditionalStmt(Ast_ConditionalStmt* t, int n){
    print("Ast_ConditionalStmt", n);
    printBooleanExpression(t->condition,n);
    printOtherStmts(t->body,n);
    printOtherStmts(t->elsePart,n);

}

void printIoStmt(Ast_IoStmt* t, int n){
    
    


}

void elsePart(Ast_ElsePart* t){

}

void logicalOperator(Ast_LogicalOperator* t, int n){
    print("Ast_LogicalOperator", n);
    printLogicalOperator(t->op,n);

}

void relationalOperator(Ast_RelationalOperator* t, int n){
    print("Ast_RelationalOperator", n);
    printLogicalOperator(t->op,n);

}

void arithmeticOperator(Ast_ArithmeticOperator* t, int n){
    print("Ast_ArithmeticOperator", n);
    printLogicalOperator(t->op,n);

}

void moreExpansion(Ast_MoreExpansion* t, int n){
    print("Ast_MoreExpansion", n);
    printLogicalOperator(t->op,n);

}
