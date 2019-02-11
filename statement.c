/* Created by Tau on 08/02/2019 */

#include "statement.h"
#include <stdlib.h>
#include "debug.h"
#include "interpreter.h"

static StatementResult executeStatementList(StatementList *, Environment *env);

static void freeStatementList(StatementList *list);

static Statement *initStatement(StatementType type);

static void freeStatement(Statement *s);

static StatementResult executeStatement(Statement *s, Environment *env);

/*expression statement start*/

static StatementResult executeExpressionStatement(Expression *expression,
                                                  Environment *env) {
    evaluateExpression(expression, env);
    return NORMAL_RESULT;
}

static void freeExpressionStatement(Expression *expression) {
    log("free expression type: %d", expression->type);
    return freeExpression(expression);
}

Statement *initExpressionStatement(Expression *expression) {
    Statement *s = initStatement(EXPRESSION_STATEMENT);
    s->type = EXPRESSION_STATEMENT;
    s->s.expression = expression;
    return s;
}
/*expression statement end*/

/*if else statement start*/

static void addElseToIf(IfStatement *ifS, StatementList *elseBlock) {
    ifS->elseBlock = elseBlock;
}

static void addElsIfToIf(IfStatement *ifS,
                         Expression *condition,
                         StatementList *block) {
    ElsIfStatement *elsIfS = (ElsIfStatement *) malloc(sizeof(ElsIfStatement));
    elsIfS->block = block;
    elsIfS->condition = condition;
    elsIfS->next = NULL;
    ifS->elsIfTail->next = elsIfS;
    ifS->elsIfTail = elsIfS;
}

// return 1 else_if executed
static StatementResult executeElsIfStatement(ElsIfStatement *s,
                                             Environment *env) {
    Value condition = evaluateExpression(s->condition, env);
    if (condition.type != BOOL_VALUE) {
        panic("%s", "condition expression is supposed to yield boolean value");
        // TODO: error handler
    }
    if (condition.v.bool_value) {
        executeStatementList(s->block, env);
        return ELSE_IF_EXE_RESULT;
    } else if (s->next != NULL) {
        return executeElsIfStatement(s->next, env);
    }
    return ELSE_IF_NE_RESULT;
}

static StatementResult executeIfStatement(IfStatement *s, Environment *env) {
    Environment* localEnv = initEnvironment(env);
    Value condition = evaluateExpression(s->condition, localEnv);
    if (condition.type != BOOL_VALUE) {
        panic("%s", "condition expression is supposed to yield boolean value");
        // TODO: error handler
    }
    StatementResult res = ELSE_IF_NE_RESULT;
    if (condition.v.bool_value) {
        res = executeStatementList(s->block, localEnv);
    } else {
        if (s->elsIfHead->next != NULL) {
            res = executeElsIfStatement(s->elsIfHead->next, localEnv);
        }
        if (res == ELSE_IF_NE_RESULT && s->elseBlock != NULL)
            res = executeStatementList(s->elseBlock, localEnv);
    }
    localEnv->free(localEnv);
    return res == ELSE_IF_NE_RESULT ? NORMAL_RESULT : res;
}

static void freeElsifStatement(ElsIfStatement *elsifS) {
    freeExpression(elsifS->condition);
    freeStatementList(elsifS->block);
}

static void freeIfStatement(IfStatement *ifS) {
    log("free if statement %s", "");
    freeExpression(ifS->condition);
//    if (ifS->env != NULL)
//        free(ifS->env);
    if (ifS->elseBlock != NULL)
        freeStatementList(ifS->elseBlock);
    freeStatementList(ifS->block);
    ElsIfStatement *elsIfS = ifS->elsIfHead->next;
    while (elsIfS != NULL) {
        ElsIfStatement *p = elsIfS->next;
        freeElsifStatement(elsIfS);
        elsIfS = p;
    }
    free(ifS->elsIfHead);
    free(ifS);
}

Statement *initIfStatement(Expression *condition, StatementList *block) {
    Statement *s = initStatement(IF_STATEMENT);
    IfStatement *ifS = (IfStatement *) malloc(sizeof(IfStatement));
//    ifS->env = NULL;
    ifS->condition = condition;
    ifS->block = block;
    ifS->elseBlock = NULL;
    ifS->elsIfHead = ifS->elsIfTail =
            (ElsIfStatement *) malloc(sizeof(ElsIfStatement));
    ifS->elsIfHead->next = NULL;
    ifS->addElse = addElseToIf;
    ifS->addElsIf = addElsIfToIf;
    s->s.ifStatement = ifS;
    return s;
}

/*if else statement end*/

/*for statement start*/
Statement *initForStatement(Expression *initial,
                            Expression *condition,  // not null
                            Expression *post,
                            StatementList *block) {
    Statement *s = initStatement(FOR_STATEMENT);
    s->type = FOR_STATEMENT;
    ForStatement *forS = (ForStatement *) malloc(sizeof(ForStatement));
    forS->initial = initial;
    forS->block = block;
    forS->condition = condition;
    forS->post = post;
    s->s.forStatement = forS;
    return s;
}

static void freeForStatement(ForStatement *forS) {
    if (forS->block != NULL)
        freeStatementList(forS->block);
    freeExpression(forS->condition);
    if (forS->initial != NULL)
        freeExpression(forS->initial);
    if (forS->post != NULL)
        freeExpression(forS->post);
}

static StatementResult executeForStatement(ForStatement *forS,
                                           Environment *env) {
    Environment* localEnv = initEnvironment(env);
    if (forS->initial != NULL) {
        evaluateExpression(forS->initial, localEnv);
    }
    StatementResult res = NORMAL_RESULT;
    while (1) {
        Value v = evaluateExpression(forS->condition, localEnv);
        if (v.type != BOOL_VALUE) {
            panic(
                    "Condition expression is supposed to yield boolean value. "
                    "value type: %d",
                    v.type);
        }
        if (!v.v.bool_value)
            break;
        if (forS->block != NULL) {
            res = executeStatementList(forS->block, localEnv);
            if (res == BREAK_RESULT || res == RETURN_RESULT)
                break;
        }
        if (forS->post != NULL)
            evaluateExpression(forS->post, localEnv);
    }
    localEnv->free(localEnv);
    return res == RETURN_RESULT ? RETURN_RESULT : NORMAL_RESULT;
}

/*for statement end*/

/* break continue return start*/
Statement *initBreakStatement() {
    return initStatement(BREAK_STATEMENT);
}

Statement *initContinueStatement() {
    return initStatement(CONTINUE_STATEMENT);
}

Statement *initReturnStatement() {
    return initStatement(RETURN_STATEMENT);
}
/* break continue return end*/

/*statement list start*/
static void addStatementToList(StatementList *list, Statement *s) {
    list->tail->next = s;
    list->tail = s;
}

static StatementResult executeStatementList(StatementList *list,
                                            Environment *env) {
    Statement *p = list->head->next;
    while (p != NULL) {
        StatementResult res = executeStatement(p, env);
        if (res == BREAK_RESULT || res == CONTINUE_RESULT ||
            res == RETURN_RESULT)
            return res;
        p = p->next;
    }
    return NORMAL_RESULT;
}

static void freeStatementList(StatementList *list) {
    Statement *p = list->head->next;
    log("free statement list%s", "");
    while (p != NULL) {
        Statement *q = p->next;
        freeStatement(p);
        p = q;
    }
    free(list->head);
}

StatementList *initStatementList() {
    StatementList *list = (StatementList *) malloc(sizeof(StatementList));
    list->tail = list->head = initStatement(-1);
    list->head->next = NULL;
    list->free = freeStatementList;
    list->add = addStatementToList;
    list->execute = executeStatementList;
    return list;
}
/*statement list end*/

/*statement start*/
static Statement *initStatement(StatementType type) {
    Statement *s = (Statement *) malloc(sizeof(Statement));
    s->type = type;
    s->next = NULL;
    return s;
}

static void freeStatement(Statement *s) {
    log("type %d", s->type);
    switch (s->type) {
        case EXPRESSION_STATEMENT:
            freeExpressionStatement(s->s.expression);
            break;
        case IF_STATEMENT:
            freeIfStatement(s->s.ifStatement);
            break;
        case FOR_STATEMENT:
            freeForStatement(s->s.forStatement);
            break;
        case BREAK_STATEMENT:
        case CONTINUE_STATEMENT:
        case RETURN_STATEMENT:
            break;
        default:
            panic("%s", "bad case...");
    }
    free(s);
}

static StatementResult executeStatement(Statement *s, Environment *env) {
    switch (s->type) {
        case EXPRESSION_STATEMENT:
            return executeExpressionStatement(s->s.expression, env);
        case IF_STATEMENT:
            return executeIfStatement(s->s.ifStatement, env);
        case FOR_STATEMENT:
            return executeForStatement(s->s.forStatement, env);
        case RETURN_STATEMENT:
            // todo
            return RETURN_RESULT;
        case BREAK_STATEMENT:
            return BREAK_RESULT;
        case CONTINUE_STATEMENT:
            return CONTINUE_RESULT;
        default:
            panic("%s", "bad case...");
    }
}

/*statement end*/
