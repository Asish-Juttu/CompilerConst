#include "parser.h"
#include "ast.h"
#include "ast_def.h"
#include "astPrint.h"

#include "symbolTable.h"

int main(){
    setbuf(stdout, NULL);
    ParseTree pt = parse("testcases/s4.txt");
    handleParseTreeElement(pt.head);
    ///printProgram(nodeToAst(pt.head->node_syn, program), 0);
    computeTypes();
    return 0;
}