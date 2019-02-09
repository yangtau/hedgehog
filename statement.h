/* Created by Tau on 08/02/2019 */
#ifndef _HG_STATEMENT_H_
#define _HG_STATEMENT_H_

#include "expression.h"

typedef enum {
    EXPRESSION_STATEMENT,
    IF_STATEMENT,
    FOR_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    FUNCTION_DEFINE_STATEMENT,
    RETURN_STATEMENT,
} StatementType;

typedef struct StatementListTag StatementList;
typedef struct StatementTag Statement;
typedef struct IfStatementTag IfStatement;
typedef struct ElsIfStatementTag ElsIfStatement;

struct ElsIfStatementTag {
    Expression *condition;
    StatementList *block;
    struct ElsIfStatementTag *next;
};

struct StatementTag {
    StatementType type;
    union {
        Expression *expression;
        IfStatement *ifStatement;
    } s;
    Statement *next;
};

struct IfStatementTag {
    Expression *condition;
    StatementList *block;
    ElsIfStatement *elsIfHead;
    ElsIfStatement *elsIfTail;
    StatementList *elseBlock;

    void (*addElsIf)(IfStatement *ifS, Expression *condition, StatementList *block);

    void (*addElse)(IfStatement *ifS, StatementList *elseBlock);
};

struct StatementListTag {
    Statement *head;
    Statement *tail;

    void (*add)(StatementList *, Statement *);

    void (*execute)(StatementList *);

    void (*free)(StatementList *);
};

Statement *initExpressionStatement(Expression *expression);

Statement *initIfStatement(Expression *condition, StatementList *block);

StatementList *initStatementList();

#endif /*_HG_STATEMENT_H_*/