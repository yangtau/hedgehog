/* Created by Tau on 08/02/2019 */
#ifndef _HG_STATEMENT_H_
#define _HG_STATEMENT_H_

#include "environment.h"
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

typedef enum {
    BREAK_RESULT,
    CONTINUE_RESULT,
    RETURN_RESULT,
    NORMAL_RESULT,
    ELSE_IF_EXE_RESULT,  // else_if statement execute
    ELSE_IF_NE_RESULT,   // else_if statement not execute
} StatementResult;

typedef struct StatementListTag StatementList;
typedef struct StatementTag Statement;
typedef struct IfStatementTag IfStatement;
typedef struct ElsIfStatementTag ElsIfStatement;
typedef struct ForStatementTag ForStatement;

struct ElsIfStatementTag {
    Expression *condition;
    StatementList *block;
    struct ElsIfStatementTag *next;
};

struct IfStatementTag {
    // Environment *env;
    Expression *condition;
    StatementList *block;
    ElsIfStatement *elsIfHead;
    ElsIfStatement *elsIfTail;
    StatementList *elseBlock;

    void (*addElsIf)(IfStatement *ifS,
                     Expression *condition,
                     StatementList *block);

    void (*addElse)(IfStatement *ifS, StatementList *elseBlock);
};

struct ForStatementTag {
    // Environment *env;
    Expression *initial;  // assign
    Expression *condition;
    Expression *post;
    StatementList *block;
};

struct StatementTag {
    StatementType type;
    union {
        Expression *expression;
        IfStatement *ifStatement;
        ForStatement *forStatement;
        FunctionDefine  *func;
    } s;
    Statement *next;  
};

struct StatementListTag {
    Statement *head;
    Statement *tail;

    void (*add)(StatementList *, Statement *);

    StatementResult (*execute)(StatementList *, Environment *);

    void (*free)(StatementList *);
};

Statement *initExpressionStatement(Expression *expression);

Statement *initIfStatement(Expression *condition, StatementList *block);

Statement *initForStatement(Expression *initial,
                            Expression *condition,
                            Expression *post,
                            StatementList *block);

Statement *initBreakStatement();

Statement *initContinueStatement();

Statement *initReturnStatement();

StatementList *initStatementList();

#endif /*_HG_STATEMENT_H_*/