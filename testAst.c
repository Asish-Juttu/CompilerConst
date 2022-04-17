#include "parser.h"
#include "ast.h"
#include "ast_def.h"
#include "astPrint.h"

#include "symbolTable.h"
#include "typeCheck.h"

int main(){
    setbuf(stdout, NULL);
    ParseTree pt = parse("testcases/s2.txt");
    handleParseTreeElement(pt.head);
    // printProgram(nodeToAst(pt.head->node_syn, program), 0);
    // All tpyes
 
    computeTypes();
    computeAllLocalType();
    LL* node = funSymbolTable.keys.head;
    for(int i = 0; i < funSymbolTable.keys.sz; i++){
        SymbolTable* t = node->kv.val.symbolTable;
        loadSymbolTable(node->kv.val.name);
        printf(" ==== FUNCTION  %s ===== \n", node->kv.val.name);
        LL* node2 = t->keys.head;
        for(int j = 0; j < t->keys.sz; j++){
            printf("Type of var %s of %s is ", node2->kv.name, node->kv.name);
            printTypeExpr(findVar(node2->kv.name)->typeExpr);
            printf("\n");
            node2 = node2->next;
        }
        node = node->next;
    }

    handleTypeExpressionProgram(nodeToAst(pt.head->node_syn, program));
    return 0;
}