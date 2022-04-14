#include "ast.h"
#include <stdlib.h>

#define declareAstNode(VARNAME, ENUM, STRUCT, FIELD) \
    AstNode *VARNAME = makeEmptyAstNode();           \
    VARNAME->type = ENUM;                            \
    VARNAME->node.FIELD = malloc(sizeof(STRUCT));

#define nodeToAst(VAR, FIELD) VAR->node.FIELD

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
            ParseTreeElement *input_par = &ptElement->children[1];
            ParseTreeElement *output_par = &ptElement->children[2];
            ParseTreeElement *statements = &ptElement->children[4];
            handleParseTreeElement(input_par);
            handleParseTreeElement(output_par);
            handleParseTreeElement(statements);
            declareAstNode(nodeFunction, AST_FUNCTION, Ast_Function, function);
            // FUNID
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
                // MAKE LEAF FOR TK_INT
            }
            else if (ptElement->ruleNo == 1)
            {
                // MAKE LEAF FOR TK_REAL
            }
            break;
        }

        case CONSTRUCTED_DATATYPE:
        {
            // MAKE LEAF FOR ALL 3 RULES
            ParseTreeElement *constructed_datatype = ptElement;
            if (ptElement->ruleNo == 0)
            {
            }
            else if (ptElement->ruleNo == 1)
            {
            }
            else if (ptElement->ruleNo == 2)
            {
            }
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
                // TK_RUID AND TK_RECORD
                handleParseTreeElement(field_def);

                declareAstNode(nodeTypeDefinition, AST_TYPEDEFINITION, Ast_TypeDefinition, typeDefinition);
                // TK_RECORD AND TK_RUID
                nodeToAst(nodeTypeDefinition, typeDefinition)->fieldDefinitions = nodeToAst(field_def->node_syn, fieldDefinitions);
                insertTo(nodeToAst(type_def->node_inh, typedefinitions));
                type_def->node_syn = type_def->node_inh;
            }
            else if (ptElement->ruleNo == 1)
            {
                ParseTreeElement *field_def = &ptElement->children[2];
                // TK_RUID AND TK_UNION
                handleParseTreeElement(field_def);

                declareAstNode(nodeTypeDefinition, AST_TYPEDEFINITION, Ast_TypeDefinition, typeDefinition);
                // TK_RECORD AND TK_UNION
                nodeToAst(nodeTypeDefinition, typeDefinition)->fieldDefinitions = nodeToAst(field_def->node_syn, fieldDefinitions);
                insertTo(nodeToAst(type_def->node_inh, typedefinitions));
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
                declareAstNode();

            }
            else if (ptElement->ruleNo == 1)
            {
            }
            else if (ptElement->ruleNo == 2)
            {
            }
        }
        }
    }
    else
    {
    }
}
