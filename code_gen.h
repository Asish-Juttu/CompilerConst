#include "ast.h"
#include "ast_def.h"

typedef struct{
    char** code;
    int size;
    int cap;
} StringList;

typedef enum {
    LL_VAL, LL_OP
} llType;

typedef struct ll{
    Ast_Var* name;
    ArithmeticOperator op;
    llType t;
    list* next;
} list;

void growIfFull(StringList* list);
void append(StringList* list, char* str);

void genProgram(Ast_Program* program);
list* postFix(Ast_ArithmeticExpression* aexp, list* head);
