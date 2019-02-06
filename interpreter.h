/* Created by Tau on 06/02/2019 */
#ifndef _HG_INTERPRETER_H_
#define _HG_INTERPRETER_H_
#include "stdio.h"
#include "value.h"

typedef struct {
    enum {
        FUNCTION_IDENTIFIER,
        VARIABLE_IDENTIFIER,
    } type;
    union {
        Value value;
        // TODO: function
    } u;
    char *name;
} Identifier;

const int TRIE_NODE_SIZE = 53;  // a-zA-Z_
typedef struct TrieNodeTag {
    struct TrieNodeTag *children[TRIE_NODE_SIZE];
    Identifier *id;
    int flag;
} TrieNode;

typedef TrieNode IdentifierTrie;
typedef struct {
    IdentifierTrie *trie;
} Environment;

Environment *createEnvironment();
void disposeEnvironment(Environment *env);
void addGlobalVariable(Environment *globalEnv, const char *name, Value value);

typedef struct {
    Environment *globalEnv;
} Interpreter;

Interpreter *createInterpreter();
void disposeInterpreter(Interpreter *interpreter);
void interpret(Interpreter *interpreter, FILE *file);
Interpreter *getCurrentInterpreter();
#endif /*_HG_INTERPRETER_H_*/
