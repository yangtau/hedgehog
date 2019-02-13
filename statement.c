/* Created by Tau on 08/02/2019 */

#include "statement.h"
#include <stdlib.h>
#include "debug.h"
#include "oop.h"
#include "function.h"
#include "value.h"

struct ElsIfStatementTag {
    Expression *condition;
    StatementList *block;
    struct ElsIfStatementTag *next;
};

struct ForStatementTag {
    Statement base;
    Expression *initial;  // assign
    Expression *condition;
    Expression *post;
    StatementList *block;
};


typedef struct ExpressionStatementTag {
    Statement base;
    Expression *exp;
} ExpressionStatement;


typedef ExpressionStatement ReturnStatement;
typedef struct FunctionDefineStatementTag {
    Statement base;
    String *id;
    ParameterList *paras;
    StatementList *block;
} FunctionDefineStatement;


/*expression statement*/

static StatementResult executeExpressionStatement(void *_self, Environment *env) {
//    log("execute %s", "start");
    ExpressionStatement *self = _self;
    self->exp->evaluate(self->exp, env);
    StatementResult result = {NORMAL_RESULT,};
    return result;
}

static void freeExpressionStatement(void *_self) {
    ExpressionStatement *self = _self;
    del(self->exp);
    free(self);
}

const static Statement ExpressionStatementInterface = {
        executeExpressionStatement, freeExpressionStatement
};

void *initExpressionStatement(Expression *expression) {
    ExpressionStatement *s = malloc(sizeof(ExpressionStatement));
    s->base = ExpressionStatementInterface;
    s->exp = expression;
    return s;
}

/*if else statement*/

static void addElseToIf(IfStatement *ifS, StatementList *elseBlock) {
    ifS->elseBlock = elseBlock;
}

static void addElsIfToIf(IfStatement *ifS,
                         Expression *condition,
                         StatementList *block) {
    ElsIfStatement *elsIfS = malloc(sizeof(ElsIfStatement));
    elsIfS->block = block;
    elsIfS->condition = condition;
    elsIfS->next = NULL;
    ifS->elsIfTail->next = elsIfS;
    ifS->elsIfTail = elsIfS;
}

static StatementResult executeElsIfStatement(void *_self,
                                             Environment *env) {
    ElsIfStatement *self = _self;
    Value condition = self->condition->evaluate(self->condition, env);
    if (condition.type != BOOL_VALUE) {
        panic("%s", "condition expression is supposed to yield boolean value");
        // TODO: error handler
    }
    StatementResult res = {ELSE_IF_NE_RESULT,};
    if (condition.v.bool_value) {
        res = self->block->execute(self->block, env);
        if (res.type == BREAK_RESULT || res.type == CONTINUE_RESULT ||
            res.type == RETURN_RESULT)
            return res;
        res.type = ELSE_IF_EXE_RESULT;
    } else if (self->next != NULL) {
        return executeElsIfStatement(self->next, env);
    }
    return res;
}

static StatementResult executeIfStatement(void *_self, Environment *env) {
    IfStatement *self = _self;
    Environment *localEnv = initEnvironment();
    localEnv->addFather(localEnv, env);
    Value condition = self->condition->evaluate(self->condition, localEnv);
    if (condition.type != BOOL_VALUE) {
        panic("%s", "condition expression is supposed to yield boolean value");
        // TODO: error handler
    }
    StatementResult res = {ELSE_IF_NE_RESULT,};
    if (condition.v.bool_value) {
        res = self->block->execute(self->block, localEnv);
    } else {
        if (self->elsIfHead->next != NULL) {
            res = executeElsIfStatement(self->elsIfHead->next, localEnv);
        }
        if (res.type == ELSE_IF_NE_RESULT && self->elseBlock != NULL)
            res = self->elseBlock->execute(self->elseBlock, localEnv);;
    }
    del(localEnv);
    if (res.type == ELSE_IF_NE_RESULT) res.type = NORMAL_RESULT;
    return res;
}

static void freeElsifStatement(ElsIfStatement *elsifS) {
    del(elsifS->condition);
    del(elsifS->block);
}

static void freeIfStatement(void *_self) {
    IfStatement *self = _self;
    del(self->condition);
    if (self->elseBlock != NULL)
        del(self->elseBlock);
    del(self->block);
    ElsIfStatement *elsIfS = self->elsIfHead->next;
    while (elsIfS != NULL) {
        ElsIfStatement *p = elsIfS->next;
        freeElsifStatement(elsIfS);
        elsIfS = p;
    }
    free(self->elsIfHead);
    free(self);
}

const static Statement IfExpressionBase = {
        executeIfStatement, freeIfStatement
};

void *initIfStatement(Expression *condition, StatementList *block) {
    IfStatement *ifS = (IfStatement *) malloc(sizeof(IfStatement));
    ifS->base = IfExpressionBase;
    ifS->condition = condition;
    ifS->block = block;
    ifS->elseBlock = NULL;
    ifS->elsIfHead = ifS->elsIfTail =
            (ElsIfStatement *) malloc(sizeof(ElsIfStatement));
    ifS->elsIfHead->next = NULL;
    ifS->addElse = addElseToIf;
    ifS->addElsIf = addElsIfToIf;
    return ifS;
}


/*for statement */

static void freeForStatement(void *_self) {
    ForStatement *self = _self;
    if (self->block != NULL)
        del(self->block);
    if (self->condition != NULL)
        del(self->condition);
    if (self->initial != NULL)
        del(self->initial);
    if (self->post != NULL)
        del(self->post);
}

static StatementResult executeForStatement(void *_self,
                                           Environment *env) {
    ForStatement *self = _self;
    Environment *localEnv = initEnvironment();
    localEnv->addFather(localEnv, env);
    if (self->initial != NULL) {
        self->initial->evaluate(self->initial, localEnv);
    }
    StatementResult res = {NORMAL_RESULT,};
    while (1) {
        if (self->condition != NULL) {
            Value v = self->condition->evaluate(self->condition, localEnv);
            if (v.type != BOOL_VALUE) {
                panic(
                        "Condition expression is supposed to yield boolean value. "
                        "value type: %d",
                        v.type);
            }
            if (!v.v.bool_value)
                break;
        }
        if (self->block != NULL) {
            res = self->block->execute(self->block, localEnv);
            if (res.type == BREAK_RESULT || res.type == RETURN_RESULT)
                break;
        }
        if (self->post != NULL)
            self->post->evaluate(self->post, localEnv);
    }
    del(localEnv);
    if (res.type != RETURN_RESULT) res.type = NORMAL_RESULT;
    return res;
}

const static Statement ForStatementBase = {
        executeForStatement, freeForStatement
};

void *initForStatement(Expression *initial, Expression *condition,  // not null
                       Expression *post, StatementList *block) {
    ForStatement *forS = (ForStatement *) malloc(sizeof(ForStatement));
    forS->initial = initial;
    forS->block = block;
    forS->condition = condition;
    forS->post = post;
    forS->base = ForStatementBase;
    return forS;
}


/* break continue */

void freeNULL(void *_) {};

static StatementResult executeBreakStatement(void *_, Environment *__) {
    StatementResult res = {BREAK_RESULT,};
    return res;
}

static Statement BreakStatement = {
        executeBreakStatement, freeNULL
};

void *initBreakStatement() {
    return &BreakStatement;
}

static StatementResult executeContinueStatement(void *_, Environment *__) {
    StatementResult res = {CONTINUE_RESULT,};
    return res;
}

static Statement ContinueStatement = {
        executeContinueStatement, freeNULL
};

void *initContinueStatement() {
    return &ContinueStatement;
}

//return

static void freeReturnStatement(void *_self) {
    ReturnStatement *self = _self;
    del(self->exp);
    free(self);
}

static StatementResult executeReturnStatement(void *_self, Environment *env) {
    ReturnStatement *self = _self;
    Value v;
    if (self->exp != NULL) {
        v = self->exp->evaluate(self->exp, env);
    } else v.type = NULL_VALUE;
    StatementResult res = {
            RETURN_RESULT, v
    };
    return res;
};

const static Statement ReturnStatementBase = {
        executeReturnStatement, freeReturnStatement
};

void *initReturnStatement(Expression *exp) {
    ReturnStatement *s = malloc(sizeof(ReturnStatement));
    s->base = ReturnStatementBase;
    s->exp = exp;
    return s;
}

//function define statement

static void freeFunctionDefineStatement(void *_self) {
    FunctionDefineStatement *self = _self;
//    del(self->block);
    on_self(self->id, release);
    if (self->paras != NULL)
        del(self->paras);
    free(self);
}

static StatementResult executeFunctionDefineStatement(void *_self, Environment *env) {
    FunctionDefineStatement *self = _self;
    Value v;
    v.type = FUNCTION_VALUE;
    v.v.function = initFunctionDefine(self->paras, self->block);
    on_self(self->id, refer);
    env->addVariable(env, initVariable(self->id, v));
    StatementResult res;
    res.type = NORMAL_RESULT;
    return res;
}

const static Statement FunctionDefineStatementBase = {
        executeFunctionDefineStatement, freeFunctionDefineStatement
};

void *initFunctionDefineStatement(String *id, ParameterList *paras, StatementList *block) {
    FunctionDefineStatement *s = malloc(sizeof(FunctionDefineStatement));
    s->base = FunctionDefineStatementBase;
    s->id = id;
    s->paras = paras;
    s->block = block;
    return s;
}

/*statement list*/
static StatementList *addStatementToList(StatementList *list, Statement *s) {
    if (list->head == NULL) {
        list->head = list->tail = s;
        list->tail->next = NULL;
    } else {
        s->next = NULL;
        list->tail->next = s;
        list->tail = s;
    }
    return list;
}

static StatementResult executeStatementList(StatementList *list,
                                            Environment *env) {
//    log("execute statement list %s", "start");
    Statement *p = list->head;
    StatementResult res = {NORMAL_RESULT,};
    while (p != NULL) {
        res = p->execute(p, env);
        if (res.type == BREAK_RESULT || res.type == CONTINUE_RESULT ||
            res.type == RETURN_RESULT)
            return res;
        p = p->next;
    }
    return res;
}

static void freeStatementList(StatementList *list) {
    Statement *p = list->head;
    while (p != NULL) {
        Statement *q = p->next;
        del(p);
        p = q;
    }
}

StatementList *initStatementList() {
    StatementList *list = (StatementList *) malloc(sizeof(StatementList));
    list->tail = list->head = NULL;
    list->free = freeStatementList;
    list->add = addStatementToList;
    list->execute = executeStatementList;
    return list;
}
