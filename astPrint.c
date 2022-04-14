#include "astPrint.h"
#include <stdio.h>
void printTabs(int n){
    for(int i = 0; i < n; i++) printf("\t");
}

void print(char* str, int n){
    printTabs(n);
    printf("%s ", str);
}

void printFloat(float f, int n){
    printTabs(n);
    printf("%f", f);
}

void printInt(int i, int n){
    printTabs(n);
    printf("%d", i);
}

void printDataType(Datatype dt, int n){
    char* str;

    if(dt == DT_PRIMITIVE){
        str = "DATATYPE: PRIMITIVE";
    }
    else if(dt == DT_UNION){
        str = "DATATYPE: UNION";
    }
    else if(dt == DT_RECORD){
        str = "DATATYPE: RECORD";
    }

    print(str, n);
}

void printTypeDefinition(Ast_TypeDefinition* t, int n){
    print("Ast_TypeDefinition", n);
    n = n + 1;
    print(t->id1, n);
    printf("");
    print(t->id2, n);
    printf("");
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
    printf("");
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
     printf("");

}

void printFieldDefinition(Ast_FieldDefinition* t, int n){
    print("Ast_FieldDefinition",n);
    printDataType(t->fieldType, n);
    print(t->id,n);
    printf("");

}

void printDefineTypeStmt(Ast_DefineTypeStmt* t, int n){
    print("Ast_FieldDefinition",n);
    printDataType(t->datatype, n);
    print(t->idOld,n);
    printf("");
    print(t->idNew,n);
    printf("");


    

}

void printA(Ast_A* t, int n){
    print("Ast_A",n);
    printDataType(t->datatype, n);


}

void printNum(Ast_Num* t, int n){
    print("Ast_Num",n);
    print(t->val,n);
    printf("");
    

}

void printRnum(Ast_Rnum* t, int n){
    print("Ast_Rnum",n);
    n = n + 1;
    printFloat(t->val,n);
    printf("");

}

void printDeclaration(Ast_Declaration* t, int n){
    print("Ast_Declaration",n);
    printDataType(t->type, n);
    print(t->id,n);
    printf("");
    print(t->isGlobal,n);
    printf("");




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
    print("Ast_ArithmeticExpression", n);
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
    print("Ast_AssignmentStmt\n", n);
    n = n + 1;
    printSingleOrRecId(t->singleOrRecId,n);
    printf(" = ");
    printArithmeticExpression(t->expression, 0);

}

void printFunCallStmt(Ast_FunCallStmt* t, int n){
    print("Ast_FunCallStmt", n);
    n = n + 1;
    print(t->funId, n);
    printIdList(t->outputIdList, 0);
    printf("<-");
    printIdList(t->inputIdList, 0);

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
    print("Ast_LogicalOperator", n);
    n = n + 1;
    printLop(t->op,n);
}

void relationalOperator(Ast_RelationalOperator* t, int n){
    print("Ast_RelationalOperator", n);
    printLogicalOperator(t->op,n);

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
    print("Ast_ArithmeticOperator", n);
    

}

void moreExpansion(Ast_MoreExpansion* t, int n){
    print("Ast_MoreExpansion", n);

}
