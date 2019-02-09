#include "statement.h"
#include <stdlib.h>
#include "debug.h"
#include "interpreter.h"

//Statement *createStatement(Expression *expression) {
//    Statement *p = (Statement *) malloc(sizeof(Statement));
//    p->s.expression = expression;
//    getCurrentInterpreter()->statement = p;
//    return p;
//}
//
//void runStatement(Statement *statement) {
//    log("start %s", "evaluateExpression");
//    evaluateExpression(statement->s.expression);
//    log("start %s", "freeExpression");
//    freeExpression(statement->s.expression);
//    log("end %s", "freeExpression");
//    log("start %s", "freeStatement");
//    free(statement);
//    log("end %s", "freeStatement");
//}

static Statement *initStatement() {
    Statement *s = (Statement *) malloc(sizeof(Statement));
    s->next = NULL;
    return s;
}

static void freeStatement(Statement *s) {
//    todo
//    free son statement eg: expression
    free(s);
}

static void executeStatement(Statement *s) {
    //todo
}


static void addStatementToList(StatementList *list, Statement *s) {
    if (list->tail == NULL) {
        list->head->next = list->tail = s;
    } else {
        list->tail->next = s;
        list->tail = s;
    }
}


static void executeStatementList(StatementList *list) {
    while (list->head->next != NULL) {
        Statement *p = list->head->next;
        list->head->next = p->next;
        executeStatement(p);
    }
}

static void freeStatementList(StatementList *list) {
    while (list->head->next != NULL) {
        Statement *p = list->head->next;
        list->head->next = p->next;
        freeStatement(p);
    }
    freeStatement(list->head);
    free(list);
}

StatementList *initStatementList() {
    StatementList *list = (StatementList *) malloc(sizeof(StatementList));
    list->head = initStatement();
    list->tail = NULL;
    list->head->next = list->tail;
    list->free = freeStatementList;
    list->addStatement = addStatementToList;
    list->execute = executeStatementList;
    return list;
}
