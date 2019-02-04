#include "cactus.h"
#include <stdio.h>
#include <stdlib.h>

#define LOG_ON
#ifdef LOG_ON
#define log(x, y) logMsg((x), (y))
#else
#define log(x, y)
#endif

void logMsg(char *x, char *y) {
    printf("--LOG--");
    printf(x, y);
    printf("\n");
}

Variable *createVariable(char *identifier, Value v) {
    log("create var: %s", identifier);
    Variable *var = (Variable *)malloc(sizeof(Variable));
    var->identifier = identifier;
    var->v = v;
    return var;
}

char *createIdentifier(char *s) {
    log("create id: %s", s);
    char *id = (char *)malloc(sizeof(char) * (strlen(s) + 1));
    strcpy(id, s);
    return id;
}

void addToGlobalVariableList(Variable *var) {
    log("add var :%s", var->identifier);
    VariableNode *p = (VariableNode *)malloc(sizeof(VariableNode));
    p->var = var;
    p->next = globalVariableList.next;
    globalVariableList.next = p;
}

void releaseGlobalVariableMemo() {
    log("release memo", "");
    while (globalVariableList.next != NULL) {
      VariableNode *p = globalVariableList.next;
      globalVariableList.next = p->next;
      free(p->var->identifier);
      free(p->var);
      free(p);
    }
}