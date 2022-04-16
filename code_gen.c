#include "code_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void append(StringList* list, char* str){
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

void genOtherFunc(Ast_OtherFunctions* otherFuncs){
    //for each function do the following
    //take current offset as 0
    //shift symbolTable to that of func
    //push normal vars
    //translate var based on symbol table offset field
    //translate each of following otherStmt
    //compute offsets (as negative from base) of output params
    //assgin values to the offset based on return stmt
}

void genIterStatement(Ast_IterativeStmt* istmt){
    //add label_n; increment n
    //evaluate condition
    // if false : jmp to end_label_n
    // translate body
    // jmp labeL_n
    //add end_label_n
}

void genConditionalStmt(Ast_ConditionalStmt* cstmt){
    //increment n
    //check condition
    //if false : jmp to else_label_n
    //translate if_part_body
    //add else_label_n
    //translate else_part
}

void read(Ast_Var* var){
    //for primitive_datatype - base case - do read
    //else call read on all vars in record
}

void write(Ast_Var* var){
    //for primitve_datatype - base case - do write
    //else if variant - call write on all except tag
    //else call write on all
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
void genIoStmt(Ast_IoStmt* iostmt){
    //for read
        //call read

    //for write
        //call write
}