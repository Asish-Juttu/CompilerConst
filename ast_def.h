#ifndef _ast_def_
#define _ast_def_

#include "type.h"

#define declareAstNode(VARNAME, ENUM, STRUCT, FIELD) \
    AstNode *VARNAME = makeEmptyAstNode();           \
    VARNAME->type = ENUM;                            \
    VARNAME->node.FIELD = malloc(sizeof(STRUCT)); 

#define nodeToAst(VAR, FIELD) (VAR == NULL ? (printf("%s is null.\n", #VAR) ? VAR->node.FIELD : VAR->node.FIELD) :\
    (toAstType(#FIELD) == VAR->type) ? VAR->node.FIELD : (printf("%s being accessed as %s\n [nodeToAst(%s, %s)]\n", astToStr(VAR->type), \
    astToStr(toAstType(#FIELD)), #VAR, #FIELD) ? VAR->node.FIELD : VAR->node.FIELD))
#define boolOpBexp(VAR) VAR->bexp.boolOp
#define varCompBexp(VAR) VAR->bexp.varComp
#define numVar(VAR) VAR->varUnion.num
#define rnumVar(VAR) VAR->varUnion.rnum
#define varVar(VAR) VAR->varUnion.singleOrRecId
#define tdef(VAR) VAR->tdefUnion.tdef
#define trdef(VAR) VAR->tdefUnion.trdef
#define assignStmt(VAR) VAR->stmtUnion.assignStmt
#define iterStmt(VAR) VAR->stmtUnion.iterStmt
#define fCallStmt(VAR) VAR->stmtUnion.funCallStmt
#define condStmt(VAR) VAR->stmtUnion.condStmt
#define ipopStmt(VAR) VAR->stmtUnion.ioStmt
#define expAexp(VAR) VAR->aexpUnion.exp
#define varAexp(VAR) VAR->aexpUnion.var

typedef enum {
    AST_PROGRAM, AST_OTHERFUNCTIONS, AST_MAIN, AST_FUNCTION, AST_PARAMETERLIST, AST_CONSTRUCTEDDATATYPE, AST_STMTS, AST_TYPEDEFINITION,
    AST_FIELDDEFINITIONS, AST_FIELDDEFINITION, AST_DECLARATION, AST_ASSIGNMENTSTMT,
    AST_SINGLEORRECID, AST_FUNCALLSTMT, AST_IDLIST, AST_ITERATIVESTMT,
    AST_CONDITIONALSTMT, AST_NOT, AST_NUM, AST_RNUM, AST_AND, AST_OR, AST_LT, AST_LE, AST_EQ,
    AST_GT, AST_GE, AST_NE, AST_DEFINETYPESTMT, AST_MOREEXPANSION,
    AST_OPTIONSINGLECONSTRUCTED, AST_A, AST_ELSEPART, AST_FACTOR,
    AST_PLUS, AST_MINUS, AST_MUL, AST_DIV, AST_OTHERSTMTS, AST_DATATYPE, AST_PARAMETERDECL, AST_PRIMITIVEDT, AST_CONSTRDT,
    AST_ARITHMETICEXPR, AST_VAR, AST_IOSTMT, AST_BOOLEXP, AST_LOGICALOP, AST_ARITHMETICOP,
    AST_RELATIONALOP, AST_ID, AST_TYPEDEFINITIONS, AST_DECLARATIONS, AST_UNKNOWN, AST_ISGLOBALORNOT,
    AST_STMT
} AstNodeType;

typedef enum{
    AOP_PLUS, AOP_MINUS, AOP_MUL, AOP_DIV
} ArithmeticOperator;

typedef enum {
    BEXP_VAR_COMP, BEXP_BOOL_OP
} BoolExpType;

typedef enum{
    LOP_AND, LOP_OR, LOP_NOT
} LogicalOperator;

typedef enum {
    ROP_Lt, ROP_Le, ROP_Eq, ROP_Gt, ROP_Ge, ROP_Ne
} RelationalOperator;

typedef enum{
    IO_READ, IO_WRITE
} IoType;

typedef enum {
    VAR_ID, VAR_NUM, VAR_RNUM
} VarType;

//////////////////////////////////////////////////////////////////
struct _AstNode;

typedef struct{
    int size;
    int cap;
    struct _AstNode** nodes;
} AstList;

AstList* createAstList();
void insertTo(AstList* list, struct _AstNode* node);

///////////////////////////////////////////////////////////////////

typedef struct {
    AstList* typeDefintionList;
    TypeExpression typeExpr;
    int lineNo;
} Ast_TypeDefinitions;

typedef struct {
    AstList* otherStmtList; 
    TypeExpression typeExpr;
    int lineNo;
} Ast_OtherStmts;

typedef struct {
    AstList* declarationList;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Declarations;

typedef struct {
    AstList* idList;
    TypeExpression typeExpr;
    int lineNo;
} Ast_IdList;

typedef struct {
    AstList* functionList;
    TypeExpression typeExpr;
    int lineNo;
} Ast_OtherFunctions;

typedef struct {
    AstList* parameterList;
    TypeExpression typeExpr;
    int lineNo;
} Ast_ParameterList;

typedef struct {
    AstList* fieldDefinitionList;
    TypeExpression typeExpr;
    int lineNo;
} Ast_FieldDefinitions;

typedef struct {
    char* id;
    AstList* fieldNameList;
    TypeExpression typeExpr;
    int lineNo;
} Ast_SingleOrRecId;

typedef struct {
    AstList* fieldNameList;
    TypeExpression typeExpr;
    int lineNo;
} Ast_OptionSingleConstructed;

////////////////////////////////////////////////////////////////////////

typedef struct {
    char* name;
    Datatype datatype;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Datatype;

typedef struct {
    Ast_Datatype* fieldType;
    char* id;
    TypeExpression typeExpr;
    int lineNo;
} Ast_FieldDefinition;

typedef struct {
    Datatype datatype;
    char* idOld;
    char* idNew;
    TypeExpression typeExpr;
    int lineNo;
} Ast_DefineTypeStmt;

typedef struct {
    Datatype datatype;
    TypeExpression typeExpr;
    int lineNo;
} Ast_A;

typedef struct {
    int val;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Num;

typedef struct {
    float val;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Rnum;

typedef struct {
    Ast_Datatype* datatype;
    char* id;
    int isGlobal;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Declaration;

/////////////////////////////////////////////////////////////////////////
typedef union {
    Ast_Num* num;
    Ast_Rnum* rnum;
    Ast_SingleOrRecId* singleOrRecId;
} Var;

typedef struct {
    Var varUnion;
    VarType varType;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Var;

typedef struct {
    Ast_TypeDefinitions* typeDefinitions;
    Ast_Declarations* declarations;
    Ast_OtherStmts* otherStmts;
    Ast_IdList* returnIds;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Stmts;

typedef struct {
    Ast_Stmts* stmts;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Main;

typedef struct {
    Ast_OtherFunctions* otherFunctions;
    Ast_Main* mainFunction;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Program;

typedef struct {
    char* funId;
    Ast_ParameterList* input_par;
    Ast_ParameterList* output_par;
    Ast_Stmts* stmts;
    Ast_IdList* retIdList;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Function;

typedef struct {
    Ast_Datatype* datatype;
    char* id;
    TypeExpression typeExpr;
    int lineNo;
} Ast_ParameterDeclaration;

typedef struct {
    Ast_Datatype* datatype;
    TypeExpression typeExpr;
    int lineNo;
} Ast_PrimitiveDatatype;

typedef struct{
    int isGlobal;
} Ast_IsGlobalOrNot;

typedef struct {
    Ast_Datatype* datatype;
    TypeExpression typeExpr;
    int lineNo;
} Ast_ConstructedDatatype;

typedef enum{
    TDEF, TRDEF
} TypeDefType;

typedef struct{
    char* id;
    Datatype datatype;
    Ast_FieldDefinitions* fieldDefinitions;
} TypeDef;

typedef struct {
    char*  from;
    char* to;
} TypeRdef;

typedef union{
    TypeDef* tdef;
    TypeRdef* trdef;
} TypeDefinitionUnion;

typedef struct {
    TypeDefinitionUnion tdefUnion;
    TypeDefType type;
    TypeExpression typeExpr;
    int lineNo;
} Ast_TypeDefinition;

typedef enum {
    AEXP_EXP, AEXP_VAR
} AexpType;

typedef struct{
    struct _Ast_ArithmeticExpression* left;
    struct _Ast_ArithmeticExpression* right;
    ArithmeticOperator op;
} ArithmeticExpression;

typedef union {
    ArithmeticExpression* exp;
    Ast_Var* var;
} AexpUnion;

typedef struct _Ast_ArithmeticExpression{
    AexpUnion aexpUnion;
    AexpType aexpType;
    TypeExpression typeExpr;
    
    int lineNo;
} Ast_ArithmeticExpression;

typedef struct {
    Ast_SingleOrRecId* singleOrRecId;
    Ast_ArithmeticExpression* expression;
    TypeExpression typeExpr;
    int lineNo;
} Ast_AssignmentStmt;

typedef struct {
    Ast_IdList* outputIdList;
    Ast_IdList* inputIdList;
    char* funId;
    TypeExpression typeExpr;
    int lineNo;
} Ast_FunCallStmt;

///////////////////////////////////////////////////////

struct _astBexp;
typedef struct _astBexp Ast_BooleanExpression;

typedef struct {
    Ast_Var* left;
    Ast_Var* right;
    RelationalOperator op;
} VarComparison;

typedef struct {
    Ast_BooleanExpression* left;
    Ast_BooleanExpression* right;
    LogicalOperator op;
    TypeExpression typeExpr;
    int lineNo;
} BoolOperation;

typedef union {
    VarComparison* varComp;
    BoolOperation* boolOp;
} Bexp;

struct _astBexp{
    Bexp bexp;
    BoolExpType bexpType;
};

///////////////////////////////////////////////////////

typedef struct {
    Ast_BooleanExpression* condition;
    Ast_OtherStmts* body;
    TypeExpression typeExpr;
    int lineNo;
} Ast_IterativeStmt;

typedef struct {
    Ast_BooleanExpression* condition;
    Ast_OtherStmts* body;
    Ast_OtherStmts* elsePart;
    TypeExpression typeExpr;
    int lineNo;
} Ast_ConditionalStmt;

typedef struct {
    IoType ioType;
    Ast_Var* var;
    TypeExpression typeExpr;
    int lineNo;
} Ast_IoStmt;

typedef struct {
    LogicalOperator op;
} Ast_LogicalOperator;

typedef struct {
    RelationalOperator op;
} Ast_RelationalOperator;

typedef enum {
    STMT_ASSIGN, STMT_COND, STMT_ITER, STMT_FUN_CALL, STMT_IO
} StmtType;

typedef union {
    Ast_AssignmentStmt* assignStmt;
    Ast_ConditionalStmt* condStmt;
    Ast_IterativeStmt* iterStmt;
    Ast_IoStmt* ioStmt;
    Ast_FunCallStmt* funCallStmt;
} StmtUnion;

typedef struct {
    StmtUnion stmtUnion;
    StmtType type;
} Ast_Stmt;

typedef struct {
    ArithmeticOperator op;
} Ast_ArithmeticOperator;

typedef struct {
    AstList* oneExpansionList;
    TypeExpression typeExpr;
} Ast_MoreExpansion;

typedef struct {
    Ast_OtherStmts* otherStmts;
    TypeExpression typeExpr;
    int lineNo;
} Ast_ElsePart;

typedef struct {
    char* id;
} Ast_Id;

typedef union { 
    Ast_Id* id;
    Ast_TypeDefinitions* typeDefinitions;
    Ast_OtherStmts* otherStmts;
    Ast_Declarations* declarations;
    Ast_IdList* idList;
    Ast_OtherFunctions* otherFunctions;
    Ast_ParameterList* parameterList;
    Ast_FieldDefinitions* fieldDefinitions;
    Ast_SingleOrRecId *singleOrRecId;
    Ast_OptionSingleConstructed* optionSingleConstructed;
    Ast_Datatype* datatype;
    Ast_FieldDefinition* fieldDefinition;
    Ast_DefineTypeStmt* defineTypeStmt;
    Ast_A* a;
    Ast_Num* num;
    Ast_Rnum* rnum;
    Ast_Declaration* declaration;
    Ast_Stmts* stmts;
    Ast_Main* mainFunction;
    Ast_Program* program;
    Ast_Function* function;
    Ast_ParameterDeclaration* parameterDeclaration;
    Ast_PrimitiveDatatype* primitiveDatatype;
    Ast_ConstructedDatatype* constructedDatatype;
    Ast_TypeDefinition* typeDefinition;
    Ast_ArithmeticExpression* arithmeticExpression;
    Ast_BooleanExpression* booleanExpression;
    Ast_AssignmentStmt* assignmentStmt;
    Ast_FunCallStmt* funCallStmt;
    Ast_Var* var;
    Ast_IterativeStmt* iterativeStmt;
    Ast_ConditionalStmt* conditionalStmt;
    Ast_IoStmt* ioStmt;
    Ast_ElsePart* elsePart;
    Ast_LogicalOperator* logicalOp;
    Ast_RelationalOperator* relationalOp;
    Ast_ArithmeticOperator* arithmeticOp;
    Ast_MoreExpansion* moreExp;
    Ast_IsGlobalOrNot* isGlobalOrNot;
    Ast_Stmt* stmt;
} AstNodeUnion;

AstNodeType toAstType(char* name);
typedef struct _AstNode{
    AstNodeUnion node;
    AstNodeType type;
} AstNode;

int isTypeRedefinition(Ast_TypeDefinition* tdef);
char* astToStr(AstNodeType t);
#endif