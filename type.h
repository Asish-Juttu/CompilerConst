#ifndef _TYPE_
#define _TYPE_


typedef enum{
    BTYPE_RNUM, BTYPE_NUM, BTYPE_UNION, BTYPE_RECORD, BTYPE_PARAM_LIST, BTYPE_FIELD, BTYPE_FUNCTION, BTYPE_ERROR, BTYPE_VOID, BTYPE_TAGGED_UNION,
    BTYPE_VOID
} BasicType;

struct _TypeExpression;

typedef struct {
    struct _TypeExpression* typeExpressionList;
    int size;
    int cap;
} ExpressionList;

typedef struct _TypeExpression{
    BasicType basicType;
    char* name; //what is this name?
    ExpressionList* expList;
} TypeExpression;


TypeExpression functionTypeExpression();
TypeExpression unionTypeExpression();
TypeExpression rnumTypeExpression();
TypeExpression numTypeExpression();
TypeExpression recordTypeExpression();
TypeExpression paramListTypeExpression();

void addToUnion(TypeExpression unionType, TypeExpression t);
void addToFuncInput(TypeExpression functionType, TypeExpression t);
void addToFuncOutput(TypeExpression funcitonType, TypeExpression t);
void addToRecord(TypeExpression recordType, TypeExpression t);

void setRecordName(char* name);

TypeExpression getOutputType(TypeExpression functionType);
TypeExpression getInputType(TypeExpression functionType);

extern const TypeExpression typeError;
extern const TypeExpression typeVoid;


char* basicTypeToString(BasicType bt);
#endif