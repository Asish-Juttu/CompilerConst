#include "parser.h"
#include "ast.h"
#include "ast_def.h"
#include "astPrint.h"

int main(){
    setbuf(stdout, NULL);
    ParseTree pt = parse("testcases/s1.txt");
    handleParseTreeElement(pt.head);
    printProgram(nodeToAst(pt.head->node_syn, program), 0);
    return 0;
}