#include "astPrint.h"
#include <stdio.h>
void printTabs(int n){
    for(int i = 0; i < n; i++) printf("  ");
}

void print(char* str, int n){
    printTabs(n);
    printf("%s ", str);
}

void printFloat(float f, int n){
    printTabs(n);
    printf("%f ", f);
}

void printInt(int i, int n){
    printTabs(n);
    printf("%d ", i);
}

void printId(Ast_Id* id, int n){
    print("Ast_Id", n);
    print(id->id, 0);
}
void printDtype(Datatype dt, int n){
    char* str = dtypeToStr(dt);
    print(str, n);
}

void printDatatype(Ast_Datatype* t, int n){
    print("Ast_Datatype:", n);
    printDtype(t->datatype, 0);
    if(t->name != NULL)
        print(t->name, 0);
}
void printTypeDefinition(Ast_TypeDefinition* t, int n){
    print("\nAst_TypeDefinition:\n", n);
    printf("Is null %d\n", t == NULL);
    n = n + 1;

    if(t->type == TDEF){
        printDtype(tdef(t)->datatype, n);
        print(tdef(t)->id, 0);
        printf("\n");
        AstList* list = tdef(t)->fieldDefinitions->fieldDefinitionList;
        int size = list->size;
        n = n + 1;
        for(int i = 0; i < size; i++){
            printFieldDefinition(nodeToAst(list->nodes[i], fieldDefinition)
                , n);
        }
    }
    else if(t->type == TRDEF){
        print(trdef(t)->from, n);
        print("as", 0);
        print(trdef(t)->to, 0);
    }
}

void printOtherStmts(Ast_OtherStmts* t, int n){
    print("\nAst_OtherStmts:\n",n);
    n=n + 1;
    AstList* list = t->otherStmtList;
    for(int i = 0; i < list->size; i++){
        printStmt(nodeToAst(list->nodes[i], stmt), n);
        printf("\n");
    }
}

void printDeclarations(Ast_Declarations* t, int n){
    print("Ast_Declarations\n",n);
    //n=n+1;
    AstList* list = t->declarationList;
    n = n + 1;
    printTabs(n);
    for(int i = 0; i < list->size; i++){
        printDeclaration(nodeToAst(list->nodes[i], declaration), n);
        printf("\n");
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
    print("Ast_OtherFunctions\n",n);
    n=n+1;
    AstList* list = t->functionList;
    for(int i = 0; i < list->size; i++){
        printFunction(nodeToAst(list->nodes[i], function), n);
    }
}

void printParameterList(Ast_ParameterList* t, int n){
    print("Ast_ParameterList:",n);
    AstList* list = t->parameterList;
    for(int i = 0; i < list->size; i++){
        printParameterDeclaration(nodeToAst(list->nodes[i], parameterDeclaration), 0);
        print(",", 0);
    }
}

void printFieldDefinitions(Ast_FieldDefinitions* t, int n){
    print("Ast_FieldDefinitions",n);
    n=n+1;
    AstList* list = t->fieldDefinitionList;
    for(int i = 0; i < list->size; i++){
        printFieldDefinition(nodeToAst(list->nodes[i], fieldDefinition), n);
    }

}

void printSingleOrRecId(Ast_SingleOrRecId* t,int n){
    print("Ast_SingleOrRecId",n);
    n = n + 1;
    print(t->id, n);
    printf("");
     AstList* list = t->fieldNameList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
       // print
    }
}

void printOptionSingleConstructed(Ast_OptionSingleConstructed* t, int n){
    print("Ast_OptionSingleConstructed",n);
}

// void printDatatype(Ast_Datatype* t,int n){
//      print("Ast_Datatype",n);
//      print(t->name,n);
// }

void printFieldDefinition(Ast_FieldDefinition* t, int n){
    print("Ast_FieldDefinition",n);
    printDatatype(t->fieldType, n);
    print(t->id,n);
}

void printA(Ast_A* t, int n){
    print("Ast_A:",n);
    printDtype(t->datatype, 0);
}

void printNum(Ast_Num* t, int n){
    print("Ast_Num:",n);
    printInt(t->val,0);
}

void printRnum(Ast_Rnum* t, int n){
    print("Ast_Rnum:",n);
    printFloat(t->val,0);
}

void printDeclaration(Ast_Declaration* t, int n){
    print("Ast_Declaration:",n);

    print(t->isGlobal ? "(Global)" : "",0);
    printDatatype(t->datatype, 0);
    print(t->id,0);
}

void printStmt(Ast_Stmt* stmt, int n){
    if(stmt->type == STMT_ASSIGN){
        printAssignmentStmt(assignStmt(stmt), n);
    }
    else if(stmt->type == STMT_COND){
        printConditionalStmt(condStmt(stmt), n);
    }
    else if(stmt->type == STMT_ITER){
        printIterativeStmt(iterStmt(stmt), n);
    }
    else if(stmt->type == STMT_IO){
        printIoStmt(ipopStmt(stmt), n);
    }
    else if(stmt->type == STMT_FUN_CALL){
        printFunCallStmt(fCallStmt(stmt), n);
    }
}
void printStmts(Ast_Stmts* t, int n){
    print("Ast_Stmts:\n",n);
    n = n + 1;
    printf("Is t Null ? %d\n", t == NULL);
    printTypeDefinitions(t->typeDefinitions, n);
    printf("\n");

    printDeclarations(t->declarations, n);
    printf("\n");

    printOtherStmts(t->otherStmts, n);
    printf("\n");

    printIdList(t->returnIds,n);
}

void printMain(Ast_Main* t, int n){
    print("Ast_Main:\n",n);
    n = n + 1;
    printStmts(t->stmts,n);
}

void printProgram(Ast_Program* t, int n){
    print("Ast_Program:\n",n);
    n = n + 1;
    printOtherFunctions(t->otherFunctions,n);
    printMain(t->mainFunction,n);
}

void printFunction(Ast_Function* t, int n){
    print("Ast_Function:\n",n);
    
    n = n + 1;
    print("Name:", n);
    print(t->funId, 0);
    printf("\n");

    print("Input Params :", n);
    printParameterList(t->input_par,0);

    printf("\n");
    print("Output Params :", n);
    printParameterList(t->output_par,0);
    printf("\n");
    printStmts(t->stmts,n);
}

void printParameterDeclaration(Ast_ParameterDeclaration* t, int n){
    print("Ast_ParameterDeclaration:",n);
    printDatatype(t->datatype, n);
    print(t->id,n);

}

void printPrimitiveDatatype(Ast_PrimitiveDatatype* t, int n){
    print("Ast_PrimitiveDatatype:",n);
    printDatatype(t->datatype, n);

}

void printConstructedDatatype(Ast_ConstructedDatatype* t, int n){
    print("Ast_ConstructedDatatype:",n);
    printDatatype(t->datatype, n);
}

void printTypeDefinitions(Ast_TypeDefinitions* t, int n){
    print("Ast_TypeDefinitions:", n);
    AstList* list = t->typeDefintionList;
    n = n + 1;
    for(int i = 0; i < list->size; i++){
        printTypeDefinition(nodeToAst(list->nodes[i], typeDefinition), n);
    }

}

void printAexp(ArithmeticExpression* aexp, int n){
    print("(", 0);
    printArithmeticExpression(aexp->left, 0);
    print(")", 0);

    printAop(aexp->op, 0);

    print("(", 0);
    printArithmeticExpression(aexp->right, 0);
    print(")", 0);

}
void printArithmeticExpression(Ast_ArithmeticExpression* t, int n){
    print("Ast_ArithmeticExpression:", n);
    
    if(t->aexpType == AEXP_EXP){
        printAexp(expAexp(t), n);
    }
    else if(t->aexpType == AEXP_VAR){
        printVar(varAexp(t), n);
    }
    else{
        print("\nAexp type not init\n", 0);
    }
}

void printRelop(RelationalOperator op, int n){
    if(op == ROP_Eq){
        print("==", n);
    }
    else if(op == ROP_Ge){
        print(">=", n);
    }
    else if(op == ROP_Gt){
        print(">", n);
    }
    else if(op == ROP_Le){
        print("<=", n);
    }
    else if(op == ROP_Lt){
        print("<", n);
    }
    else if(op == ROP_Ne){
        print("!=", n);
    }
}
void printBooleanExpression(Ast_BooleanExpression* t, int n){
    print("Ast_ArithmeticExpression:", n);
    n = n + 1;

    if(t->bexpType == BEXP_BOOL_OP){
        printBooleanExpression(boolOpBexp(t)->left, n);
        printLop(boolOpBexp(t)->op, n);
        printBooleanExpression(boolOpBexp(t)->right, n);

    }
    else if(t->bexpType == BEXP_VAR_COMP){
        printVar(varCompBexp(t)->left, n);
        printRelop(varCompBexp(t)->op, 0);
        printVar(varCompBexp(t)->right, 0);
        printf("\n");
    }
}

void printAssignmentStmt(Ast_AssignmentStmt* t, int n){
    print("Ast_AssignmentStmt:", n);
    n = n + 1;
    printSingleOrRecId(t->singleOrRecId,n);
    printf(" = ");
    printArithmeticExpression(t->expression, 0);

}

void printFunCallStmt(Ast_FunCallStmt* t, int n){
    print("Ast_FunCallStmt", n);
    n = n + 1;
    
    printIdList(t->outputIdList, 0);
    printf("<---");
    print(t->funId, 0);
    printIdList(t->inputIdList, 0);

}

void printVar(Ast_Var* t, int n){
    print("Ast_Var:", n);
    if(t->varType == VAR_ID){
        print("[", 0);
        print(varVar(t)->id, 0);
        print("]", 0);
    }
    else if(t->varType == VAR_NUM){
        print("[", 0);
        printInt(numVar(t)->val, 0);
        print("]", 0);
    }
    else if(t->varType == VAR_RNUM){
        print("[", 0);
        printFloat(rnumVar(t)->val, 0);
        print("]", 0);
    }
    else{
        printf("<#####>%d", t->varType);
    }
}

void printIterativeStmt(Ast_IterativeStmt* t, int n){
    print("Ast_IterativeStmt:", n);
    printBooleanExpression(t->condition,n);
    printOtherStmts(t->body,n);
}

void printConditionalStmt(Ast_ConditionalStmt* t, int n){
    print("Ast_ConditionalStmt:", n);
    printBooleanExpression(t->condition,n);
    printOtherStmts(t->body,n);
    printOtherStmts(t->elsePart,n);

}

void printIoStmt(Ast_IoStmt* t, int n){
    print("Ast_IoStmt:", n);
    n = n + 1;
    if(t->ioType == IO_READ){
        print("Read", n);
    }
    else if(t->ioType == IO_WRITE){
        print("Write", n);
    }
    printVar(t->var, n);
}

void elsePart(Ast_ElsePart* t, int n){
    print("Ast_ElsePart\n", n);
    n = n + 1;
    printOtherStmts(t->otherStmts, n);
}

void printLop(LogicalOperator lop, int n){
    if(lop == LOP_AND){
        print("AND", n);
    }
    else if(lop == LOP_OR){
        print("OR", n);
    }
    else if(lop == LOP_NOT){
        print("NOT", n);
    }
}
void printLogicalOperator(Ast_LogicalOperator* t, int n){
    print("Ast_LogicalOperator:", n);
    n = n + 1;
    printLop(t->op,n);
}

void relationalOperator(Ast_RelationalOperator* t, int n){
    print("Ast_RelationalOperator:", n);
    printRelop(t->op,n);
}

void printAop(ArithmeticOperator op, int n){
    if(op == AOP_PLUS){
        print("+", n);
    }
    else if(op == AOP_MINUS){
        print("-", n);
    }
    else if(op == AOP_DIV){
        print("/", n);
    }
    else {
        print("*", n);
    }
}

void arithmeticOperator(Ast_ArithmeticOperator* t, int n){
    print("Ast_ArithmeticOperator:", n);
}

void moreExpansion(Ast_MoreExpansion* t, int n){
    print("Ast_MoreExpansion:", n);
}
