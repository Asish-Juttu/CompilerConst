#include "parser.h"
#include "ast.h"
#include "ast_def.h"
#include "astPrint.h"

#include "symbolTable.h"

int main(){
    setbuf(stdout, NULL);
    ParseTree pt = parse("testcases/s2.txt");
    handleParseTreeElement(pt.head);
    ///printProgram(nodeToAst(pt.head->node_syn, program), 0);
    // All tpyes
    printf("\nTYPEDEFF\n");
    LL* node = typeDefSymbolTable.keys.head;
    for(int i = 0; i < typeDefSymbolTable.keys.sz; i++){
        printf("%s\n", node->kv.name);
        node = node->next;
    }
    computeTypes();
    return 0;
}