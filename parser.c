#include "parser.h"
#include "parserDef.h"
#include "token.h"
#include <stdlib.h>

typedef struct {
    NonTermSet ntSet;
    TokenSet tSet;
} FollowHelperSet;

typedef struct {
    NonTerminal nt;
    int ruleNo;
    int index;
} Location;

typedef struct {
    int size;
    Location* location;
} LocationArray;

TokenSet firstSetDp[NON_TERMINAL_SIZE];
LocationArray ntLocation[NON_TERMINAL_SIZE];
NonTermSet nset;

NonTermSet nullNonTermSet(){
    return (NonTermSet){0};
}

NonTermSet singletonNonTermSet(NonTerminal nt){
    return (NonTermSet){1 << nt};
}

NonTermSet nonTermSetUnion(NonTermSet s1, NonTermSet s2){
    return (NonTermSet){s1.bitMask | s2.bitMask};
}

int equalsNonTermSet(NonTermSet s1, NonTermSet s2){
    return s1.bitMask == s2.bitMask;
}

int isNullable(NonTerminal nt){
    return (nset.bitMask >> nt) & 1 == 1;
}

void addNullable(NonTerminal nt){
    nset.bitMask |= (1 << nt); 
}

void updateNumOccur(Symbol* symbol, int n){
    for(int i = 0; i < n; i++){
        if(!symbol[i].isTerminal){
            ntLocation[symbol[i].symbol].size++;
        }
    }
}
void addRule(Grammar* grammar, NonTerminal nt, Symbol* symbols, int size, int rNum){
    Rule r = {size, symbols};
    grammar->ruleArray[nt].rule[rNum] = r;
    updateNumOccur(symbols, size);
}

void initRuleArray(Grammar* grammar, NonTerminal nt, int size){
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
    nset.bitMask = 0;
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
    initRuleArray(grammar, PRIMITIVE_DATATYPE,3);
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
    addRule(grammar, STMTS, stmts , 4, 0);

    Symbol typeDefinitions0[] = {{0, ACTUAL_OR_REDEFINED}, {0,TYPE_DEFINITIONS}};
    Symbol typeDefinitions1[] = {{1, EPSILON}};
    initRuleArray(grammar, TYPE_DEFINITIONS,2);
    addRule(grammar, TYPE_DEFINITIONS, typeDefinitions0, 2, 0);
    addRule(grammar, TYPE_DEFINITIONS, typeDefinitions1, 1, 1);

    Symbol actualOrRedefined0[] = {{0, TYPE_DEFINITION}};
    Symbol actualOrRedefined1[] = {{0,DEFINE_TYPE_STATEMENT}};
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
    initRuleArray(grammar, MORE_FIELDS,2);
    addRule(grammar, MORE_FIELDS, moreFields0, 2, 0);
    addRule(grammar, MORE_FIELDS, moreFields1, 1, 1);

    Symbol declarations0[] = {{0, DECLARATION}, {0,DECLARATIONS}};
    Symbol declarations1[] = {{1, EPSILON}};
    initRuleArray(grammar, MORE_FIELDS,2);
    addRule(grammar, DECLARATIONS, declarations0, 2, 0);
    addRule(grammar, DECLARATIONS, declarations1, 1, 1);

    Symbol declaration0[] = {{1,TK_TYPE}, {0, DATATYPE}, {1,TK_COLON}, {1,TK_ID}, {0,GLOBAL_OR_NOT}, {1,TK_SEM}};
    initRuleArray(grammar, DECLARATION,1);
    addRule(grammar, DECLARATION, declaration0 , 6, 0);

    Symbol global_or_not0[] = {{1,TK_COLON}, {1,TK_GLOBAL}};
    Symbol global_or_not1[] = {{1, EPSILON}};
    initRuleArray(grammar, GLOBAL_OR_NOT,2);
    addRule(grammar, MORE_FIELDS, global_or_not0, 2, 0);
    addRule(grammar, MORE_FIELDS, global_or_not1, 1, 1);

    Symbol stmt0[] = {{0, ASSIGNMENT_STMT}};
    Symbol stmt1[] = {{0, ITERATIVE_STMT}};
    Symbol stmt1[] = {{0, CONDITIONAL_STMT}};
    Symbol stmt1[] = {{0, IO_STMT}};
    Symbol stmt1[] = {{0, FUN_CALL_STMT}};
    initRuleArray(grammar, MORE_FIELDS,5);
    addRule(grammar, OTHER_STMTS, otherStmts0, 1, 0);
    addRule(grammar, OTHER_STMTS, otherStmts1, 1, 1);
    addRule(grammar, OTHER_STMTS, otherStmts0, 1, 2);
    addRule(grammar, OTHER_STMTS, otherStmts1, 1, 3);
    addRule(grammar, OTHER_STMTS, otherStmts1, 1, 4);

    Symbol assignmentStmt0[] = {{0, SINGLE_OR_REC_ID}, {1,TK_ASSIGNOP}, {0,ARITHMETIC_EXPRESSION}, {1,TK_SEM}};
    initRuleArray(grammar, ASSIGNMENT_STMT,1);
    addRule(grammar, ASSIGNMENT_STMT, assignmentStmt0 , 4, 0);

    Symbol singleOrRecId0[] = {{1,TK_ID},{0, OPTION_SINGLE_CONSTRUCTED}};
    initRuleArray(grammar, SINGLE_OR_REC_ID,1);
    addRule(grammar, SINGLE_OR_REC_ID, singleOrRecId0, 2, 0);

    Symbol option_single_constructed0[] = {{0,ONE_EXPANSION}, {0,MORE_EXPANSIONS}};
    Symbol option_single_constructed1[] = {{1, EPSILON}};
    initRuleArray(grammar, GLOBAL_OR_NOT,2);
    addRule(grammar, OPTION_SINGLE_CONSTRUCTED, option_single_constructed0, 2, 0);
    addRule(grammar, OPTION_SINGLE_CONSTRUCTED, option_single_constructed1, 1, 1);

    Symbol oneExpansion0[] = {{1,TK_ID},{1, TK_FIELDID}};
    initRuleArray(grammar, ONE_EXPANSION,1);
    addRule(grammar, ONE_EXPANSION, oneExpansion0, 2, 0);

    Symbol moreExpansions0[] = {{0,ONE_EXPANSION}, {0,MORE_EXPANSIONS}};
    Symbol moreExpansions1[] = {{1, EPSILON}};
    initRuleArray(grammar, GLOBAL_OR_NOT,2);
    addRule(grammar, MORE_EXPANSIONS, moreExpansions0, 2, 0);
    addRule(grammar, MORE_EXPANSIONS, moreExpansions1, 1, 1);

    Symbol funCallStmt[] = {{1,TK_CALL},{1, TK_FUNID}, {1, TK_WITH}, {1, TK_PARAMETERS}, {0,INPUT_PARAMETERS}, {1, TK_SEM} };
    initRuleArray(grammar, FUN_CALL_STMT,1);
    addRule(grammar, ONE_EXPANSION, oneExpansion0, 6, 0);

    Symbol outputParameters0[] = {{1,TK_SQL}{0,ID_LIST}, {1,TK_SQR}, {1,TK_ASSIGNOP}};
    Symbol outputParameters1[] = {{1, EPSILON}};
    initRuleArray(grammar, GLOBAL_OR_NOT,2);
    addRule(grammar, OUTPUT_PARAMETERS, outputParameters0, 4, 0);
    addRule(grammar, OUTPUT_PARAMETERS, outputParameters1, 1, 1);

    Symbol inputParameters0[] = {{1,TK_SQL}{0,ID_LIST}, {1,TK_SQR}};
    initRuleArray(grammar, GLOBAL_OR_NOT,1);
    addRule(grammar, INPUT_PARAMETERS, inputParameters0, 3, 0);

    Symbol iterativeStmt0[] = {{1,TK_WHILE}, {1,TK_OP},{0,BOOLEAN_EXPRESSION}, {1,TK_CL}, {0,STMT}, {0,OTHER_STMTS}, {1,TK_ENDWHILE}};
    initRuleArray(grammar, ITERATIVE_STMT,1);
    addRule(grammar, ITERATIVE_STMT, iterativeStmt0, 7, 0);

    

    

    


    










    

















    
    


    
    
    
    
    
    

    
    
    


    // initialize nset at the end
    initLocations(grammar);
}

TokenSet first(Grammar* grammar, NonTerminal nt){
    TokenSet tSet = nullTokenSet();

    if(equalsTokenSet(firstSetDp[nt],nullTokenSet()))
        return firstSetDp[nt];
    else {
        for(int i = 0; i < grammar->ruleArray[nt].size; i++){
            for(int j = 0; j < grammar->ruleArray[nt].rule[i].size; j++){
                Symbol s = grammar->ruleArray[nt].rule[i].symbol[j];
                if(s.isTerminal){
                    tokenSetUnion(tSet, singletonTokenSet(s.symbol));
                    firstSetDp[nt] = tSet;
                    return tSet;
                }
                else if(!s.isTerminal){
                    tSet = tokenSetUnion(tSet, first(grammar, s.symbol));
                    if(!isNullable(s.symbol))
                        return tSet;
                }
            }
        }
    }

    return tSet;
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
                break;
            }
            else{
                firstDep = tokenSetUnion(firstDep, first(grammar, s.symbol));
                if(!isNullable(s.symbol))
                    break;
            }
        }
        Symbol lastSymbol = grammar->ruleArray[l.nt].rule[l.ruleNo].symbol[n - 1];
        if(l.index == n && !(lastSymbol.isTerminal) && isNullable(lastSymbol.symbol)){
            followDep = nonTermSetUnion(followDep, singletonNonTermSet(l.nt));
        }
    }

    return (FollowHelperSet){followDep, firstDep};
}


ParseTable initParseTable(Grammar* grammar,FirstAndFollow* f){
      
      struct ParseTable parsetable;
      for(int i=0;i<grammar->size;i++){
          for(int j=0;j<grammar->ruleArray->size;j++){
              int isepsilon = 1;
              for(int k=0;(k<grammar->ruleArray[i]->rule[j]->size) && isepsilon;k++){
                  if(k == 0){
                      ispsilon = 0;
                  }
                  int sym = grammar->ruleArray[i]->rule[j]->symbols[k]->symbol;
                  int type = grammar->ruleArray[i]->rule[j]->symbols[k]->isTerminal;
                  if(type == 0){
                   for(int l=0;l<f.first[sym].size;l++){
                      if(f.first[sym]->symbols[l] == 55){
                          isepsilon = 1;
                      }
                      parsetable.table[i][f.first[sym]->symbols[l]] = grammar->ruleArray[i]->rule[j];
                  }
                  }else{
                      parsetable.table[i][sym] = grammar->ruleArray[i]->rule[j];
                  }
                  
              }
          }
      }
      return parsetable;
}

