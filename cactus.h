// Created by Tao Yang on 2019/02/03
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CMD_MOD

#ifdef CMD_MOD
#define SHOW_HEADER printf(">>>")
#else
#define SHOW_HEADER
#endif

#define LOG_ON
#ifdef LOG_ON
#define log(x, y) printf("LOG:"), printf((x), (y)), printf("\n")
#else
#define log(x, y)
#endif

typedef enum {
    INT_VALUE = 1,
    BOOL_VALUE,
    DOUBLE_VALUE,
    STRING_VALUE,
    NULL_VALUE
} ValueType;

typedef struct {
    // 0 false
    int8_t v;
} Bool;

typedef struct {
    char *s;
    size_t length;
} String;

typedef struct value_tag {
    ValueType type;
    union {
        int32_t int_value;
        double double_value;
        Bool bool_value;
        String string_value;
    } u;
} Value;

typedef struct {
    char *identifier;
    Value v;
} Variable;

typedef struct node_tag {
    Variable *var;
    struct node_tag *next;
} VariableNode, ArgumentNode;

VariableNode globalVariableList;

Variable *createVariable(char *identifier, Value v);

char *createIdentifier(char *s);

void addToGlobalVariableList(Variable *var);

void releaseGlobalVariableMemo();

Variable *searchLocalVariable(char *identifier);

void printValue(Value v);

Value callFunction(cahr *identifier, ArgumentNode *argumentList);