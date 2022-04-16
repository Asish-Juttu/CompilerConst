#ifndef _TYPE_DEF_
#define _TYPE_DEF_

typedef enum{
    BTYPE_RNUM, BTYPE_NUM, BTYPE_UNION, BTYPE_RECORD, BTYPE_PARAM_LIST, BTYPE_FIELD, BTYPE_FUNCTION, BTYPE_ERROR, BTYPE_VOID, BTYPE_TAGGED_UNION
} BasicType;

struct _TypeExpression;

typedef enum{
    DT_PRIMITIVE, DT_UNION, DT_RECORD, DT_NUM, DT_RNUM, DT_NONE, DT_REC_OR_UNION
} Datatype;

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

char* dtypeToStr(Datatype t);
#endif