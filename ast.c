#include "ast.h"
#include <stdlib.h>
#include "type.h"
#include "token.h"

AstNode *makeEmptyAstNode()
{
    AstNode *astNode = malloc(sizeof(AstNode));
    memset(&astNode->node, 0, sizeof(AstNodeUnion));
    memset(&astNode->type, 0, sizeof(AstNodeType));
    return astNode;
}

float strToReal(char* str)
{
    return strtof(str, NULL);
}

int strToInt(char* str)
{   
    return strtol(str, NULL, 10);
}

void handleParseTreeElement(ParseTreeElement *ptElement)
{
    if(ptElement == NULL){
        printf("PTELEMENT IS NULL!!");
        return;
    }
    printf("Handling ParseTreeElement of type %s - %s (Rule no = %d)\n", 
        ptElement->elem.isTerminal ? "TERM" : "NON TERM", nonTermToStr(ptElement->elem.symbol), ptElement->ruleNo);
    if (!ptElement->elem.isTerminal)
    {
        switch (ptElement->elem.symbol)
        {
            case PROGRAM:
            {
                ParseTreeElement *prog = ptElement;
                ParseTreeElement *othFun = &ptElement->children[0];
                ParseTreeElement *mainFun = &ptElement->children[1];

                declareAstNode(nodeOtherFunctions, AST_OTHERFUNCTIONS, Ast_OtherFunctions, otherFunctions);
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

            case MAIN_FUNCTION:
            {
                ParseTreeElement *mainFunc = ptElement;
                ParseTreeElement *statements = &ptElement->children[1];

                handleParseTreeElement(statements);

                declareAstNode(nodeMainFunction, AST_MAIN, Ast_Main, mainFunction);
                nodeToAst(nodeMainFunction, mainFunction)->stmts =
                    nodeToAst(statements->node_syn, stmts);

                mainFunc->node_syn = nodeMainFunction;
                break;
            }

            case OTHER_FUNCTIONS:
            {
                ParseTreeElement *otherFunc = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *func = &ptElement->children[0];
                    ParseTreeElement *otherFunc1 = &ptElement->children[1];

                    func->node_inh = ptElement->node_inh;
                    handleParseTreeElement(func);

                    otherFunc1->node_inh = func->node_syn;
                    handleParseTreeElement(otherFunc1);

                    otherFunc->node_syn = otherFunc1->node_syn;
                }

                else if (ptElement->ruleNo == 1)
                {
                    otherFunc->node_syn = ptElement->node_inh;
                }
                break;
            }

            case FUNCTION:
            {
                ParseTreeElement *func = ptElement;
                TokenInfo tkFunId = func->children[0].tinfo;
                ParseTreeElement *input_par = &ptElement->children[1];
                ParseTreeElement *output_par = &ptElement->children[2];
                ParseTreeElement *statements = &ptElement->children[4];
                handleParseTreeElement(input_par);
                handleParseTreeElement(output_par);
                handleParseTreeElement(statements);
                declareAstNode(nodeFunction, AST_FUNCTION, Ast_Function, function);

                nodeToAst(nodeFunction, function)->funId = tkFunId.lexeme;
                nodeToAst(nodeFunction, function)->input_par = nodeToAst(input_par->node_syn, parameterList);
                nodeToAst(nodeFunction, function)->output_par = nodeToAst(output_par->node_syn, parameterList);
                nodeToAst(nodeFunction, function)->stmts = nodeToAst(statements->node_syn, stmts);

                insertTo(nodeToAst(func->node_inh, otherFunctions)->functionList, nodeFunction);
                func->node_syn = func->node_inh;
                break;
            }

            case INPUT_PAR:
            {
                ParseTreeElement *input_par = ptElement;
                ParseTreeElement *parameter_list = &ptElement->children[4];
                declareAstNode(nodeParameterList, AST_PARAMETERLIST, Ast_ParameterList, parameterList);
                nodeToAst(nodeParameterList, parameterList)->parameterList = createAstList();
                parameter_list->node_inh = nodeParameterList;
                handleParseTreeElement(parameter_list);
                input_par->node_syn = parameter_list->node_syn;
                break;
            }

            case OUTPUT_PAR:
            {
                ParseTreeElement *output_par = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *parameter_list = &ptElement->children[4];
                    declareAstNode(nodeParameterList, AST_PARAMETERLIST, Ast_ParameterList, parameterList);
                    nodeToAst(nodeParameterList, parameterList)->parameterList = createAstList();
                    parameter_list->node_inh = nodeParameterList;
                    handleParseTreeElement(parameter_list);
                    output_par->node_syn = parameter_list->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    output_par->node_syn = NULL;
                }
                break;
            }

            case PARAMETER_LIST:
            {
                ParseTreeElement *parameter_list = ptElement;
                ParseTreeElement *datatype = &ptElement->children[0];
                TokenInfo tkId = ptElement->children[1].tinfo;
                ParseTreeElement *remaining_list = &ptElement->children[2];
                handleParseTreeElement(datatype);
                // ADDTYPE TK_ID

                declareAstNode(nodeParameterDecl, AST_PARAMETERDECL, Ast_ParameterDeclaration, parameterDeclaration);

                nodeToAst(nodeParameterDecl, parameterDeclaration)->datatype = 
                    nodeToAst(datatype->node_syn, datatype);
                nodeToAst(nodeParameterDecl, parameterDeclaration)->id = 
                    tkId.lexeme;

                insertTo(nodeToAst(parameter_list->node_inh, parameterList)->parameterList, nodeParameterDecl);
                remaining_list->node_inh = parameter_list->node_inh;
                handleParseTreeElement(remaining_list);
                parameter_list->node_syn = remaining_list->node_syn;
                break;
            }

            case DATATYPE:
            {
                ParseTreeElement *datatype = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *primitive_datatype = &ptElement->children[0];
                    handleParseTreeElement(primitive_datatype);
                    datatype->node_syn = primitive_datatype->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *constructed_datatype = &ptElement->children[0];
                    handleParseTreeElement(constructed_datatype);
                    datatype->node_syn = constructed_datatype->node_syn;
                }
                break;
            }

            case PRIMITIVE_DATATYPE:
            {
                ParseTreeElement *primitive_datatype = ptElement;
                TokenInfo tkNum = ptElement->children[0].tinfo;
                if (ptElement->ruleNo == 0)
                {
                    declareAstNode(nodeDatatype, AST_DATATYPE, Ast_Datatype, datatype);
                    nodeToAst(nodeDatatype, datatype)->typeExpr = numTypeExpression();
                    nodeToAst(nodeDatatype, datatype)->datatype = DT_NUM;
                    nodeToAst(nodeDatatype, datatype)->name = NULL;
                    primitive_datatype->node_syn = nodeDatatype;
                }
                else if (ptElement->ruleNo == 1)
                {
                    declareAstNode(nodeDatatype, AST_DATATYPE, Ast_Datatype, datatype);
                    nodeToAst(nodeDatatype, datatype)->typeExpr = rnumTypeExpression();
                    nodeToAst(nodeDatatype, datatype)->datatype = DT_RNUM;
                    nodeToAst(nodeDatatype, datatype)->name = NULL;
                    primitive_datatype->node_syn = nodeDatatype;
                }
                break;
            }

            case CONSTRUCTED_DATATYPE:
            {
                ParseTreeElement *constructed_datatype = ptElement;
                TokenInfo tkRuid;
                declareAstNode(nodeDatatype, AST_DATATYPE, Ast_Datatype, datatype);
                if (ptElement->ruleNo == 0)
                {
                    tkRuid = ptElement->children[1].tinfo;
                    nodeToAst(nodeDatatype, datatype)->name = tkRuid.lexeme;
                    nodeToAst(nodeDatatype, datatype)->datatype = DT_RECORD;
                    nodeToAst(nodeDatatype, datatype)->typeExpr = recordTypeExpression();
                }
                else if (ptElement->ruleNo == 1)
                {
                    tkRuid = ptElement->children[1].tinfo;
                    nodeToAst(nodeDatatype, datatype)->name = tkRuid.lexeme;
                    nodeToAst(nodeDatatype, datatype)->datatype = DT_UNION;
                    nodeToAst(nodeDatatype, datatype)->typeExpr = unionTypeExpression();
                }
                else if (ptElement->ruleNo == 2)
                {
                    tkRuid = ptElement->children[0].tinfo;
                }

                nodeToAst(nodeDatatype, datatype)->name = tkRuid.lexeme;
                constructed_datatype->node_syn = nodeDatatype;
                break;
            }

            case REMAINING_LIST:
            {
                ParseTreeElement *remaining_list = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *parameter_list = &ptElement->children[1];
                    parameter_list->node_inh = remaining_list->node_inh;
                    handleParseTreeElement(parameter_list);
                    remaining_list->node_syn = parameter_list->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    remaining_list->node_syn = remaining_list->node_inh;
                }
                break;
            }

            case STMTS:
            {
                ParseTreeElement *stmts = ptElement;
                ParseTreeElement *type_definitions = &ptElement->children[0];
                ParseTreeElement *declarations = &ptElement->children[1];
                ParseTreeElement *other_stmts = &ptElement->children[2];
                ParseTreeElement *return_stmt = &ptElement->children[3];

                declareAstNode(nodeTypeDefinitions, AST_TYPEDEFINITIONS, Ast_TypeDefinitions, typeDefinitions);
                nodeToAst(nodeTypeDefinitions, typeDefinitions)->typeDefintionList = createAstList();
                type_definitions->node_inh = nodeTypeDefinitions;
                handleParseTreeElement(type_definitions);
        
                declareAstNode(nodeDecls, AST_DECLARATIONS, Ast_Declarations, declarations);
                nodeToAst(nodeDecls, declarations)->declarationList = createAstList();
                declarations->node_inh = nodeDecls;
                handleParseTreeElement(declarations);

                declareAstNode(nodeOtherStmts, AST_OTHERSTMTS, Ast_OtherStmts, otherStmts);
                nodeToAst(nodeOtherStmts, otherStmts)->otherStmtList = createAstList();
                other_stmts->node_inh = nodeOtherStmts;
                handleParseTreeElement(other_stmts);

                handleParseTreeElement(return_stmt);

                declareAstNode(nodeStmts, AST_STMTS, Ast_Stmts, stmts);
                nodeToAst(nodeStmts, stmts)->typeDefinitions = nodeToAst(type_definitions->node_syn, typeDefinitions);
                nodeToAst(nodeStmts, stmts)->declarations = nodeToAst(declarations->node_syn, declarations);
                nodeToAst(nodeStmts, stmts)->otherStmts = nodeToAst(other_stmts->node_syn, otherStmts);
                nodeToAst(nodeStmts, stmts)->returnIds = nodeToAst(return_stmt->node_syn, idList);
                printf("HI\n");
                stmts->node_syn = nodeStmts;
                break;
            }

            case TYPE_DEFINITIONS:
            {
                ParseTreeElement *type_def = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *actual_or_redef = &ptElement->children[0];
                    ParseTreeElement *type_def1 = &ptElement->children[1];
                    actual_or_redef->node_inh = type_def->node_inh;
                    handleParseTreeElement(actual_or_redef);
                    type_def1->node_inh = actual_or_redef->node_syn;
                    handleParseTreeElement(type_def1);
                    type_def->node_syn = type_def1->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    type_def->node_syn = type_def->node_inh;
                }
                else{
                    printf("Error Rule doesnt match!!!\n");
                }
                break;
            }

            case TYPE_DEFINITION:
            {
                ParseTreeElement *type_def = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *field_def = &ptElement->children[2];
                    TokenInfo tkRuid = type_def->children[1].tinfo;

                    handleParseTreeElement(field_def);

                    declareAstNode(nodeTypeDefinition, AST_TYPEDEFINITION, Ast_TypeDefinition, typeDefinition);
                    nodeToAst(nodeTypeDefinition, typeDefinition)->type = TDEF;
              
                    tdef(nodeToAst(nodeTypeDefinition, typeDefinition))->fieldDefinitions = nodeToAst(field_def->node_syn, fieldDefinitions);
                    tdef(nodeToAst(nodeTypeDefinition, typeDefinition))->id = tkRuid.lexeme;
                    tdef(nodeToAst(nodeTypeDefinition, typeDefinition))->datatype = DT_RECORD;

                    insertTo(nodeToAst(type_def->node_inh, typeDefinitions)->typeDefintionList,
                            nodeTypeDefinition);

                    type_def->node_syn = type_def->node_inh;
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *field_def = &ptElement->children[2];
                    TokenInfo tkRuid = type_def->children[1].tinfo;

                    handleParseTreeElement(field_def);

                    declareAstNode(nodeTypeDefinition, AST_TYPEDEFINITION, Ast_TypeDefinition, typeDefinition);
                    nodeToAst(nodeTypeDefinition, typeDefinition)->type = TDEF;
              
                    tdef(nodeToAst(nodeTypeDefinition, typeDefinition))->fieldDefinitions = nodeToAst(field_def->node_syn, fieldDefinitions);
                    tdef(nodeToAst(nodeTypeDefinition, typeDefinition))->id = tkRuid.lexeme;
                    tdef(nodeToAst(nodeTypeDefinition, typeDefinition))->datatype = DT_UNION;

                    insertTo(nodeToAst(type_def->node_inh, typeDefinitions)->typeDefintionList,
                            nodeTypeDefinition);

                    type_def->node_syn = type_def->node_inh;
                }
                break;
            }

            case FIELD_DEFINITIONS:
            {
                ParseTreeElement *field_defs = ptElement;
                ParseTreeElement *field_def1 = &ptElement->children[0];
                ParseTreeElement *field_def2 = &ptElement->children[1];
                ParseTreeElement *more_fields = &ptElement->children[2];
                handleParseTreeElement(field_def1);
                handleParseTreeElement(field_def2);
                declareAstNode(nodeFieldDefinitions, AST_FIELDDEFINITIONS, Ast_FieldDefinitions, fieldDefinitions);

                nodeToAst(nodeFieldDefinitions, fieldDefinitions)->fieldDefinitionList = createAstList();
                insertTo(nodeToAst(nodeFieldDefinitions, fieldDefinitions)->fieldDefinitionList,
                        field_def1->node_syn);
                insertTo(nodeToAst(nodeFieldDefinitions, fieldDefinitions)->fieldDefinitionList,
                        field_def2->node_syn);

                more_fields->node_inh = nodeFieldDefinitions;
                handleParseTreeElement(more_fields);
                field_defs->node_syn = more_fields->node_syn;

                break;
            }

            case FIELD_DEFINITION:
            {
                ParseTreeElement *field_def = ptElement;
                ParseTreeElement *field_type = &ptElement->children[1];
                handleParseTreeElement(field_type);
                declareAstNode(nodeFieldDefinition, AST_FIELDDEFINITION, Ast_FieldDefinition, fieldDefinition);
                // TK_FIELDID
                //  DATATYPE: nodeToAst(nodeFieldDefinition,fieldDefinition)->field_type = nodeToAst(field_type->node_syn,primitive_datatype);
                insertTo(nodeToAst(field_def->node_inh, fieldDefinitions)->fieldDefinitionList, nodeFieldDefinition);
                field_def->node_syn = field_def->node_inh;

                break;
            }
            case MORE_FIELDS:
            {
                ParseTreeElement *more_fields = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *field_def = &ptElement->children[0];
                    ParseTreeElement *more_fields1 = &ptElement->children[1];

                    field_def->node_inh = ptElement->node_inh;
                    handleParseTreeElement(field_def);

                    more_fields1->node_inh = field_def->node_syn;
                    handleParseTreeElement(more_fields1);

                    more_fields->node_syn = more_fields1->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    more_fields->node_syn = ptElement->node_inh;
                }
                break;
            }
            case DECLARATIONS:
            {
                ParseTreeElement *declarations = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *declaration = &ptElement->children[0];
                    ParseTreeElement *declarations1 = &ptElement->children[1];

                    declaration->node_inh = ptElement->node_inh;
                    handleParseTreeElement(declaration);

                    declarations1->node_inh = declaration->node_syn;
                    handleParseTreeElement(declarations1);

                    declarations->node_syn = declarations1->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    declarations->node_syn = ptElement->node_inh;
                }
                break;
            }
            case DECLARATION:
            {
                ParseTreeElement *declaration = ptElement;
                ParseTreeElement *datatype = &ptElement->children[1];
                ParseTreeElement *global_or_not = &ptElement->children[4];
                TokenInfo tkId = ptElement->children[3].tinfo;
                printf("Declaration id is %s \n", tkId.lexeme);
                handleParseTreeElement(datatype);
                
                handleParseTreeElement(global_or_not);
                declareAstNode(nodeDeclaration, AST_DECLARATION, Ast_Declaration, declaration);
                // DATATYPE: nodeToAst(nodeDeclaration,declaration)->type = nodeToAst(type->node_syn,datatype);

                nodeToAst(nodeDeclaration, declaration)->id = tkId.lexeme;
                nodeToAst(nodeDeclaration, declaration)->type = nodeToAst(datatype->node_syn,
                    datatype)->datatype;
                nodeToAst(nodeDeclaration, declaration)->isGlobal = nodeToAst(global_or_not->node_syn, isGlobalOrNot)->isGlobal;
                insertTo(nodeToAst(declaration->node_inh, declarations)->declarationList, nodeDeclaration);
                declaration->node_syn = declaration->node_inh;
                break;
            }
            case GLOBAL_OR_NOT:
            {

                ParseTreeElement *global_or_not = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    declareAstNode(nodeIsG, AST_ISGLOBALORNOT, Ast_IsGlobalOrNot,
                        isGlobalOrNot);
                    nodeToAst(nodeIsG, isGlobalOrNot)->isGlobal = 1;

                    global_or_not->node_syn = nodeIsG;
                }
                else if (ptElement->ruleNo == 1)
                {
                    declareAstNode(nodeIsG, AST_ISGLOBALORNOT, Ast_IsGlobalOrNot,
                        isGlobalOrNot);
                    nodeToAst(nodeIsG, isGlobalOrNot)->isGlobal = 0;

                    global_or_not->node_syn = nodeIsG;
                }
                break;
            }
            case OTHER_STMTS:
            {
                ParseTreeElement *other_stmts = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *stmt = &ptElement->children[0];
                    ParseTreeElement *other_stmts1 = &ptElement->children[1];

                    stmt->node_inh = ptElement->node_inh;
                    handleParseTreeElement(stmt);

                    other_stmts1->node_inh = stmt->node_syn;
                    handleParseTreeElement(other_stmts1);

                    other_stmts->node_syn = other_stmts1->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    other_stmts->node_syn = ptElement->node_inh;
                }
                break;
            }
            case STMT:
            {
                ParseTreeElement *stmt = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *assignment_stmt = &ptElement->children[0];
                    handleParseTreeElement(assignment_stmt);

                    declareAstNode(nodeStmt, AST_STMT, Ast_Stmt, stmt);
                    nodeToAst(nodeStmt, stmt)->type = STMT_ASSIGN;
                    assignStmt(nodeToAst(nodeStmt, stmt)) = nodeToAst(assignment_stmt->node_syn, assignmentStmt);
                    
                    insertTo(nodeToAst(stmt->node_inh, otherStmts)->otherStmtList, nodeStmt);
                    stmt->node_syn = stmt->node_inh;
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *iterative_stmt = &ptElement->children[0];
                    handleParseTreeElement(iterative_stmt);

                    declareAstNode(nodeStmt, AST_STMT, Ast_Stmt, stmt);
                    nodeToAst(nodeStmt, stmt)->type = STMT_ITER;
                    iterStmt(nodeToAst(nodeStmt, stmt)) = nodeToAst(iterative_stmt->node_syn, iterativeStmt);

                    insertTo(nodeToAst(stmt->node_inh, otherStmts)->otherStmtList, nodeStmt);
                    stmt->node_syn = stmt->node_inh;
                }
                else if (ptElement->ruleNo == 2)
                {
                    ParseTreeElement *conditional_stmt = &ptElement->children[0];
                    handleParseTreeElement(conditional_stmt);
                    declareAstNode(nodeStmt, AST_STMT, Ast_Stmt, stmt);
                    nodeToAst(nodeStmt, stmt)->type = STMT_COND;
                    condStmt(nodeToAst(nodeStmt, stmt)) = nodeToAst(conditional_stmt->node_syn, conditionalStmt);

                    insertTo(nodeToAst(stmt->node_inh, otherStmts)->otherStmtList, nodeStmt);
                    stmt->node_syn = stmt->node_inh;
                }

                else if (ptElement->ruleNo == 3)
                {
                    ParseTreeElement *io_stmt = &ptElement->children[0];
                    handleParseTreeElement(io_stmt);
                    declareAstNode(nodeStmt, AST_STMT, Ast_Stmt, stmt);
                    nodeToAst(nodeStmt, stmt)->type = STMT_IO;
                    ipopStmt(nodeToAst(nodeStmt, stmt)) = nodeToAst(io_stmt->node_syn, ioStmt);

                    insertTo(nodeToAst(stmt->node_inh, otherStmts)->otherStmtList, nodeStmt);
                    stmt->node_syn = stmt->node_inh;
                }
                else if (ptElement->ruleNo == 4)
                {
                    ParseTreeElement *fun_call_stmt = &ptElement->children[0];
                    handleParseTreeElement(fun_call_stmt);
                    declareAstNode(nodeStmt, AST_STMT, Ast_Stmt, stmt);
                    nodeToAst(nodeStmt, stmt)->type = STMT_FUN_CALL;
                    fCallStmt(nodeToAst(nodeStmt, stmt)) = nodeToAst(fun_call_stmt->node_syn, funCallStmt);

                    insertTo(nodeToAst(stmt->node_inh, otherStmts)->otherStmtList, nodeStmt);
                    stmt->node_syn = stmt->node_inh;
                }
                break;
            }
            case ASSIGNMENT_STMT:
            {
                ParseTreeElement *assignment_stmt = ptElement;
                ParseTreeElement *single_or_rec_id = &ptElement->children[0];
                ParseTreeElement *arithmetic_expression = &ptElement->children[2];
                handleParseTreeElement(single_or_rec_id);
                handleParseTreeElement(arithmetic_expression);
                declareAstNode(nodeAssignmentStmt, AST_ASSIGNMENTSTMT, Ast_AssignmentStmt, assignmentStmt);

                nodeToAst(nodeAssignmentStmt, assignmentStmt)->singleOrRecId = 
                    nodeToAst(single_or_rec_id->node_syn, singleOrRecId);
                nodeToAst(nodeAssignmentStmt, assignmentStmt)->expression = 
                    nodeToAst(arithmetic_expression->node_syn, arithmeticExpression);

                assignment_stmt->node_syn = nodeAssignmentStmt;
                break;
            }
            case SINGLE_OR_REC_ID:
            {
                ParseTreeElement *single_or_rec_id = ptElement;
                ParseTreeElement *option_single_constructed = &ptElement->children[1];
                TokenInfo tkId = ptElement->children[0].tinfo;

                handleParseTreeElement(option_single_constructed);
                // TK_ID
                declareAstNode(nodeSingleOrRecId, AST_SINGLEORRECID, Ast_SingleOrRecId, singleOrRecId);
                nodeToAst(nodeSingleOrRecId, singleOrRecId)->id = tkId.lexeme;
                nodeToAst(nodeSingleOrRecId, singleOrRecId)->fieldNameList = nodeToAst(option_single_constructed->node_syn, 
                    moreExp)->oneExpansionList;

                single_or_rec_id->node_syn = nodeSingleOrRecId;
                break;
            }
            case FUN_CALL_STMT:
            {
                ParseTreeElement *fun_call_stmt = ptElement;
                ParseTreeElement *output_parameters = &ptElement->children[0];
                ParseTreeElement *input_parameters = &ptElement->children[5];
                handleParseTreeElement(output_parameters);
                handleParseTreeElement(input_parameters);

                // TK_FUNID
                declareAstNode(nodeFunCallStmt, AST_FUNCALLSTMT, Ast_FunCallStmt, funCallStmt);
                nodeToAst(nodeFunCallStmt, funCallStmt)->outputIdList = nodeToAst(output_parameters->node_syn, idList);
                nodeToAst(nodeFunCallStmt, funCallStmt)->inputIdList = nodeToAst(input_parameters->node_syn, idList);
                fun_call_stmt->node_syn = nodeFunCallStmt;
                break;
            }
            case OUTPUT_PARAMETERS:
            {
                ParseTreeElement *output_parameters = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *id_list = &ptElement->children[1];

                    declareAstNode(nodeIdList, AST_IDLIST, Ast_IdList, idList);
                    nodeToAst(nodeIdList, idList)->idList = createAstList();
                    id_list->node_inh = nodeIdList;

                    handleParseTreeElement(id_list);

                    id_list->node_inh = nodeIdList;
                    output_parameters->node_syn = id_list->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    output_parameters->node_syn = NULL;
                }
                break;
            }

            case INPUT_PARAMETERS:
            {
                ParseTreeElement *input_parameters = ptElement;
                ParseTreeElement *id_list = &ptElement->children[1];

                declareAstNode(nodeIdList, AST_IDLIST, Ast_IdList, idList);
                nodeToAst(nodeIdList, idList)->idList = createAstList();
                id_list->node_inh = nodeIdList;
                handleParseTreeElement(id_list);
                input_parameters->node_syn = id_list->node_syn;
                break;
            }
            case ITERATIVE_STMT:
            {
                ParseTreeElement *iterative_stmt = ptElement;
                ParseTreeElement *boolean_expression = &ptElement->children[2];
                ParseTreeElement *stmt = &ptElement->children[4];
                ParseTreeElement *other_stmts = &ptElement->children[5];
                handleParseTreeElement(boolean_expression);
                declareAstNode(nodeOtherStmts, AST_OTHERSTMTS, Ast_OtherStmts, otherStmts);
                nodeToAst(nodeOtherStmts, otherStmts)->otherStmtList = createAstList();
                stmt->node_inh = nodeOtherStmts;
                handleParseTreeElement(stmt);
                other_stmts->node_inh = stmt->node_syn;
                handleParseTreeElement(other_stmts);

                declareAstNode(nodeIterativeStmt, AST_ITERATIVESTMT, Ast_IterativeStmt, iterativeStmt);

                nodeToAst(nodeIterativeStmt, iterativeStmt)->condition = 
                    nodeToAst(boolean_expression->node_syn, booleanExpression);

                nodeToAst(nodeIterativeStmt, iterativeStmt)->body = nodeToAst(nodeOtherStmts, otherStmts);

                iterative_stmt->node_syn = nodeIterativeStmt;
                break;
            }

            case CONDITIONAL_STMT:
            {
                ParseTreeElement *condStmt = ptElement;
                ParseTreeElement *boolExp = &ptElement->children[2];
                ParseTreeElement *stmt = &ptElement->children[4];
                ParseTreeElement *other_stmts = &ptElement->children[5];
                ParseTreeElement *else_part = &ptElement->children[6];
                handleParseTreeElement(boolExp);
                declareAstNode(nodeOtherStmts, AST_OTHERSTMTS, Ast_OtherStmts, otherStmts);
                nodeToAst(nodeOtherStmts, otherStmts)->otherStmtList = createAstList();
                stmt->node_inh = nodeOtherStmts;
                handleParseTreeElement(stmt);
                other_stmts->node_inh = stmt->node_syn;
                handleParseTreeElement(other_stmts);
                handleParseTreeElement(else_part);
                declareAstNode(nodeConditionalStmt, AST_CONDITIONALSTMT,
                            Ast_ConditionalStmt, conditionalStmt);

                nodeToAst(nodeConditionalStmt, conditionalStmt)->body = nodeOtherStmts;
                nodeToAst(nodeConditionalStmt, conditionalStmt)->condition = nodeToAst(boolExp->node_syn, booleanExpression);

                nodeToAst(nodeConditionalStmt, conditionalStmt)->elsePart = nodeToAst(else_part->node_syn, elsePart);

                condStmt->node_syn = nodeConditionalStmt;
                break;
            }
            
            case IO_STMT:
            {
                
                ParseTreeElement* ioStmt = ptElement;
                ParseTreeElement* var = &ptElement->children[2];
                handleParseTreeElement(var);
                

                if(ioStmt->ruleNo == 0){
                    declareAstNode(nodeIo, AST_IOSTMT, Ast_IoStmt,
                        ioStmt);
                    nodeToAst(nodeIo, ioStmt)->ioType = IO_READ;
                    nodeToAst(nodeIo, ioStmt)->var = nodeToAst(var->node_syn, var);
                    ioStmt->node_syn = nodeIo;
                }
                else if(ioStmt->ruleNo == 1){
                    declareAstNode(nodeIo, AST_IOSTMT, Ast_IoStmt,
                        ioStmt);
                    nodeToAst(nodeIo, ioStmt)->ioType = IO_WRITE;
                    nodeToAst(nodeIo, ioStmt)->var = nodeToAst(var->node_syn, var);
                    ioStmt->node_syn = nodeIo;
                }
                break;
            }
            case BOOLEAN_EXPRESSION:
            {
                ParseTreeElement *boolean_expression = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *tk_op1 = &ptElement->children[0];
                    ParseTreeElement *boolean_expression1 = &ptElement->children[1];
                    ParseTreeElement *tk_cl1 = &ptElement->children[2];
                    ParseTreeElement *logical_op = &ptElement->children[2];
                    ParseTreeElement *tk_op2 = &ptElement->children[2];
                    ParseTreeElement *boolean_expression2 = &ptElement->children[2];
                    ParseTreeElement *tk_cl2 = &ptElement->children[2];
                    handleParseTreeElement(boolean_expression1);
                    handleParseTreeElement(boolean_expression2);
                    handleParseTreeElement(logical_op);
                    declareAstNode(nodeBooleanExpression, AST_BOOLEXP, Ast_BooleanExpression, booleanExpression);

                    nodeBooleanExpression->node.booleanExpression->bexpType = BEXP_BOOL_OP;
                    nodeBooleanExpression->node.booleanExpression->bexp.boolOp = (BoolOperation *)malloc(sizeof(BoolOperation));

                    boolOpBexp(nodeToAst(nodeBooleanExpression, booleanExpression))->op =
                        nodeToAst(logical_op->node_syn, logicalOp)->op;

                    Ast_BooleanExpression* left = boolean_expression1->node_syn;
                    Ast_BooleanExpression* right = boolean_expression2->node_syn;

                    if(left->bexpType == BEXP_BOOL_OP){
                        boolOpBexp(nodeToAst(nodeBooleanExpression, booleanExpression))->left = 
                            boolOpBexp(left);
                    }
                    else if(left->bexpType == BEXP_VAR_COMP){
                        boolOpBexp(nodeToAst(nodeBooleanExpression, booleanExpression))->left =
                            varCompBexp(left);
                    }

                    if(right->bexpType == BEXP_BOOL_OP){
                        boolOpBexp(nodeToAst(nodeBooleanExpression, booleanExpression))->right = 
                            boolOpBexp(right);
                    }
                    else if(right->bexpType == BEXP_VAR_COMP){
                        boolOpBexp(nodeToAst(nodeBooleanExpression, booleanExpression))->right =
                            varCompBexp(right);
                    }
                    
                    ptElement->node_syn = nodeBooleanExpression;
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *var1 = &ptElement->children[0];
                    ParseTreeElement *relop = &ptElement->children[1];
                    ParseTreeElement *var2 = &ptElement->children[2];
                    handleParseTreeElement(var1);
                    handleParseTreeElement(var2);
                    handleParseTreeElement(relop);
                    declareAstNode(nodeBooleanExpression, AST_BOOLEXP, Ast_BooleanExpression, booleanExpression);

                    nodeBooleanExpression->node.booleanExpression->bexpType = BEXP_VAR_COMP;
                    nodeBooleanExpression->node.booleanExpression->bexp.varComp = (VarComparison *)malloc(sizeof(VarComparison));
                    varCompBexp(nodeToAst(nodeBooleanExpression, booleanExpression))->op =
                        nodeToAst(relop->node_syn, relationalOp)->op;
                    nodeBooleanExpression->node.booleanExpression->bexp.varComp->left = nodeToAst(var1->node_syn, var);
                    nodeBooleanExpression->node.booleanExpression->bexp.varComp->right = nodeToAst(var2->node_syn, var);

                    ptElement->node_syn = nodeBooleanExpression;
                }
                else if (ptElement->ruleNo == 2)
                {
                    ParseTreeElement *tk_not = &ptElement->children[0];
                    ParseTreeElement *boolean = &ptElement->children[2];
                    handleParseTreeElement(boolean);
                    handleParseTreeElement(tk_not);
                    declareAstNode(nodeBooleanExpression, AST_BOOLEXP, Ast_BooleanExpression, booleanExpression);

                    nodeBooleanExpression->node.booleanExpression->bexpType = BEXP_BOOL_OP;
                    nodeBooleanExpression->node.booleanExpression->bexp.boolOp = (BoolOperation *)malloc(sizeof(BoolOperation));
                    nodeBooleanExpression->node.booleanExpression->bexp.boolOp->op = LOP_NOT;
                    nodeBooleanExpression->node.booleanExpression->bexp.boolOp->left = boolean->node_syn;
                    nodeBooleanExpression->node.booleanExpression->bexp.boolOp->right = NULL;

                    ptElement->node_syn = nodeBooleanExpression;
                }

                break;
            }

            case VAR:
            {
                ParseTreeElement *var = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *singleorrecId = &ptElement->children[0];
                    handleParseTreeElement(singleorrecId);

                    declareAstNode(nodeVar, AST_VAR, Ast_Var, var);
                    nodeToAst(nodeVar, var)->varType = VAR_ID;
                    varVar(nodeToAst(nodeVar, var)) = 
                        nodeToAst(singleorrecId->node_syn, singleOrRecId);  

                    var->node_syn = nodeVar;
                }
                else if (ptElement->ruleNo == 1)
                {
                    TokenInfo tkNum = ptElement->children[0].tinfo;
                    declareAstNode(nodeNum, AST_NUM, Ast_Num, num);
                    nodeToAst(nodeNum, num)->typeExpr = numTypeExpression();
                    nodeToAst(nodeNum, num)->val = strToInt(tkNum.lexeme);

                    declareAstNode(nodeVar, AST_VAR, Ast_Var, var);
                    nodeToAst(nodeVar, var)->varType = VAR_NUM;
                    numVar(nodeToAst(nodeVar, var)) = nodeToAst(nodeNum, num);
                    
                    var->node_syn = nodeVar;
                }
                else if (ptElement->ruleNo == 2)
                {
                    TokenInfo tkRnum = ptElement->children[0].tinfo;
                    declareAstNode(nodeRnum, AST_RNUM, Ast_Rnum, rnum);
                    nodeToAst(nodeRnum, rnum)->typeExpr = rnumTypeExpression();
                    nodeToAst(nodeRnum, rnum)->val = strToReal(tkRnum.lexeme);

                    declareAstNode(nodeVar, AST_VAR, Ast_Var, var);
                    nodeToAst(nodeVar, var)->varType = VAR_RNUM;
                    rnumVar(nodeToAst(nodeVar, var)) = nodeToAst(nodeRnum, rnum);

                    var->node_syn = nodeVar;
                }
                else{
                    printf("BLahh");
                    exit(0);
                }
                break;
            }

            case LOGICAL_OP:
            {
                ParseTreeElement *localOp = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *tkand = &ptElement->children[0];

                    declareAstNode(nodeLogOp, AST_LOGICALOP, Ast_LogicalOperator, logicalOp);
                    nodeToAst(nodeLogOp, logicalOp)->op = LOP_AND;

                    localOp->node_syn = nodeLogOp;
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *tkor = &ptElement->children[0];

                    declareAstNode(nodeLogOp, AST_LOGICALOP, Ast_LogicalOperator, logicalOp);
                    nodeToAst(nodeLogOp, logicalOp)->op = LOP_OR;

                    localOp->node_syn = nodeLogOp;
                }
                break;
            }

            case RELATIONAL_OP:
            {
                ParseTreeElement *relationalOP = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    TokenInfo tkLt = ptElement->children[0].tinfo;
                    declareAstNode(nodeROp, AST_RELATIONALOP, Ast_RelationalOperator, relationalOp);
                    nodeToAst(nodeROp, relationalOp)->op = ROP_Lt;
                    relationalOP->node_syn = nodeROp;
                }
                else if (ptElement->ruleNo == 1)
                {
                    TokenInfo tkLe = ptElement->children[0].tinfo;
                    declareAstNode(nodeROp, AST_RELATIONALOP, Ast_RelationalOperator, relationalOp);
                    nodeToAst(nodeROp, relationalOp)->op = ROP_Le;
                    relationalOP->node_syn = nodeROp;
                }
                else if (ptElement->ruleNo == 2)
                {
                    TokenInfo tkEq = ptElement->children[0].tinfo;
                    declareAstNode(nodeROp, AST_RELATIONALOP, Ast_RelationalOperator, relationalOp);
                    nodeToAst(nodeROp, relationalOp)->op = ROP_Eq;
                    relationalOP->node_syn = nodeROp;
                }
                else if (ptElement->ruleNo == 3)
                {
                    TokenInfo tkGt = ptElement->children[0].tinfo;
                    declareAstNode(nodeROp, AST_RELATIONALOP, Ast_RelationalOperator, relationalOp);
                    nodeToAst(nodeROp, relationalOp)->op = ROP_Gt;
                    relationalOP->node_syn = nodeROp;
                }
                else if (ptElement->ruleNo == 4)
                {
                    TokenInfo tkGe = ptElement->children[0].tinfo;
                    declareAstNode(nodeROp, AST_RELATIONALOP, Ast_RelationalOperator, relationalOp);
                    nodeToAst(nodeROp, relationalOp)->op = ROP_Ge;
                    relationalOP->node_syn = nodeROp;
                }
                else if (ptElement->ruleNo == 5)
                {
                    TokenInfo tkNe = ptElement->children[0].tinfo;
                    declareAstNode(nodeROp, AST_RELATIONALOP, Ast_RelationalOperator, relationalOp);
                    nodeToAst(nodeROp, relationalOp)->op = ROP_Ne;
                    relationalOP->node_syn = nodeROp;
                }
                break;
            }

            case RETURN_STMT:
            {
                ParseTreeElement *returnStmt = ptElement;
                ParseTreeElement *optioanlReturn = &ptElement->children[1];
                handleParseTreeElement(optioanlReturn);
                returnStmt->node_syn = optioanlReturn->node_syn;
                break;
            }

            case OPTIONAL_RETURN:
            {
                ParseTreeElement *optionalReturn = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *idList = &ptElement->children[1];
                    declareAstNode(nodeidList, AST_IDLIST, Ast_IdList, idList);
                    nodeToAst(nodeidList, idList)->idList = createAstList();
                    idList->node_inh = nodeidList;
                    handleParseTreeElement(idList);
                    optionalReturn->node_syn = idList->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    declareAstNode(nodeidList, AST_IDLIST, Ast_IdList, idList);
                    nodeToAst(nodeidList, idList)->idList = createAstList();
                    optionalReturn->node_syn = nodeidList;
                }
                break;
            }

            case DEFINE_TYPE_STATEMENT:
            {
                ParseTreeElement* dtypeStmt = ptElement;
                ParseTreeElement* a = &ptElement->children[1];

                TokenInfo tkruid = dtypeStmt->children[2].tinfo;
                TokenInfo tkruid1 = dtypeStmt->children[4].tinfo;

                handleParseTreeElement(a);
                declareAstNode(nodeTrdef, AST_TYPEDEFINITION, 
                    Ast_TypeDefinition, typeDefinition);
                nodeToAst(nodeTrdef, typeDefinition)->type = TRDEF;
                trdef(nodeToAst(nodeTrdef, typeDefinition))->from
                    = tkruid.lexeme;
                trdef(nodeToAst(nodeTrdef, typeDefinition))->from
                    = tkruid1.lexeme;
                
                dtypeStmt->node_syn = nodeTrdef;
                break;
            }
            case ID_LIST:
            {
                ParseTreeElement *idList = ptElement;
                TokenInfo tkId = ptElement->children[0].tinfo;
                ParseTreeElement *moreIds = &ptElement->children[1];

                declareAstNode(nodeId, AST_ID, Ast_Id, id);
                nodeToAst(nodeId, id)->id = tkId.lexeme;
                insertTo(nodeToAst(idList->node_inh, idList)->idList, nodeId);
                moreIds->node_inh = idList->node_inh;
                handleParseTreeElement(moreIds);
                idList->node_syn = moreIds->node_syn;
                break;
            }

            case MORE_IDS:
            {
                ParseTreeElement *moreIds = ptElement;

                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *idList = &ptElement->children[1];
                    idList->node_inh = moreIds->node_inh;
                    handleParseTreeElement(idList);
                    moreIds->node_syn = idList->node_syn;
                }

                else if (ptElement->ruleNo == 1)
                {
                    moreIds->node_syn = moreIds->node_inh;
                }
                break;
            }

            case ACTUAL_OR_REDEFINED:
            {
                ParseTreeElement *actualOrredefined = ptElement;

                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *typeDefinition = &ptElement->children[0];
                    handleParseTreeElement(typeDefinition);
                    insertTo(nodeToAst(actualOrredefined->node_inh, typeDefinitions)->typeDefintionList, nodeToAst(typeDefinition->node_syn, typeDefinition));
                    ptElement->node_syn = ptElement->node_inh;
                }

                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *defineTypeStatement = &ptElement->children[0];
                    handleParseTreeElement(defineTypeStatement);
                    insertTo(nodeToAst(actualOrredefined->node_inh, typeDefinitions)->typeDefintionList, nodeToAst(defineTypeStatement->node_syn, typeDefinition));
                    ptElement->node_syn = ptElement->node_inh;
                }
                break;
            }

            case FIELD_TYPE:
            {
                ParseTreeElement *fieldType = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *primitiveDatatype = &ptElement->children[0];
                    fieldType->node_syn = primitiveDatatype->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    TokenInfo tkRuId = ptElement->children[1].tinfo;
                    declareAstNode(nodeRuid, AST_DATATYPE, Ast_Datatype, datatype);
                    nodeToAst(nodeRuid, datatype)->name = tkRuId.lexeme;
                    fieldType->node_syn = nodeRuid;
                }
                break;
            }

            case OPTION_SINGLE_CONSTRUCTED:
            {
                ParseTreeElement *optionSingleConstructed = ptElement;

                if(optionSingleConstructed->ruleNo == 0){
                    ParseTreeElement* oneExp = &ptElement->children[0];
                    ParseTreeElement* moreExp = &ptElement->children[1];
                    declareAstNode(nodeMoreExp, AST_MOREEXPANSION, Ast_MoreExpansion,
                        moreExp);
                    nodeToAst(nodeMoreExp, moreExp)->oneExpansionList = createAstList();
                    oneExp->node_inh = nodeMoreExp;

                    handleParseTreeElement(oneExp);

                    moreExp->node_inh = oneExp->node_syn;
                    handleParseTreeElement(moreExp);

                    optionSingleConstructed->node_syn = moreExp->node_syn;
                }
                else if(optionSingleConstructed->ruleNo == 1){
                    declareAstNode(nodeMoreExp, AST_MOREEXPANSION, Ast_MoreExpansion,
                        moreExp);
                    nodeToAst(nodeMoreExp, moreExp)->oneExpansionList = createAstList();
                    optionSingleConstructed->node_syn = nodeMoreExp;
                }
                break;
            }

            case ONE_EXPANSION:
            {
                ParseTreeElement *oneExpansion = ptElement;
                // ONE_EXPANSION INSERTO
                TokenInfo tkFieldId = ptElement->children[1].tinfo;
                insertTo(nodeToAst(oneExpansion->node_inh, moreExp)->oneExpansionList,
                        tkFieldId.lexeme);
                oneExpansion->node_syn = oneExpansion->node_inh;
                break;
            }

            case MORE_EXPANSIONS:
            {
                ParseTreeElement *moreExpansions = ptElement;

                if (moreExpansions->ruleNo == 0)
                {
                    ParseTreeElement *oneExpansion = &ptElement->children[0];
                    ParseTreeElement *moreExpansion1 = &ptElement->children[1];
                    oneExpansion->node_inh = moreExpansions->node_inh;
                    handleParseTreeElement(oneExpansion);
                    moreExpansion1->node_inh = oneExpansion->node_syn;
                    handleParseTreeElement(moreExpansion1);
                    moreExpansions->node_syn = moreExpansion1->node_syn;
                }

                else if (moreExpansions->ruleNo == 1)
                {
                    moreExpansions->node_syn = moreExpansions->node_inh;
                }
                break;
            }

            case EXP_PRIME:
            {
                ParseTreeElement *expPrime = ptElement;
                if(expPrime->ruleNo == 0){
                    ParseTreeElement *lowPrOp = &expPrime->children[0];
                    ParseTreeElement *term = &expPrime->children[1];
                    ParseTreeElement* expPrime1 = &ptElement->children[2];

                    declareAstNode(nodeArithExpr, AST_ARITHMETICEXPR, Ast_ArithmeticExpression,
                                arithmeticExpression);
                                
                    handleParseTreeElement(lowPrOp);
                    nodeToAst(nodeArithExpr, arithmeticExpression)->op =
                        nodeToAst(lowPrOp->node_syn, arithmeticOp)->op;

                    handleParseTreeElement(term);

                    AstNode* left = expPrime->node_inh;
                    AstNode* right = term->node_syn;
                    Ast_ArithmeticExpression* aexp = nodeToAst(nodeArithExpr, arithmeticExpression);

                    if(left->type == AST_VAR){
                        aexp->lefType = AEXP_VAR;
                        aexp->left.var = nodeToAst(left, var);
                    }
                    else if(left->type == AST_ARITHMETICEXPR){
                        aexp->lefType = AEXP_EXP;
                        aexp_expLeft(aexp) = nodeToAst(left, arithmeticExpression);
                    }

                    if(right->type == AST_VAR){
                        aexp->rightType = AEXP_VAR;
                        aexp_varRight(aexp) = nodeToAst(right, var);
                    }
                    else if(right->type == AST_ARITHMETICEXPR){
                        aexp->rightType = AEXP_EXP;
                        aexp_expRight(aexp) = nodeToAst(right, arithmeticExpression);
                    }

                    expPrime1->node_inh = nodeArithExpr;
                    handleParseTreeElement(expPrime1);
                    expPrime->node_syn = expPrime1->node_syn;
                }

                else if(expPrime->ruleNo == 1){
                    expPrime->node_syn = expPrime->node_inh;
                }
                break;
            }

            case A:
            {
                ParseTreeElement *a = ptElement;
    
                if (ptElement->ruleNo == 0)
                {
                    declareAstNode(nodeDatatype, AST_DATATYPE, Ast_Datatype, datatype);
                    nodeToAst(nodeDatatype, datatype)->datatype = DT_RECORD;
                    a->node_syn = nodeDatatype;
                }
                else if (ptElement->ruleNo == 1)
                {
                    declareAstNode(nodeDatatype, AST_DATATYPE, Ast_Datatype, datatype);
                    nodeToAst(nodeDatatype, datatype)->datatype = DT_UNION;
                    a->node_syn = nodeDatatype;
                }
                break;
            }

            case ELSE_PART:
            {
                ParseTreeElement *else_part = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement* stmt = &ptElement->children[1];
                    ParseTreeElement* othStmt = &ptElement->children[2];
                    
                    declareAstNode(nodeOtherStmts, AST_OTHERSTMTS, Ast_OtherStmts,
                        otherStmts);
                    
                    nodeToAst(nodeOtherStmts, otherStmts)->otherStmtList = 
                        createAstList();
                    
                    stmt->node_inh = nodeOtherStmts;
                    handleParseTreeElement(stmt);

                    othStmt->node_inh = stmt->node_syn;
                    handleParseTreeElement(othStmt);

                    declareAstNode(nodeElsePart, AST_ELSEPART, Ast_ElsePart,
                        elsePart);
                    nodeToAst(nodeElsePart, elsePart)->otherStmts = 
                        othStmt->node_syn;

                    else_part->node_syn = nodeElsePart;
                }
                else if (ptElement->ruleNo == 1)
                {
                    declareAstNode(nodeOtherStmts, AST_OTHERSTMTS, Ast_OtherStmts,
                        otherStmts);
                    
                    nodeToAst(nodeOtherStmts, otherStmts)->otherStmtList = 
                        createAstList();
                    declareAstNode(nodeElsePart, AST_ELSEPART, Ast_ElsePart,
                        elsePart);

                    nodeToAst(nodeElsePart, elsePart)->otherStmts = 
                        nodeToAst(nodeOtherStmts, otherStmts);

                    else_part->node_syn = nodeElsePart;
                }
                break;
            }

            case ARITHMETIC_EXPRESSION:
            {
                ParseTreeElement* aExp = ptElement;
                ParseTreeElement* term = &ptElement->children[0];
                ParseTreeElement* expPrime = &ptElement->children[1];

                handleParseTreeElement(term);
                expPrime->node_inh = term->node_syn;
                handleParseTreeElement(expPrime);

                aExp->node_syn = expPrime->node_syn;
                break;
            }
            case TERM:
            {
                ParseTreeElement* term = ptElement;
                ParseTreeElement* factor = &ptElement->children[0];
                ParseTreeElement* tPrime = &ptElement->children[1];

                handleParseTreeElement(factor);
                tPrime->node_inh = factor->node_syn;
                handleParseTreeElement(tPrime);

                term->node_syn = tPrime->node_syn;
                break;
            }

            case LOW_PRECEDENCE_OPERATORS:
            {
                ParseTreeElement *low_precedence_operators = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    declareAstNode(nodeAop, AST_ARITHMETICOP, Ast_ArithmeticOperator,
                        arithmeticOp);

                    nodeToAst(nodeAop, arithmeticOp)->op = AOP_PLUS;
                    low_precedence_operators->node_syn = nodeAop;
                }
                else if (ptElement->ruleNo == 1)
                {
                    declareAstNode(nodeAop, AST_ARITHMETICOP, Ast_ArithmeticOperator,
                        arithmeticOp);

                    nodeToAst(nodeAop, arithmeticOp)->op = AOP_MINUS;
                    low_precedence_operators->node_syn = nodeAop;
                }
                break;
            }  

            case FACTOR:
            {
                ParseTreeElement *factor = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *arithmetic_expression = &ptElement->children[1];
                    handleParseTreeElement(arithmetic_expression);
                    factor->node_syn = arithmetic_expression->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *var = &ptElement->children[0];
                    handleParseTreeElement(var);
                    factor->node_syn = var->node_syn;
                }
                break;
            } 

            case TERM_PRIME:
            {
                ParseTreeElement *term_prime = ptElement;
                if (ptElement->ruleNo == 0){
                    ParseTreeElement* hpOp = &ptElement->children[0];
                    ParseTreeElement* factor = &ptElement->children[1];
                    ParseTreeElement* term_prime1 = &ptElement->children[2];

                    handleParseTreeElement(hpOp);
                    handleParseTreeElement(factor);

                    declareAstNode(nodeAexp, AST_ARITHMETICEXPR, Ast_ArithmeticExpression,
                        arithmeticExpression);
                    
                    nodeToAst(nodeAexp, arithmeticExpression)->op = 
                        nodeToAst(hpOp->node_syn, arithmeticOp)->op;
                    
                    AstNode* left = term_prime->node_inh;
                    AstNode* right = factor->node_syn;
                    Ast_ArithmeticExpression* aexp = nodeToAst(nodeAexp, arithmeticExpression);

                    if(left->type == AST_VAR){
                        aexp->lefType = AEXP_VAR;
                        aexp->left.var = nodeToAst(left, var);
                    }
                    else if(left->type == AST_ARITHMETICEXPR){
                        aexp->lefType = AEXP_EXP;
                        aexp_expLeft(aexp) = nodeToAst(left, arithmeticExpression);
                    }

                    if(right->type == AST_VAR){
                        aexp->rightType = AEXP_VAR;
                        aexp_varRight(aexp) = nodeToAst(right, var);
                    }

                    else if(right->type == AST_ARITHMETICEXPR){
                        aexp->rightType = AEXP_EXP;
                        aexp_expRight(aexp) = nodeToAst(right, arithmeticExpression);
                    }

                    term_prime1->node_inh = nodeAexp;
                    
                    handleParseTreeElement(term_prime1);

                    term_prime->node_syn = term_prime1->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    term_prime->node_syn = ptElement->node_inh;
                }
                break;
            }

            case HIGH_PRECEDENCE_OPERATORS:
            {
                ParseTreeElement *high_precedence_operators = ptElement;
                if (ptElement->ruleNo == 0)
                {
                    declareAstNode(nodeAop, AST_ARITHMETICOP, Ast_ArithmeticOperator,
                        arithmeticOp);
                    nodeToAst(nodeAop, arithmeticOp)->op = AOP_MUL;

                    high_precedence_operators->node_syn = nodeAop;
                }
                else if (ptElement->ruleNo == 1)
                {
                    declareAstNode(nodeAop, AST_ARITHMETICOP, Ast_ArithmeticOperator,
                        arithmeticOp);
                    nodeToAst(nodeAop, arithmeticOp)->op = AOP_DIV;

                    high_precedence_operators->node_syn = nodeAop;
                }
                break;
            }  
        }
    }

    else
    {
        Token t = ptElement->elem.symbol;
        printf("Error !! Calling handleParseTreeELement on terminal symbol %s", tokToStr(t));
    }
}