#include "parser.h"
#include "ast.h"
#include "ast_def.h"

int main(){
    ParseTree pt = parse("testcases/s1.txt");
    handleParseTreeElement(pt.head);

    return 1;
}