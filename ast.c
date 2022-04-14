#include "ast.h"
#include <stdlib.h>
#include "type.h"


AstNode *makeEmptyAstNode()
{
    AstNode *astNode = malloc(sizeof(AstNode));
    memset(&astNode->node, 0, sizeof(AstNodeUnion));
    memset(&astNode->type, 0, sizeof(AstNodeType));
    return astNode;
}

void handleParseTreeElement(ParseTreeElement *ptElement)
{
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

                insertTo(nodeToAst(func->node_inh, otherFunctions), nodeFunction);
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
                ParseTreeElement *remaining_list = &ptElement->children[2];

                TokenInfo tkId = parameter_list->children[1].tinfo;
                handleParseTreeElement(datatype);
                // TK_ID
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
                if (ptElement->ruleNo == 0)
                {
                    declareAstNode(nodeDatatype, AST_DATATYPE, Ast_Datatype, datatype);
                    nodeToAst(nodeDatatype, datatype)->typeExpr = numTypeExpression();
                    nodeToAst(nodeDatatype, datatype)->name = NULL;
                    primitive_datatype->node_syn = nodeDatatype;
                }
                else if (ptElement->ruleNo == 1)
                {
                    // MAKE LEAF FOR TK_REAL
                    declareAstNode(nodeDatatype, AST_DATATYPE, Ast_Datatype, datatype);
                    nodeToAst(nodeDatatype, datatype)->typeExpr = rnumTypeExpression();
                    nodeToAst(nodeDatatype, datatype)->name = NULL;
                    primitive_datatype->node_syn = nodeDatatype;
                }
                break;
            }

            case CONSTRUCTED_DATATYPE:
            {
                // MAKE LEAF FOR ALL 3 RULES
                ParseTreeElement *constructed_datatype = ptElement;
                TokenInfo tkRuid;

                if (ptElement->ruleNo == 0)
                {
                    tkRuid = ptElement->children[1].tinfo;
                }
                else if (ptElement->ruleNo == 1)
                {
                    tkRuid = ptElement->children[1].tinfo;
                }
                else if (ptElement->ruleNo == 2)
                {
                    tkRuid = ptElement->children[0].tinfo;
                }

                nodeToAst(nodeDatatype, datatype)->name = tkRuid.lexeme;

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
                handleParseTreeElement(type_definitions);
                handleParseTreeElement(declarations);
                handleParseTreeElement(other_stmts);
                handleParseTreeElement(return_stmt);

                declareAstNode(nodeStmts, AST_STMTS, Ast_Stmts, stmts);
                nodeToAst(nodeStmts, stmts)->typeDefinitions = nodeToAst(type_definitions->node_syn, typedefinitions);
                nodeToAst(nodeStmts, stmts)->declarations = nodeToAst(declarations->node_syn, declarations);
                nodeToAst(nodeStmts, stmts)->otherStmts = nodeToAst(other_stmts->node_syn, otherStmts);
                nodeToAst(nodeStmts, stmts)->returnIds = nodeToAst(return_stmt->node_syn, idList);
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
                    
                    nodeToAst(nodeTypeDefinition, typeDefinition)->fieldDefinitions = nodeToAst(field_def->node_syn, fieldDefinitions);
                    nodeToAst(nodeTypeDefinition, typeDefinition)->id1 = tkRuid.lexeme;
                    nodeToAst(nodeTypeDefinition, typeDefinition)->id2 = NULL;
                    nodeToAst(nodeTypeDefinition, typeDefinition)->datatype = DT_RECORD;

                    insertTo(nodeToAst(type_def->node_inh, typedefinitions)->typeDefintionList,
                        nodeTypeDefinition);

                    type_def->node_syn = type_def->node_inh;
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *field_def = &ptElement->children[2];
                    TokenInfo tkRuid = type_def->children[1].tinfo;
            
                    handleParseTreeElement(field_def);

                    declareAstNode(nodeTypeDefinition, AST_TYPEDEFINITION, Ast_TypeDefinition, typeDefinition);
                    
                    nodeToAst(nodeTypeDefinition, typeDefinition)->datatype = DT_UNION;
                    nodeToAst(nodeTypeDefinition, typeDefinition)->id1 = tkRuid.lexeme;
                    nodeToAst(nodeTypeDefinition, typeDefinition)->id2 = NULL;
                    nodeToAst(nodeTypeDefinition, typeDefinition)->fieldDefinitions 
                                = nodeToAst(field_def->node_syn, fieldDefinitions);

                    insertTo(nodeToAst(type_def->node_inh, typedefinitions)->typeDefintionList,
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
                    // nodeBooleanExpression->node.booleanExpression->bexp.boolOp->op = How do you check over here??
                    nodeBooleanExpression->node.booleanExpression->bexp.boolOp->left = boolean_expression1->node_syn;
                    nodeBooleanExpression->node.booleanExpression->bexp.boolOp->right = boolean_expression2->node_syn;
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
                    // nodeBooleanExpression->node.booleanExpression->bexp.varComp->op =
                    nodeBooleanExpression->node.booleanExpression->bexp.varComp->left = var1->node_syn;
                    nodeBooleanExpression->node.booleanExpression->bexp.varComp->right = var2->node_syn;
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
                    // nodeBooleanExpression->node.booleanExpression->bexp.boolOp->op = How do you check over here??
                    nodeBooleanExpression->node.booleanExpression->bexp.boolOp->left = boolean->node_syn;
                    nodeBooleanExpression->node.booleanExpression->bexp.boolOp->right = NULL;
                    ptElement->node_syn = nodeBooleanExpression;
                }
            }
            case VAR:
            {
                ParseTreeElement *var = &ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *singleorrecId = &ptElement->children[0];
                    handleParseTreeElement(singleorrecId);
                    var->node_syn = singleorrecId->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *tknum = &ptElement->children[0];
                    // leaf ast node creation.
                }
                else if (ptElement->ruleNo == 2)
                {
                    ParseTreeElement *tkrunm = &ptElement->children[0];
                    // leaf ast node creation.
                }
            }

            case LOGICAL_OP:
            {
                ParseTreeElement *localOp = &ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *tkand = &ptElement->children[0];
                    // leaf ast node creation.
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *tkor = &ptElement->children[0];
                    // leaf ast node creation.
                }
            }

            case RELATIONAL_OP:
            {
                ParseTreeElement *relationalOp = &ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *tklt = &ptElement->children[0];
                    // leaf ast node should be created
                }
                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *tkle = &ptElement->children[0];
                    // leaf ast node should be created
                }
                else if (ptElement->ruleNo == 2)
                {
                    ParseTreeElement *tkeq = &ptElement->children[0];
                    // leaf ast node should be created
                }
                else if (ptElement->ruleNo == 3)
                {
                    ParseTreeElement *tkgt = &ptElement->children[0];
                    // leaf ast node should be created
                }
                else if (ptElement->ruleNo == 4)
                {
                    ParseTreeElement *tkge = &ptElement->children[0];
                    // leaf ast node should be created
                }
                else if (ptElement->ruleNo == 5)
                {
                    ParseTreeElement *tkne = &ptElement->children[0];
                    // leaf ast node should be created
                }
            }

            case RETURN_STMT:
            {
                ParseTreeElement *returnStmt = &ptElement;
                ParseTreeElement *optioanlReturn = &ptElement->children[1];
                handleParseTreeElement(optioanlReturn);
                returnStmt->node_syn = optioanlReturn->node_syn;
            }

            case OPTIONAL_RETURN:
            {
                ParseTreeElement *optionalReturn = &ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *idList = &ptElement->children[1];
                    declareAstNode(nodeidList, ID_LIST, Ast_IdList, idList);
                    nodeidList->node.idList->idList = NULL;
                    idList->node_inh = nodeidList;
                    handleParseTreeElement(idList);
                    optionalReturn->node_syn = idList->node_syn;
                }
                else if (ptElement->ruleNo == 1)
                {
                    optionalReturn->node_syn = NULL;
                }
            }

            case ID_LIST:
            {
                ParseTreeElement *idList = &ptElement;
                ParseTreeElement *tkid = &ptElement->children[0];
                ParseTreeElement *moreIds = &ptElement->children[1];
                // make a leaf node of tkid.
                // then insert it to idlist inh
                //  and then equate it to moreids.inh
                handleParseTreeElement(moreIds);
                idList->node_syn = moreIds->node_syn;
            }

            case MORE_IDS:
            {
                ParseTreeElement *moreIds = &ptElement;

                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *idList = &ptElement->children[1];
                    idList->node_inh = moreIds->node_inh;
                    handleParseTreeElement(idList);
                    moreIds->node_syn = idList->node_syn;
                }

                else if (ptElement->ruleNo == 1)
                {
                    moreIds->node_syn = moreIds->node_syn;
                }
            }

            case ACTUAL_OR_REDEFINED:
            {
                ParseTreeElement *actualOrredefined = &ptElement;

                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *typeDefinition = &ptElement->children[0];
                    handleParseTreeElement(typeDefinition);
                    insertTo(actualOrredefined->node_inh->node.typedefinitions->typeDefintionList, typeDefinition->node_syn->node.typeDefinition);
                    ptElement->node_syn = ptElement->node_inh;
                }

                else if (ptElement->ruleNo == 1)
                {
                    ParseTreeElement *defineTypeStatement = &ptElement->children[0];
                    handleParseTreeElement(defineTypeStatement);
                    insertTo(actualOrredefined->node_inh->node.typedefinitions->typeDefintionList, defineTypeStatement->node_syn->node.typeDefinition);
                    ptElement->node_syn = ptElement->node_inh;
                }
            }

            case DEFINE_TYPE_STATEMENT:
            {
                ParseTreeElement *defineTypeStatement = &ptElement;
                // TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID1
                //<A> is either <TK_RECORD> or <TK_UNION>
                // basically A is a data type.

                ParseTreeElement *a = &ptElement->children[1];
                ParseTreeElement *tkRuId = &ptElement->children[2];
                ParseTreeElement *tkRuId1 = &ptElement->children[4];
                handleParseTreeElement(a);
                handleParseTreeElement(tkRuId1);
                handleParseTreeElement(tkRuId);
                declareAstNode(nodedefineTypeStatement, TYPE_DEFINITION, Ast_TypeDefinition, typedefinition);
                // equate id1,id2 and type.
                ptElement->node_syn = nodedefineTypeStatement;
            }

            case FIELD_TYPE:
            {
                ParseTreeElement *fieldType = &ptElement;
                if (ptElement->ruleNo == 0)
                {
                    ParseTreeElement *primitiveDatatype = &ptElement->children[0];
                    fieldType->node_syn = primitiveDatatype->node_syn;
                }
                else if(ptElement->ruleNo == 1){
                    ParseTreeElement *tkRuId = &ptElement->children[1];
                    //need to make a leaf and then insert.
                }
            }

            case OPTION_SINGLE_CONSTRUCTED:
            {
                ParseTreeElement *optionSingleConstructed = &ptElement;

                if(ptElement->ruleNo == 0){
                    ParseTreeElement *oneExpansion = &ptElement->children[0];
                    ParseTreeElement *moreExpansions = &ptElement->children[1];
                    declareAstNode(nodeOneExpansion,AST_OPTIONSINGLECONSTRUCTED,Ast_OptionSingleConstructed,optionSingleConstructed);
                    nodeOneExpansion->node.optionSingleConstructed->fieldNameList = NULL;
                    oneExpansion->node_inh = nodeOneExpansion;
                    handleParseTreeElement(oneExpansion);
                    handleParseTreeElement(moreExpansions);
                    declareAstNode(nodeOptionSingleConstructed,AST_OPTIONSINGLECONSTRUCTED,Ast_OptionSingleConstructed,optionSingleConstructed);
                    nodeOptionSingleConstructed->node.optionSingleConstructed->fieldNameList = moreExpansions->node_syn->node.optionSingleConstructed->fieldNameList;
                    optionSingleConstructed->node_syn = nodeOptionSingleConstructed;
                }
                else if(ptElement->ruleNo == 1){
                    optionSingleConstructed->node_syn = NULL;
                }
            }
            
            case ONE_EXPANSION:
            {
                ParseTreeElement *oneExpansion = &ptElement;
                //create a leaf node of TK_FIELD and add it to inh 
                ptElement->node_syn = ptElement->node_inh;
            }

            case MORE_EXPANSIONS:
            {
                ParseTreeElement *moreExpansions = &ptElement;
                
                if(moreExpansions->ruleNo == 0){
                    ParseTreeElement *oneExpansion = &ptElement->children[0];
                    ParseTreeElement *moreExpansion1 = &ptElement->children[1];
                    oneExpansion->node_inh = moreExpansions->node_inh;
                    handleParseTreeElement(oneExpansion);
                    moreExpansion1->node_inh = oneExpansion->node_syn;
                    handleParseTreeElement(moreExpansion1);
                    moreExpansions->node_syn = moreExpansion1->node_syn;
                }

                else if(moreExpansions->ruleNo == 1){
                    moreExpansions->node_syn = moreExpansions->node_inh;
                }
            }

        }
    }
    else
    {
    }
}
