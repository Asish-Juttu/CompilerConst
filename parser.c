#include "parser.h"
#include "parserDef.h"
#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "string.h"




TokenSet firstSetDp[NON_TERMINAL_SIZE];
int tokRuleNum[NON_TERMINAL_SIZE][TOKEN_SIZE];
LocationArray ntLocation[NON_TERMINAL_SIZE];

void initFNF(){
    memset(firstSetDp, 0, NON_TERMINAL_SIZE * sizeof(TokenSet));
}
NonTermSet nullNonTermSet(){
    return (NonTermSet){0};
}

NonTermSet singletonNonTermSet(NonTerminal nt){
    return (NonTermSet){1ULL << nt};
}

NonTermSet nonTermSetUnion(NonTermSet s1, NonTermSet s2){
    return (NonTermSet){s1.bitMask | s2.bitMask};
}

TokenSet removeEpsilon(TokenSet s1){
    return (TokenSet){s1.bitMask & ~(1ULL << EPSILON)};
}

int equalsNonTermSet(NonTermSet s1, NonTermSet s2){
    return s1.bitMask == s2.bitMask;
}

int isNullable(Grammar* grammar, NonTerminal nt){
    return (grammar->nullableSet.bitMask >> nt) & 1ULL == 1ULL;
}

int nonTermSetContains(NonTermSet ntSet, NonTerminal nt){
    return (ntSet.bitMask >> nt) & 1ULL;
}

void addNullable(NonTerminal nt, Grammar* grammar){
    (grammar->nullableSet) = nonTermSetUnion(grammar->nullableSet, singletonNonTermSet(nt));
}

void updateNumOccur(Symbol* symbol, int n){
    for(int i = 0; i < n; i++){
        if(!symbol[i].isTerminal){
            ntLocation[symbol[i].symbol].size++;
        }
    }
}


void printSymbols(Symbol* syms, int size){
    for(int i = 0; i < size; i++){
        Symbol s  = syms[i];
        if(s.isTerminal){
            LOG("%s ", tokToStr(s.symbol));
        }
        else{
            LOG("%s ", nonTermToStr(s.symbol));
        }
    }
}
void addRule(Grammar* grammar, NonTerminal nt, Symbol* symbols, int size, int rNum){
    Symbol* sym = malloc(size * sizeof(Symbol));
    memmove(sym, symbols, size * sizeof(Symbol));
    Rule r = {size, sym};
    if(rNum >= grammar->ruleArray[nt].size || rNum < 0){
        LOG("Error !! rNum greater than size for %s \n", nonTermToStr(nt));
        return;
    }
    
    

    grammar->ruleArray[nt].rule[rNum] = r;
    LOG("[Adding Rule {");
    printSymbols(grammar->ruleArray[nt].rule[rNum].symbol, size);
    LOG("} to NonTerminal %s ]\n", nonTermToStr(nt));
    updateNumOccur(symbols, size);
    if(symbols[0].symbol == EPSILON){
        // printf("Adding Nullables %s \n", nonTermToStr(nt));
        addNullable(nt, grammar);

        // printf("NonTerminal Set now is ");
        // printNTSet(grammar->nullableSet);
        // printf("\n");
    }
}

void initRuleArray(Grammar* grammar, NonTerminal nt, int size){
    LOG("[Initializing rule array for %s]\n", nonTermToStr(nt));
    grammar->ruleArray[nt].size = size;
    grammar->ruleArray[nt].rule = (Rule*)malloc(size * sizeof(Rule));
}

void initNumOccur(){
    for(int i = 0; i < NON_TERMINAL_SIZE; i++)
        ntLocation[i].size = 0;
}


void initLocations(Grammar* grammar){

    int pos[NON_TERMINAL_SIZE];
    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        ntLocation[i].location = (Location*)malloc(ntLocation[i].size * sizeof(Location));
        pos[i] = 0;
    }
    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        for(int j = 0; j < grammar->ruleArray[i].size; j++){
            for(int k = 0; k < grammar->ruleArray[i].rule[j].size; k++){
                Symbol s = grammar->ruleArray[i].rule[j].symbol[k];
                if(!s.isTerminal){
                    ntLocation[s.symbol].location[pos[s.symbol]++] = (Location){i, j, k};
                }
            }
        }
    }
}

void initGrammar(Grammar* grammar){
    initNumOccur();
    initFNF();
    grammar->nullableSet = nullNonTermSet();
    printf("Nullable set will print now");
    printNTSet(grammar->nullableSet);
    printf("Printed");
    int size = NON_TERMINAL_SIZE;

    grammar->size = size;
    grammar->ruleArray = (RuleArray*)malloc(size * sizeof(RuleArray));
    
    Symbol program[] = {{0, OTHER_FUNCTIONS}, {0, MAIN_FUNCTION}};
    initRuleArray(grammar, PROGRAM, 1);
    addRule(grammar, PROGRAM, program, 2, 0);

    Symbol mainFunction[] = {{1, TK_MAIN}, {0, STMTS}, {1, TK_END}};
    initRuleArray(grammar, MAIN_FUNCTION, 1);
    addRule(grammar, MAIN_FUNCTION, mainFunction, 3, 0);

    Symbol otherFunctions0[] = {{0, FUNCTION}, {0, OTHER_FUNCTIONS}};
    Symbol otherFunctions1[] = {{1, EPSILON}};
    initRuleArray(grammar, OTHER_FUNCTIONS, 2);
    addRule(grammar, OTHER_FUNCTIONS, otherFunctions0, 2, 0);
    addRule(grammar, OTHER_FUNCTIONS, otherFunctions1, 1, 1);
    
    Symbol function[] = {{1,TK_FUNID}, {0,INPUT_PAR}, {0,OUTPUT_PAR}, {1,TK_SEM}, {0,STMTS}, {1,TK_END}};
    initRuleArray(grammar, FUNCTION,1);
    addRule(grammar, FUNCTION, function, 6, 0);
    
    Symbol input_par[] = {{1,TK_INPUT}, {1,TK_PARAMETER}, {1,TK_LIST}, {1,TK_SQL}, {0,PARAMETER_LIST}, {1,TK_SQR}};
    initRuleArray(grammar, INPUT_PAR,1);
    addRule(grammar, INPUT_PAR, input_par, 6, 0);
    
    Symbol output_par0[] = {{1,TK_OUTPUT}, {1,TK_PARAMETER}, {1,TK_LIST}, {1,TK_SQL}, {0,PARAMETER_LIST}, {1,TK_SQR}};
    Symbol output_par1[] = {{1, EPSILON}};
    initRuleArray(grammar, OUTPUT_PAR,2);
    addRule(grammar, OUTPUT_PAR, output_par0, 6, 0);
    addRule(grammar, OUTPUT_PAR, output_par1, 1, 1);
    
    Symbol parameter_list[] = {{0,DATATYPE}, {1,TK_ID}, {0,REMAINING_LIST}};
    initRuleArray(grammar, PARAMETER_LIST,1);
    addRule(grammar, PARAMETER_LIST, parameter_list , 3, 0);
    
    Symbol datatype0[] = {{0, PRIMITIVE_DATATYPE}};
    Symbol datatype1[] = {{0, CONSTRUCTED_DATATYPE}};
    initRuleArray(grammar, DATATYPE,2);
    addRule(grammar, DATATYPE, datatype0, 1, 0);
    addRule(grammar, DATATYPE, datatype1, 1, 1);
    
    Symbol primitiveDatatype0[] = {{1, TK_INT}};
    Symbol primitiveDatatype1[] = {{1, TK_REAL}};
    initRuleArray(grammar, PRIMITIVE_DATATYPE,2);
    addRule(grammar, PRIMITIVE_DATATYPE, primitiveDatatype0, 1, 0);
    addRule(grammar, PRIMITIVE_DATATYPE, primitiveDatatype1, 1, 1);
    
    Symbol constructedDatatype0[] = {{1, TK_RECORD}, {1,TK_RUID}};
    Symbol constructedDatatype1[] = {{1, TK_UNION}, {1,TK_RUID}};
    Symbol constructedDatatype2[] = {{1,TK_RUID}};
    initRuleArray(grammar, CONSTRUCTED_DATATYPE,3);
    addRule(grammar, CONSTRUCTED_DATATYPE, constructedDatatype0, 2, 0);
    addRule(grammar, CONSTRUCTED_DATATYPE, constructedDatatype1, 2, 1);
    addRule(grammar, CONSTRUCTED_DATATYPE, constructedDatatype2, 1, 2);
    
    Symbol remaining_list0[] = {{1, TK_COMMA}, {0,PARAMETER_LIST}};
    Symbol remaining_list1[] = {{1, EPSILON}};
    initRuleArray(grammar, REMAINING_LIST,2);
    addRule(grammar, REMAINING_LIST, remaining_list0, 2, 0);
    addRule(grammar, REMAINING_LIST, remaining_list1, 1, 1);
      
    Symbol stmts[] = {{0,TYPE_DEFINITIONS}, {0,DECLARATIONS}, {0,OTHER_STMTS}, {0,RETURN_STMT}};
    initRuleArray(grammar, STMTS,1);
    addRule(grammar, STMTS, stmts, 4, 0);

    Symbol typeDefinitions0[] = {{0, ACTUAL_OR_REDEFINED}, {0,TYPE_DEFINITIONS}};
    Symbol typeDefinitions1[] = {{1, EPSILON}};
    initRuleArray(grammar, TYPE_DEFINITIONS,2);
    addRule(grammar, TYPE_DEFINITIONS, typeDefinitions0, 2, 0);
    addRule(grammar, TYPE_DEFINITIONS, typeDefinitions1, 1, 1);

    Symbol actualOrRedefined0[] = {{0, TYPE_DEFINITION}};
    Symbol actualOrRedefined1[] = {{0, DEFINE_TYPE_STATEMENT}};
    initRuleArray(grammar, ACTUAL_OR_REDEFINED,2);
    addRule(grammar, ACTUAL_OR_REDEFINED, actualOrRedefined0, 1, 0);
    addRule(grammar, ACTUAL_OR_REDEFINED, actualOrRedefined1, 1, 1);

    Symbol typeDefinition0[] = {{1, TK_RECORD}, {1, TK_RUID}, {0, FIELD_DEFINITIONS}, {1, TK_ENDRECORD}};
    Symbol typeDefinition1[] = {{1, TK_UNION}, {1, TK_RUID}, {0, FIELD_DEFINITIONS}, {1, TK_ENDUNION}};
    initRuleArray(grammar, TYPE_DEFINITION,2);
    addRule(grammar, TYPE_DEFINITION, typeDefinition0, 4, 0);
    addRule(grammar, TYPE_DEFINITION, typeDefinition1, 4, 1);
    
    Symbol fieldDefinitions0[] = {{0,FIELD_DEFINITION}, {0,FIELD_DEFINITION}, {0,MORE_FIELDS}};
    initRuleArray(grammar, FIELD_DEFINITIONS,1);
    addRule(grammar, FIELD_DEFINITIONS, fieldDefinitions0 , 3, 0);

    Symbol fieldDefinition0[] = {{1,TK_TYPE}, {0,FIELD_TYPE}, {1,TK_COLON}, {1,TK_FIELDID}, {1,TK_SEM}};
    initRuleArray(grammar, FIELD_DEFINITION,1);
    addRule(grammar, FIELD_DEFINITION, fieldDefinition0 , 5, 0);

    Symbol fieldType0[] = {{0,PRIMITIVE_DATATYPE}, {1,TK_RUID}};
    initRuleArray(grammar, FIELD_TYPE,1);
    addRule(grammar, FIELD_TYPE, fieldType0 , 2, 0);

    Symbol moreFields0[] = {{0, FIELD_DEFINITION}, {0,MORE_FIELDS}};
    Symbol moreFields1[] = {{1, EPSILON}};
    initRuleArray(grammar, MORE_FIELDS, 2);
    addRule(grammar, MORE_FIELDS, moreFields0, 2, 0);
    addRule(grammar, MORE_FIELDS, moreFields1, 1, 1);

    Symbol declarations0[] = {{0, DECLARATION}, {0,DECLARATIONS}};
    Symbol declarations1[] = {{1, EPSILON}};
    initRuleArray(grammar, DECLARATIONS, 2);
    addRule(grammar, DECLARATIONS, declarations0, 2, 0);
    addRule(grammar, DECLARATIONS, declarations1, 1, 1);

    Symbol declaration0[] = {{1,TK_TYPE}, {0, DATATYPE}, {1,TK_COLON}, {1,TK_ID}, {0,GLOBAL_OR_NOT}, {1,TK_SEM}};
    initRuleArray(grammar, DECLARATION,1);
    addRule(grammar, DECLARATION, declaration0 , 6, 0);

    Symbol global_or_not0[] = {{1,TK_COLON}, {1,TK_GLOBAL}};
    Symbol global_or_not1[] = {{1, EPSILON}};
    initRuleArray(grammar, GLOBAL_OR_NOT,2);
    addRule(grammar, GLOBAL_OR_NOT, global_or_not0, 2, 0);
    addRule(grammar, GLOBAL_OR_NOT, global_or_not1, 1, 1);

    Symbol otherStmts0[] = {{0,STMT}, {0,OTHER_STMTS}};
    Symbol otherStmts1[] = {{1, EPSILON}};
    initRuleArray(grammar, OTHER_STMTS,2);
    addRule(grammar, OTHER_STMTS, otherStmts0, 2, 0);
    addRule(grammar, OTHER_STMTS, otherStmts1, 1, 1);

    Symbol stmt0[] = {{0, ASSIGNMENT_STMT}};
    Symbol stmt1[] = {{0, ITERATIVE_STMT}};
    Symbol stmt2[] = {{0, CONDITIONAL_STMT}};
    Symbol stmt3[] = {{0, IO_STMT}};
    Symbol stmt4[] = {{0, FUN_CALL_STMT}};
    initRuleArray(grammar, STMT, 5);
    addRule(grammar, STMT, stmt0, 1, 0);
    addRule(grammar, STMT, stmt1, 1, 1);
    addRule(grammar, STMT, stmt2, 1, 2);
    addRule(grammar, STMT, stmt3, 1, 3);
    addRule(grammar, STMT, stmt4, 1, 4);
     

    Symbol assignmentStmt0[] = {{0, SINGLE_OR_REC_ID}, {1,TK_ASSIGNOP}, {0,ARITHMETIC_EXPRESSION}, {1,TK_SEM}};
    initRuleArray(grammar, ASSIGNMENT_STMT,1);
    addRule(grammar, ASSIGNMENT_STMT, assignmentStmt0 , 4, 0);

    Symbol singleOrRecId0[] = {{1,TK_ID},{0, OPTION_SINGLE_CONSTRUCTED}};
    initRuleArray(grammar, SINGLE_OR_REC_ID,1);
    addRule(grammar, SINGLE_OR_REC_ID, singleOrRecId0, 2, 0);

    Symbol option_single_constructed0[] = {{0,ONE_EXPANSION}, {0,MORE_EXPANSIONS}};
    Symbol option_single_constructed1[] = {{1, EPSILON}};
    initRuleArray(grammar, OPTION_SINGLE_CONSTRUCTED,2);
    addRule(grammar, OPTION_SINGLE_CONSTRUCTED, option_single_constructed0, 2, 0);
    addRule(grammar, OPTION_SINGLE_CONSTRUCTED, option_single_constructed1, 1, 1);

    Symbol oneExpansion0[] = {{1,TK_DOT},{1, TK_FIELDID}};
    initRuleArray(grammar, ONE_EXPANSION,1);
    addRule(grammar, ONE_EXPANSION, oneExpansion0, 2, 0);

    Symbol moreExpansions0[] = {{0,ONE_EXPANSION}, {0,MORE_EXPANSIONS}};
    Symbol moreExpansions1[] = {{1, EPSILON}};
    initRuleArray(grammar, MORE_EXPANSIONS,2);
    addRule(grammar, MORE_EXPANSIONS, moreExpansions0, 2, 0);
    addRule(grammar, MORE_EXPANSIONS, moreExpansions1, 1, 1);

    Symbol funCallStmt[] = {{0,OUTPUT_PARAMETERS},{1,TK_CALL},{1, TK_FUNID}, {1, TK_WITH}, {1, TK_PARAMETERS}, {0,INPUT_PARAMETERS}, {1, TK_SEM} };
    initRuleArray(grammar, FUN_CALL_STMT,1);
    addRule(grammar, FUN_CALL_STMT, funCallStmt, 7, 0);

    Symbol outputParameters0[] = {{1,TK_SQL}, {0,ID_LIST}, {1,TK_SQR}, {1,TK_ASSIGNOP}};
    Symbol outputParameters1[] = {{1, EPSILON}};
    initRuleArray(grammar, OUTPUT_PARAMETERS,2);
    addRule(grammar, OUTPUT_PARAMETERS, outputParameters0, 4, 0);
    addRule(grammar, OUTPUT_PARAMETERS, outputParameters1, 1, 1);

    Symbol inputParameters0[] = {{1,TK_SQL}, {0,ID_LIST}, {1,TK_SQR}};
    initRuleArray(grammar, INPUT_PARAMETERS,1);
    addRule(grammar, INPUT_PARAMETERS, inputParameters0, 3, 0);

    Symbol iterativeStmt0[] = {{1,TK_WHILE}, {1,TK_OP},{0,BOOLEAN_EXPRESSION}, {1,TK_CL}, {0,STMT}, {0,OTHER_STMTS}, {1,TK_ENDWHILE}};
    initRuleArray(grammar, ITERATIVE_STMT,1);
    addRule(grammar, ITERATIVE_STMT, iterativeStmt0, 7, 0);

    Symbol conditionalStmt[] = {{1,TK_IF}, {1,TK_OP}, {0,BOOLEAN_EXPRESSION}, {1,TK_CL}, {1,TK_THEN}, {0,STMT}, {0,OTHER_STMTS}, {0,ELSE_PART}};
    initRuleArray(grammar, CONDITIONAL_STMT,1);
    addRule(grammar, CONDITIONAL_STMT, conditionalStmt , 8, 0);

    Symbol elsePart0[] = {{1,TK_ELSE}, {0,STMT}, {0,OTHER_STMTS}, {1,TK_ENDIF}};
    Symbol elsePart1[] = {{1,TK_ENDIF}};
    initRuleArray(grammar, ELSE_PART,2);
    addRule(grammar, ELSE_PART, elsePart0 , 4, 0);
    addRule(grammar, ELSE_PART, elsePart1 , 1, 1);

    Symbol ioStmt0[] = {{1,TK_READ}, {1,TK_OP}, {0,VAR},{1,TK_CL},{1,TK_SEM}};
    Symbol ioStmt1[] = {{1,TK_WRITE}, {1,TK_OP}, {0,VAR},{1,TK_CL},{1,TK_SEM}};
    initRuleArray(grammar, IO_STMT,2);
    addRule(grammar, IO_STMT, ioStmt0 , 5, 0);
    addRule(grammar, IO_STMT, ioStmt1 , 5, 1);

    
    Symbol arithmeticExpression[] = {{0,TERM}, {0,EXP_PRIME}};
    initRuleArray(grammar, ARITHMETIC_EXPRESSION,1);
    addRule(grammar, ARITHMETIC_EXPRESSION, arithmeticExpression, 2, 0);

    Symbol expPrime0[] = {{0,LOW_PRECEDENCE_OPERATORS}, {0,TERM},{0,EXP_PRIME}};
    Symbol expPrime1[] = {{1,EPSILON}};
    initRuleArray(grammar, EXP_PRIME,2);
    addRule(grammar, EXP_PRIME, expPrime0 , 3, 0);
    addRule(grammar, EXP_PRIME, expPrime1 , 1, 1);

    Symbol term[] = {{0,FACTOR}, {0,TERM_PRIME}};
    initRuleArray(grammar, TERM,1);
    addRule(grammar, TERM, term , 2, 0);

    Symbol termPrime0[] = {{0,HIGH_PRECEDENCE_OPERATORS}, {0,FACTOR},{0,TERM_PRIME}};
    Symbol termPrime1[] = {{1,EPSILON}};
    initRuleArray(grammar, TERM_PRIME,2);
    addRule(grammar, TERM_PRIME, termPrime0 , 3, 0);
    addRule(grammar, TERM_PRIME, termPrime1 , 1, 1);

    Symbol factor0[] = {{1,TK_OP}, {0,ARITHMETIC_EXPRESSION},{1,TK_CL}};
    Symbol factor1[] = {{0,VAR}};
    initRuleArray(grammar, FACTOR,2);
    addRule(grammar, FACTOR, factor0 , 3, 0);
    addRule(grammar, FACTOR, factor1 , 1, 1);

    
    Symbol highPrecedenceOperators0[] = {{1,TK_MUL}};
    Symbol highPrecedenceOperators1[] = {{1,TK_DIV}};
    initRuleArray(grammar, HIGH_PRECEDENCE_OPERATORS,2);
    addRule(grammar, HIGH_PRECEDENCE_OPERATORS, highPrecedenceOperators0 , 1, 0);
    addRule(grammar, HIGH_PRECEDENCE_OPERATORS, highPrecedenceOperators1 , 1, 1);

    Symbol lowPrecedenceOperators0[] = {{1,TK_PLUS}};
    Symbol lowPrecedenceOperators1[] = {{1,TK_MINUS}};
    initRuleArray(grammar, LOW_PRECEDENCE_OPERATORS,2);
    addRule(grammar, LOW_PRECEDENCE_OPERATORS, lowPrecedenceOperators0 , 1, 0);
    addRule(grammar, LOW_PRECEDENCE_OPERATORS, lowPrecedenceOperators1 , 1, 1);

    
    Symbol booleanExpression0[] = {{1,TK_OP}, {0,BOOLEAN_EXPRESSION},{1,TK_CL},{0,LOGICAL_OP},{1,TK_OP},{0,BOOLEAN_EXPRESSION},{1,TK_CL}};
    Symbol booleanExpression1[] = {{0,VAR},{0,RELATIONAL_OP},{0,VAR}};
    Symbol booleanExpression2[] = {{1,TK_NOT},{1,TK_OP}, {0,BOOLEAN_EXPRESSION},{1,TK_CL}};
    initRuleArray(grammar, BOOLEAN_EXPRESSION,3);
    addRule(grammar, BOOLEAN_EXPRESSION, booleanExpression0 , 7, 0);
    addRule(grammar, BOOLEAN_EXPRESSION, booleanExpression1 , 3, 1);
    addRule(grammar, BOOLEAN_EXPRESSION, booleanExpression2 , 4, 2);

    Symbol var0[] = {{0,SINGLE_OR_REC_ID}};
    Symbol var1[] = {{1,TK_NUM}};
    Symbol var2[] = {{1,TK_RNUM}};
    initRuleArray(grammar, VAR,3);
    addRule(grammar, VAR, var0 , 1, 0);
    addRule(grammar, VAR, var1 , 1, 1);
    addRule(grammar, VAR, var2 , 1, 2);

    Symbol logicalOp0[] = {{1,TK_AND}};
    Symbol logicalOp1[] = {{1,TK_OR}};
    initRuleArray(grammar, LOGICAL_OP,2);
    addRule(grammar, LOGICAL_OP, logicalOp0 , 1, 0);
    addRule(grammar, LOGICAL_OP, logicalOp1 , 1, 1);

    Symbol relationalOp0[] = {{1,TK_LT}};
    Symbol relationalOp1[] = {{1,TK_LE}};
    Symbol relationalOp2[] = {{1,TK_EQ}};
    Symbol relationalOp3[] = {{1,TK_GT}};
    Symbol relationalOp4[] = {{1,TK_GE}};
    Symbol relationalOp5[] = {{1,TK_NE}};
    initRuleArray(grammar, RELATIONAL_OP,6);
    addRule(grammar, RELATIONAL_OP, relationalOp0 , 1, 0);
    addRule(grammar, RELATIONAL_OP, relationalOp1 , 1, 1);
    addRule(grammar, RELATIONAL_OP, relationalOp2 , 1, 2);
    addRule(grammar, RELATIONAL_OP, relationalOp3 , 1, 3);
    addRule(grammar, RELATIONAL_OP, relationalOp4 , 1, 4);
    addRule(grammar, RELATIONAL_OP, relationalOp5 , 1, 5);

    Symbol returnStmt[] = {{1,TK_RETURN}, {0,OPTIONAL_RETURN}, {1,TK_SEM}};
    initRuleArray(grammar, RETURN_STMT,1);
    addRule(grammar, RETURN_STMT, returnStmt , 3, 0);

    Symbol optionalReturn0[] = {{1,TK_SQL}, {0,ID_LIST}, {1,TK_SQR}};
    Symbol optionalReturn1[] = {{1,EPSILON}};
    initRuleArray(grammar, OPTIONAL_RETURN,2);
    addRule(grammar, OPTIONAL_RETURN, optionalReturn0 , 3, 0);
    addRule(grammar, OPTIONAL_RETURN, optionalReturn1 , 1, 1);

    Symbol idList[] = {{1,TK_ID}, {0,MORE_IDS}};
    initRuleArray(grammar, ID_LIST,1);
    addRule(grammar, ID_LIST, idList , 2, 0);

    Symbol more_ids0[] = {{1,TK_COMMA}, {0,ID_LIST}};
    Symbol more_ids1[] = {{1,EPSILON}};
    initRuleArray(grammar, MORE_IDS,2);
    addRule(grammar, MORE_IDS, more_ids0 , 2, 0);
    addRule(grammar, MORE_IDS, more_ids1 , 1, 1);

    Symbol definetypestmt0[] = {{1,TK_DEFINETYPE}, {0,A}, {1,TK_RUID}, {1,TK_AS}, {1,TK_RUID}};
    initRuleArray(grammar, DEFINE_TYPE_STATEMENT,1);
    addRule(grammar, DEFINE_TYPE_STATEMENT, definetypestmt0, 5, 0);
 
    Symbol A0[] = {{1,TK_RECORD},{1,TK_UNION}};
    initRuleArray(grammar, A,1);
    addRule(grammar, A, A0, 2, 0);

    // initialize nset at the end
    initLocations(grammar);
}

void printNTSet(NonTermSet ntSet){
    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        if(nonTermSetContains(ntSet, i)){
            printf("%s ", nonTermToStr(i));
        }
    }
    printf("\n");
}
void printTSet(TokenSet tSet){
    for(int i = 0; i < TOKEN_SIZE; i++){
        if(tokenSetContains(tSet, i)){
            printf("%s ", tokToStr(i));
        }
    }
    printf("\n");
}

void recordRuleNumber(TokenSet tSet, NonTerminal nt, int ruleNum){
    // printf("%s Rule %d => ", nonTermToStr(nt), ruleNum);
    for(int i = 0; i < TOKEN_SIZE; i++){
        if(tokenSetContains(tSet, i)){
            tokRuleNum[nt][i] = ruleNum;
            //printf("%s ", tokToStr(i));
        }
    }

}

TokenSet first(Grammar* grammar, NonTerminal nt){
    TokenSet ftSet = nullTokenSet();
    // LOG("%s \n", nonTermToStr(nt));
    // LOG("--------------------------\n");
    if(!equalsTokenSet(firstSetDp[nt],nullTokenSet()))
        return firstSetDp[nt];
    else {
        for(int i = 0; i < grammar->ruleArray[nt].size; i++){
            TokenSet tSet = nullTokenSet();
            for(int j = 0; j < grammar->ruleArray[nt].rule[i].size; j++){
                Symbol s = grammar->ruleArray[nt].rule[i].symbol[j];
                if(s.isTerminal){
                    tSet = tokenSetUnion(tSet, singletonTokenSet(s.symbol));
                    printf("%s\n", tokToStr(s.symbol));
                    break;
                }
                else{
                    tSet = tokenSetUnion(tSet, first(grammar, s.symbol));
                    // LOG("%s is ", nonTermToStr(s.symbol));
                    // printTSet(tSet);
                    // LOG("\n");

                    if(!isNullable(grammar, s.symbol))
                        break;
                    tSet = removeEpsilon(tSet);
                }
            }
            recordRuleNumber(tSet, nt, i);
            ftSet = tokenSetUnion(ftSet, tSet);
            //LOG("%lld\n", ftSet.bitMask);
        }
        
    }
    if(equalsTokenSet(ftSet, nullTokenSet()))
        ftSet = tokenSetUnion(ftSet, singletonTokenSet(EPSILON));
    firstSetDp[nt] = ftSet;
    return ftSet;
}

FollowHelperSet followHelper(Grammar* grammar, NonTerminal nt){
    NonTermSet followDep = nullNonTermSet();
    TokenSet firstDep = nullTokenSet();
    
    for(int i = 0; i < ntLocation[nt].size; i++){
        Location l = ntLocation[nt].location[i];
        l.index++;

        int n = grammar->ruleArray[l.nt].rule[l.ruleNo].size;
        while(l.index < n){
            Symbol s = grammar->ruleArray[l.nt].rule[l.ruleNo].symbol[l.index];
            if(s.isTerminal){
                firstDep = tokenSetUnion(firstDep, singletonTokenSet(s.symbol));
                //printf("%s\n", tokToStr(s.symbol));
                break;
            }
            else{
                firstDep = tokenSetUnion(firstDep, first(grammar, s.symbol));
                firstDep = removeEpsilon(firstDep);
                //printf("%s ", nonTermToStr(s.symbol));
                if(!isNullable(grammar, s.symbol))
                    break;
            }
            l.index++;
        }
        //Symbol lastSymbol = grammar->ruleArray[l.nt].rule[l.ruleNo].symbol[n - 1];
        if(l.index == n ){
            followDep = nonTermSetUnion(followDep, singletonNonTermSet(l.nt));
        }
    }

    return (FollowHelperSet){followDep, firstDep};
}

// ParseTable initParseTable(Grammar* grammar,FirstAndFollow* f){
//       struct ParseTable parsetable;
//       for(int i=0;i<grammar->size;i++){
//           for(int j=0;j<grammar->ruleArray->size;j++){
//               int isepsilon = 1;
//               for(int k=0;(k<grammar->ruleArray[i]->rule[j]->size) && isepsilon;k++){
//                   if(k == 0){
//                       ispsilon = 0;
//                   }
//                   int sym = grammar->ruleArray[i]->rule[j]->symbols[k]->symbol;
//                   int type = grammar->ruleArray[i]->rule[j]->symbols[k]->isTerminal;
//                   if(type == 0){
//                    for(int l=0;l<f.first[sym].size;l++){
//                       if(f.first[sym]->symbols[l] == 55){
//                           isepsilon = 1;
//                       }
//                       parsetable.table[i][f.first[sym]->symbols[l]] = grammar->ruleArray[i]->rule[j];
//                   }
//                   }else{
//                       parsetable.table[i][sym] = grammar->ruleArray[i]->rule[j];
//                   }
                  
//               }
//           }
//       }
//       return parsetable;
// }

void printFirstArray(FirstFollowArray array){
    for(int i = 0; i < array.size; i++){
        printf("%s ", tokToStr(array.elements[i].t));
    }
    
}

FirstFollowArray tokenSetToArray(NonTerminal nt, TokenSet tSet){
    FirstFollowArray array;
    array.size = 0;
    array.elements = (FirstFollowElement*)malloc(TOKEN_SIZE * sizeof(FirstFollowElement));

    for(int i = 0; i < TOKEN_SIZE; i++){
        if(tokenSetContains(tSet, i)){
            array.elements[array.size++] = (FirstFollowElement){i, tokRuleNum[nt][i]};
        }
    }
    
    // printf("========");
    // printFirstArray(array);
    // printf("\n=========");
    return array;
}


void initFirstAndFollow(FirstAndFollow* firstNFollow, Grammar* grammar){
    firstNFollow->first = (FirstFollowArray*) malloc(NON_TERMINAL_SIZE * sizeof(FirstFollowArray));
    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        TokenSet tset = first(grammar, i);
        firstNFollow->first[i] = tokenSetToArray(i, tset);
    }
}

LocationArray* getLocationArray(){
    return ntLocation;
}