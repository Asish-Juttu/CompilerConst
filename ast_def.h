#ifndef _ast_def_
#define _ast_def_

#include "type.h"

#define declareAstNode(VARNAME, ENUM, STRUCT, FIELD) \
    AstNode *VARNAME = makeEmptyAstNode();           \
    VARNAME->type = ENUM;                            \
    VARNAME->node.FIELD = malloc(sizeof(STRUCT));

#define nodeToAst(VAR, FIELD) VAR->node.FIELD
#define boolOpBexp(VAR) VAR->bexp.boolOp
#define varCompBexp(VAR) VAR->bexp.varComp

typedef enum {
    AST_PROGRAM, AST_OTHERFUNCTIONS, AST_MAIN, AST_FUNCTION, AST_PARAMETERLIST, AST_CONSTRUCTEDDATATYPE, AST_STMTS, AST_TYPEDEFINITION,
    AST_FIELDDEFINITIONS, AST_FIELDDEFINITION, AST_DECLARATION, AST_ASSIGNMENTSTMT,
    AST_SINGLEORRECID, AST_FUNCALLSTMT, AST_IDLIST, AST_ITERATIVESTMT,
    AST_CONDITIONALSTMT, AST_NOT, AST_NUM, AST_RNUM, AST_AND, AST_OR, AST_LT, AST_LE, AST_EQ,
    AST_GT, AST_GE, AST_NE, AST_DEFINETYPESTMT, AST_MOREEXPANSION,
    AST_OPTIONSINGLECONSTRUCTED, AST_A, AST_ELSEPART, AST_FACTOR,
    AST_PLUS, AST_MINUS, AST_MUL, AST_DIV, AST_OTHERSTMTS, AST_DATATYPE, AST_PARAMETERDECL, AST_PRIMITIVEDT, AST_CONSTRDT,
    AST_ARITHMETICEXPR, AST_VAR, AST_IOSTMT, AST_BOOLEXP, AST_LOGICALOP, AST_ARITHMETICOP,
    AST_RELATIONALOP
} AstNodeType;

typedef enum{
    AOP_PLUS, AOP_MINUS, AOP_MUL, AOP_DIV
} ArithmeticOperator;

typedef enum{
    DT_PRIMITIVE, DT_UNION, DT_RECORD
} Datatype;

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
    TypeExpression typeExpr;
    int lineNo;
} Ast_Datatype;

typedef struct {
    Datatype fieldType;
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
    Datatype type;
    char* id;
    int isGlobal;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Declaration;

/////////////////////////////////////////////////////////////////////////

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

typedef struct {
    Ast_Datatype* datatype;
    TypeExpression typeExpr;
    int lineNo;
} Ast_ConstructedDatatype;

typedef struct {
    char* id1; char* id2;
    Datatype datatype;
    Ast_FieldDefinitions* fieldDefinitions;
    TypeExpression typeExpr;
    int lineNo;
} Ast_TypeDefinition;

typedef struct _Ast_ArithmeticExpression{
    struct _Ast_ArithmeticExpression* left;
    struct _Ast_ArithmeticExpression* right;
    ArithmeticOperator op;
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

typedef union {
    Ast_Num* num;
    Ast_Rnum* rnum;
    Ast_SingleOrRecId* vid;
} Var;

typedef struct {
    Var varUnion;
    VarType varType;
    TypeExpression typeExpr;
    int lineNo;
} Ast_Var;

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

typedef struct {
    ArithmeticOperator op;
} Ast_ArithmeticOperator;

typedef struct {
    Ast_OtherStmts* otherStmts;
    TypeExpression typeExpr;
    int lineNo;
} Ast_ElsePart;

typedef union { 
    Ast_TypeDefinitions* typedefinitions;
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
    Ast_ArithmeticExpression* arithmeticExporession;
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
} AstNodeUnion;

AstNodeType toAstType(char* name);
typedef struct _AstNode{
    AstNodeUnion node;
    AstNodeType type;
} AstNode;

#endif