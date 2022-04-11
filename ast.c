#include "ast.h"
#include <stdlib.h>

#define declareAstNode(VARNAME, ENUM,  STRUCT, FIELD) AstNode* VARNAME = makeEmptyAstNode();\
VARNAME->type = ENUM;\
VARNAME->node.FIELD = malloc(sizeof(STRUCT));

#define nodeToAst(VAR, FIELD) VAR->node.FIELD

AstNode* makeEmptyAstNode(){
    AstNode* astNode = malloc(sizeof(AstNode));
    memset(&astNode->node, 0, sizeof(AstNodeUnion));
    memset(&astNode->type, 0, sizeof(AstNodeType));
    return astNode;
}

void handleParseTreeElement(ParseTreeElement* ptElement){
    if(!ptElement->elem.isTerminal){
        switch(ptElement->elem.symbol){
            case PROGRAM : {
                ParseTreeElement* prog = ptElement;
                ParseTreeElement* othFun = &ptElement->children[0];
                ParseTreeElement* mainFun = &ptElement->children[1];
                
                declareAstNode(nodeOtherFunctions, AST_OTHERFUNCTIONS,  Ast_OtherFunctions, otherFunctions);
                nodeToAst(nodeOtherFunctions, otherFunctions)->functionList = createAstList();
                othFun->node_inh = nodeOtherFunctions;

                handleParseTreeElement(othFun);
                handleParseTreeElement(mainFun);

                declareAstNode(nodeProgram, AST_PROGRAM, Ast_Program, program);
                nodeToAst(nodeProgram, program)->mainFunction = 
                                                nodeToAst(mainFun->node_syn, mainFunction);
                
                nodeToAst(nodeProgram, program)->otherFunctions = 
                                                nodeToAst(othFun->node_syn, otherFunctions);
                
                ptElement->node_syn = nodeProgram;
                break;
            }

            case MAIN_FUNCTION : {
                ParseTreeElement* mainFunc = ptElement;
                ParseTreeElement* statements = &ptElement->children[1];

                handleParseTreeElement(statements);

                declareAstNode(nodeMainFunction, AST_MAIN, Ast_Main, mainFunction);
                nodeToAst(nodeMainFunction, mainFunction)->stmts = 
                                                nodeToAst(statements->node_syn, stmts);
                
                mainFunc->node_syn = nodeMainFunction;
                break;
            }

            case OTHER_FUNCTIONS : {
                ParseTreeElement* otherFunc = ptElement;
                if(ptElement->ruleNo == 0){
                    ParseTreeElement* func = &ptElement->children[0];
                    ParseTreeElement* otherFunc1 = &ptElement->children[1];

                    func->node_inh = ptElement->node_inh;
                    handleParseTreeElement(func);

                    otherFunc1->node_inh = func->node_syn;
                    handleParseTreeElement(otherFunc1);

                    otherFunc->node_syn = otherFunc1->node_syn;
                }
                else if (ptElement->ruleNo == 1){
                    otherFunc->node_syn = ptElement->node_inh;
                }
                break;
            }

            case FUNCTION : {
                break;
            }
        }
    }
    else{

    }
}
