#include "parser.h"
#include "parserDef.h"
#include "token.h"

TokenSet firstSetDp[NON_TERMINAL_SIZE];

typedef struct{
    unsigned int bitMask : NON_TERMINAL_SIZE;
} NonTerminalSet;
NonTerminalSet nset;

typedef struct {
    NonTerminalSet ntSet;
    TokenSet tSet;
} FollowSet
int isNullable(NonTerminal nt){
    return (nset.bitMask >> nt) & 1 == 1;
}

void addNullable(NonTerminal nt){
    nset.bitMask |= (1 << nt); 
}
void addRule(Grammar* grammar, NonTerminal nt, Symbol* symbols, int size, int rNum){
    Rule r = {size, symbols};
    grammar->ruleSet[nt].rule[rNum] = r;
}

void initRuleSet(Grammar* grammar, NonTerminal nt, int size){
    grammar->ruleSet[nt].size = size;
    grammar->ruleSet[nt].rule = (Rule*)malloc(size * sizeof(Rule));
}

void initGrammar(Grammar* grammar){
    nset.bitMask = 0;
    int size = NON_TERMINAL_SIZE;

    grammar->size = size;
    grammar->ruleSets = (RuleSet*)malloc(size * sizeof(RuleSet));
    
    Symbol program[] = {{0, OTHER_FUNCTIONS}, {0, MAIN_FUNCTION}};
    initRuleSet(grammar, PROGRAM, 1);
    addRule(grammar, PROGRAM, program, 2, 0);

    Symbol mainFunction[] = {{1, TK_MAIN}, {0, STMTS}, {1, TK_END}};
    initRuleSet(grammar, MAIN_FUNCTION, 1);
    addRule(grammar, MAIN_FUNCTION, mainFunction, 3, 0);

    Symbol otherFunctions0[] = {{0, FUNCTION}, {0, OTHER_FUNCTIONS}};
    Symbol otherFunctions1[] = {{1, EPSILON}};
    initRuleSet(grammar, OTHER_FUNCTIONS, 2);
    addRule(grammar, OTHER_FUNCTIONS, otherFunctions0, 2, 0);
    addRule(grammar, OTHER_FUNCTIONS, otherFunctions1, 1, 1);
     

    // initialize nset at the end
}

TokenSet first(Grammar* grammar, NonTerminal nt){
    TokenSet tSet = nullTokenSet();

    if(firstSetDp[nt] != tSet)
        return firstSetDp[nt];
    else {
        for(int i = 0; i < grammar->ruleSet[nt].size; i++){
            for(int j = 0; j < grammar->ruleSet[nt][i].size; j++){
                Symbol s = grammar->ruleSet[nt][i][j];
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


TokenSet follow(Grammar* grammar, NonTerminal nt){

}