/* Created by Tau on 08/02/2019 */
#ifndef _HG_STATEMENT_H_
#define _HG_STATEMENT_H_

#include "expression.h"

typedef struct StatementListTag StatementList;
typedef struct StatementTag Statement;

typedef enum {
    EXPRESSION_STATEMENT,
    IF_STATEMENT,
    ELSE_STATEMENT,
    ELSEIF_STATEMENT,
    FOR_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    FUNCTION_DEFINE_STATEMENT,
    RETURN_STATEMENT,
} StatementType;


typedef struct IfStatementTag IfStatement;

typedef struct ElsIfStatementTag {
    StatementList *block;
    struct ElsIfStatementTag *next;
} ElsIfStatement;

IfStatement* initElsIfStatement(Expression* condition, StatementList* block);

struct IfStatementTag {
    Expression *condition;
    StatementList *block;
    ElsIfStatement *elsIfHead;
    ElsIfStatement* elsIfTail;
    StatementList* elseBlock;

    void (*addElsIf)(ElsIfStatement *);

    void (*addElse)(StatementList *);
};

IfStatement *initIfStatement(Expression *condition, StatementList *block);


struct StatementTag {
    StatementType type;
    union {
        Expression *expression;
        IfStatement *ifStatement;
    } s;
    Statement *next;
};

struct StatementListTag {
    Statement *head;
    Statement *tail;

    void (*addStatement)(StatementList *, Statement *);

    void (*execute)(StatementList *);

    void (*free)(StatementList *);
};

StatementList *initStatementList();


#endif /*_HG_STATEMENT_H_*/