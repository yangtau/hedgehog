#include "statement.h"
#include <stdlib.h>
#include "debug.h"
#include "interpreter.h"

static void executeStatementList(StatementList*);

static void freeStatementList(StatementList* list);

static Statement* initStatement();

static void freeStatement(Statement* s);

static void executeStatement(Statement* s);

/*expression statement start*/

static void executeExpressionStatement(Expression* expression) {
    evaluateExpression(expression);
}

static void freeExpressionStatement(Expression* expression) {
    log("free expression type: %d", expression->type);
    return freeExpression(expression);
}

Statement* initExpressionStatement(Expression* expression) {
    Statement* s = initStatement();
    s->type = EXPRESSION_STATEMENT;
    s->s.expression = expression;
    return s;
}
/*expression statement end*/

/*if else statement start*/

static void addElseToIf(IfStatement* ifS, StatementList* elseBlock) {
    ifS->elseBlock = elseBlock;
}

static void addElsIfToIf(IfStatement* ifS,
                         Expression* condition,
                         StatementList* block) {
    ElsIfStatement* elsIfS = (ElsIfStatement*)malloc(sizeof(ElsIfStatement));
    elsIfS->block = block;
    elsIfS->condition = condition;
    elsIfS->next = NULL;
    ifS->elsIfTail->next = elsIfS;
    ifS->elsIfTail = elsIfS;
}

// return 1 else_if executed
static int executeElsIfStatement(ElsIfStatement* s) {
    Value condition = evaluateExpression(s->condition);
    if (condition.type != BOOL_VALUE) {
        panic("%s", "condition expression is supposed to yield boolean value");
        // TODO: error handler
    }
    if (condition.v.bool_value.v) {
        executeStatementList(s->block);
        return 1;
    } else if (s->next != NULL) {
        return executeElsIfStatement(s->next);
    }
    return 0;
}

static void executeIfStatement(IfStatement* s) {
    Value condition = evaluateExpression(s->condition);
    if (condition.type != BOOL_VALUE) {
        panic("%s", "condition expression is supposed to yield boolean value");
        // TODO: error handler
    }
    if (condition.v.bool_value.v) {
        executeStatementList(s->block);
    } else {
        int flag = 0;
        if (s->elsIfHead->next != NULL) {
            flag = executeElsIfStatement(s->elsIfHead->next);
        }
        if (flag == 0 && s->elseBlock != NULL)
            executeStatementList(s->elseBlock);
    }
}

static void freeElsifStatement(ElsIfStatement* elsifS) {
    freeExpression(elsifS->condition);
    freeStatementList(elsifS->block);
}

static void freeIfStatement(IfStatement* ifS) {
    log("free if statement %s", "");
    freeExpression(ifS->condition);
    if (ifS->elseBlock != NULL)
        freeStatementList(ifS->elseBlock);
    freeStatementList(ifS->block);
    ElsIfStatement* elsIfS = ifS->elsIfHead->next;
    while (elsIfS != NULL) {
        ElsIfStatement* p = elsIfS->next;
        freeElsifStatement(elsIfS);
        elsIfS = p;
    }
    free(ifS->elsIfHead);
    free(ifS);
}

Statement* initIfStatement(Expression* condition, StatementList* block) {
    Statement* s = initStatement();
    s->type = IF_STATEMENT;
    IfStatement* ifS = (IfStatement*)malloc(sizeof(IfStatement));
    ifS->condition = condition;
    ifS->block = block;
    ifS->elseBlock = NULL;
    ifS->elsIfHead = ifS->elsIfTail =
        (ElsIfStatement*)malloc(sizeof(ElsIfStatement));
    ifS->elsIfHead->next = NULL;
    ifS->addElse = addElseToIf;
    ifS->addElsIf = addElsIfToIf;
    s->s.ifStatement = ifS;
    return s;
}

/*if else statement end*/

/*statement list start*/
static void addStatementToList(StatementList* list, Statement* s) {
    list->tail->next = s;
    list->tail = s;
}

static void executeStatementList(StatementList* list) {
    Statement* p = list->head->next;
    while (p != NULL) {
        executeStatement(p);
        p = p->next;
    }
}

static void freeStatementList(StatementList* list) {
    Statement* p = list->head->next;
    log("free statement list%s", "");
    while (p != NULL) {
        Statement* q = p->next;
        freeStatement(p);
        p = q;
    }
    free(list->head);
}

StatementList* initStatementList() {
    StatementList* list = (StatementList*)malloc(sizeof(StatementList));
    list->tail = list->head = initStatement();
    list->head->next = NULL;
    list->free = freeStatementList;
    list->add = addStatementToList;
    list->execute = executeStatementList;
    return list;
}
/*statement list end*/

/*statement start*/
static Statement* initStatement() {
    Statement* s = (Statement*)malloc(sizeof(Statement));
    s->next = NULL;
    return s;
}

static void freeStatement(Statement* s) {
    log("type %d", s->type);
    switch (s->type) {
        case EXPRESSION_STATEMENT:
            freeExpressionStatement(s->s.expression);
            break;
        case IF_STATEMENT:
            freeIfStatement(s->s.ifStatement);
            break;
        default:
            panic("%s", "bad case...");
    }
    free(s);
}

static void executeStatement(Statement* s) {
    switch (s->type) {
        case EXPRESSION_STATEMENT:
            return executeExpressionStatement(s->s.expression);
        case IF_STATEMENT:
            return executeIfStatement(s->s.ifStatement);
        default:
            panic("%s", "bad case...");
    }
}

/*statement end*/
