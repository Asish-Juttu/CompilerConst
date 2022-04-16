#include "code_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

StringList code;
FILE* fptr;
int n = 0;

void initStringList(StringList* list){
    list->cap = 1;
    list->size = 0;
    list->code = malloc(sizeof(char*));
}

void growIfFull(StringList* list){
    if(list->cap == list->size){
        list->cap = (int)(list->cap * 1.4 + 1);
        list->code = realloc(list->code, list->cap * sizeof(char*));
    }
}

void fprintf(StringList* list, char* str){
    growIfFull(list);
    list->code[list->size] = str;
    list->size ++;
}

void genProgram(Ast_Program* program){
    //Declare global variables
    //name: resb bytes

    //add jmp to main_
    //genOtherFunc
    //add label main_
    //genMain func
}

void pushVar(SymbolVal* var){
    for(int i = 0; i < var->width / 4; i++){
        fprintf(fptr, "push word 0\n");
    }
}

void popVar(SymbolVal* var){
    int width = var->width;
    for(int i = 0; i < width / 4; i++){
        fprintf(fptr, "add esp, 4\n");
    }
}

void genStmt(Ast_Stmt* stmt){
    switch(stmt->type){
        case STMT_ASSIGN: {
            genAssStmt(assignStmt(stmt));
            break;
        }
        case STMT_COND: {
            genConditionalStmt(condStmt(stmt));
            break;
        }
        case STMT_IO:{
            
            break;
        }
        case STMT_FUN_CALL:{
            genFunCallStmt(fCallStmt(stmt));
            break;
        }
        case STMT_ITER:{
            genIterStatement(iterStmt(stmt));
            break;
        }
    }   
}

void genOtherStmts(Ast_OtherStmts* otherStmts){
    AstList* otherStmtsList = otherStmts->otherStmtList;
    for(int i = 0; i < otherStmtsList->size; i++){
        Ast_Stmt* stmt = nodeToAst(otherStmtsList->nodes[i], stmt);
        genStmt(stmt);
    }
}
void genIterStatement(Ast_IterativeStmt* istmt){
    //add label_n; increment n
    int temp = n;
    fprintf(fptr, "iterLabel%d:\n", temp);
    n++;

    //evaluate condition
    genBExp(istmt->condition);
    fprintf(fptr, "cmp eax,1\n");
    fprintf(fptr, "jne iterEndLabel%d\n", temp);
    // if false : jmp to end_label_n
    // translate body
    genOtherStmts(istmt->body);
    fprintf(fptr, "jmp iterLabel%d\n", temp);
    // jmp labeL_n
    //add end_label_n
    fprintf(fptr, "iterEndLabel%d:\n", temp);
}

void genConditionalStmt(Ast_ConditionalStmt* cstmt){
    //increment ns
    int temp = n;
    n++;

    //check condition
    //if false : jmp to else_label_n
    genBExp(cstmt->condition);
    fprintf(fptr, "cmp eax,1\n");
    fprintf(fptr, "jne ifLabel%d\n", temp);
    genOtherStmts(cstmt->body);
    fprintf(fptr, "jmp ifEndLabel%d\n", temp);
    //translate if_part_body
    //add else_label_n
    fprintf(fptr, "ifLabel%d:\n", temp);
    genOtherStmts(cstmt->body);
    fprintf(fptr, "ifEndLabel%d:\n", temp);
    //translate else_part
}


void genFunCallStmt(Ast_FunCallStmt* fCallStmt){
    AstList* inList = fCallStmt->inputIdList->idList;
    AstList* outList = fCallStmt->outputIdList->idList;

    SymbolTable* cur = currentSymbolTable();
    loadSymbolTable(fCallStmt->funId);
    for(int i = 0; i < inList->size; i++){
        Ast_Id* id = nodeToAst(inList->nodes[i], id);
        SymbolVal* symVal = findVar(id->id);
        pushVar(symVal);
    }

    for(int i = 0; i < outList->size; i++){
        Ast_Id* id = nodeToAst(outList->nodes[i], id);
        SymbolVal* symVal = findVar(id->id);
        pushVar(symVal);
    }

    fprintf("call %s\n", fCallStmt->funId);
    setCurrentSymbolTable(cur);
}
void genAExp(Ast_ArithmeticExpression* aexp){
    // if only one var - direct assignment
    // else generate post fix notation
    // eval post fix r3 = r1 op r2 by copying stack top 2 to r2 and r1 resp
    // pop r1, r2, op push r3
    // repeat until stack empty
    // mov r3 to lhs location
}

void genBExp(Ast_BooleanExpression* bexp){
    // generate post fix
    // eval post fix r3 = r1 op r2 by copying stack top 2 to r2 and r1 resp
    // pop r1, r2, op push r3
    // repeat until stack empty
    // mov r3 to lhs location
}

void genAssStmt(Ast_AssignmentStmt* aStmt){
    //compute aStmt's arithmetic exp
    //execute move
}

void printInt(SymbolVal* typeOfVal){

}

void printReal(SymbolVal* typeOfVal){
    
}

void assign(SymbolVal* to, SymbolVal* from){
    int width = to->width;
    fprintf(fptr, "push eax\n");
    fprintf(fptr, "push ecx\n");
    fprintf(fptr, "mov eax, ebx\n");
    fprintf(fptr, "mov ecx, ebx\n");
    fprintf(fptr, "sub eax, %d\n", to->offset);
    fprintf(fptr, "sub ecx, %d\n", from->offset);

    for(int i = 0; i < width; i++){
        fprintf(fptr, "mov [eax], [ecx]\n");
        fprintf(fptr, "sub eax, 1\n");
        fprintf(fptr, "sub ecx, 1\n");
    }

    fprintf(fptr, "pop ecx\n");
    fprintf(fptr, "pop eax\n");
}

void genOtherFunc(Ast_OtherFunctions* otherFuncs){
    //for each function do the following
    //take current offset as 0
    //shift symbolTable to that of func
    //push normal vars
    //translate var based on symbol table offset field
    //translate each of following otherStmt
    //compute offsets (as negative from base) of output params
    //assgin values to the offset based on return stmt

    AstList* list = otherFuncs->functionList;
    for(int i = 0; i < list->size; i++){
        Ast_Function* func = nodeToAst(list->nodes[i], function);
        AstList* ipList = func->input_par->parameterList;
        AstList* opList = func->output_par->parameterList;
        
        Ast_Stmts* stmts = func->stmts;
        AstList* declList = func->stmts->declarations;

        fprintf(fptr, "%s:\n", func->funId);
        for(int i = 0; i < declList->size; i++){
            Ast_Declaration* decl = nodeToAst(declList->nodes[i], declaration);
            SymbolVal* symVal = findVar(decl->id);
            pushVar(symVal);
        }

        genOtherStmts(func->stmts->otherStmts);

        Ast_IdList* retIdList = func->stmts->returnIds;
        AstList* idList = retIdList->idList;
        AstList* outList = func->output_par->parameterList;

        for(int i = 0; i < idList->size; i++){
            Ast_Id* id = nodeToAst(idList->nodes[i], id);
            Ast_ParameterDeclaration* outPar = nodeToAst(outList->nodes[i], parameterDeclaration);
            SymbolVal* from = findVar(id->id);
            SymbolVal* to = findVar(outPar->id);
            assign(to, from);
        }

        for(int i = 0; i < declList->size; i++){
            Ast_Declaration* decl = nodeToAst(declList->nodes[i], declaration);
            SymbolVal* symVal = findVar(decl->id);
            popVar(symVal);
        }

        fprintf(fptr, "ret\n");
    }
}


void read(Ast_Var* var){
    //for primitive_datatype - base case - do read
    if(var->varType == VAR_ID){
        Ast_SingleOrRecId* singleRecId = varVar(var);
        SymbolVal* typeOfVar = findType(singleRecId);

        if(typeOfVar->typeName == NULL) {// is primitive
           
        }
    }

    //else call read on all vars in record
    else {
        // MUST CHECK IN SEM ANAL
    }
}

void write(Ast_Var* var){
    //for primitve_datatype - base case - do write
    //else if variant - call write on all except tag
    //else call write on all
}

void genIoStmt(Ast_IoStmt* iostmt){
    //for read
        //call read
    
    if(iostmt->ioType == IO_READ){
        fprintf(fptr, ";io read\n");
        read(iostmt->var);
    }
    //for write
        //call write
    else if(iostmt->ioType == IO_WRITE){

    }
}

list* postFix(Ast_ArithmeticExpression* aexp, list* head){
    list* temp = head;
    if(aexp->lefType==AEXP_VAR){
        temp->next = (list*)malloc(sizeof(list));
        temp = temp->next;
        temp->name = aexp->left.var;
        temp->t = LL_VAL;
        temp->next = NULL;
    }
    else if(aexp->lefType==AEXP_EXP){
        temp->next = postFix(aexp->left.exp,temp);
        while(temp->next!=NULL) temp = temp->next;
    }
    if(aexp->rightType==AEXP_VAR){
        temp->next = (list*)malloc(sizeof(list));
        temp = temp->next;
        temp->name = aexp->right.var;
        temp->t = LL_VAL;
        temp->next = NULL;
    }
    else if(aexp->rightType==AEXP_EXP){
        temp->next = postFix(aexp->right.exp,temp);
        while(temp->next!=NULL) temp = temp->next;
    }
    temp->next = (list*)malloc(sizeof(list));
    temp = temp->next;
    temp->op = aexp->op;
    temp->t = LL_OP;
    temp->next = NULL;
    return head;
}