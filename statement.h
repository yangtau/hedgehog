/* Created by Tau on 08/02/2019 */
#ifndef _HG_STATEMENT_H_
#define _HG_STATEMENT_H_

#include "environment.h"
#include "expression.h"

typedef struct StatementListTag StatementList;
typedef struct StatementTag Statement;
typedef struct IfStatementTag IfStatement;
typedef struct ElsIfStatementTag ElsIfStatement;
typedef struct ForStatementTag ForStatement;

typedef enum {
    BREAK_RESULT,
    CONTINUE_RESULT,
    RETURN_RESULT,
    NORMAL_RESULT,
    ELSE_IF_EXE_RESULT,  // else_if statement execute
    ELSE_IF_NE_RESULT,   // else_if statement not execute
} StatementResultType;

typedef struct {
    StatementResultType type;
    Value returnValue;
} StatementResult;


struct StatementTag {
    StatementResult (*execute)(Statement *self, Environment *env);

    void (*free)(Statement *self);

    Statement *next;
};


struct IfStatementTag {
    Statement base;
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

    StatementList *(*add)(StatementList *self, Statement *s);

    StatementResult (*execute)(StatementList *self, Environment *env);

    void (*free)(StatementList *self);
};

void *initExpressionStatement(Expression *expression);

void *initIfStatement(Expression *condition, StatementList *block);

void *initForStatement(Expression *initial, Expression *condition, Expression *post, StatementList *block);

void *initBreakStatement();

void *initContinueStatement();

void *initReturnStatement(Expression *exp);

void *initFunctionDefineStatement(String *id, ParameterList *paras, StatementList *block);

StatementList *initStatementList();

#endif /*_HG_STATEMENT_H_*/