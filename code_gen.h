#include "ast.h"
#include "ast_def.h"

typedef struct{
    char** code;
    int size;
    int cap;
} StringList;

void growIfFull(StringList* list);
void append(StringList* list, char* str);

void genProgram(Ast_Program* program);
