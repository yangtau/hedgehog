#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
// char to int
static int _hashChar(char c) {
    if (c == '_')
        return 0;
    else if (c <= 'Z' && c >= 'A')
        return 1 + c - 'A';
    else
        return 27 + c - 'a';
}

static Identifier* _createVariableIdentifier(const char* name, Value v) {
    Identifier* id = (Identifier*)malloc(sizeof(Identifier));
    id->name = (char*)malloc(sizeof((strlen(name) + 1)));
    strcpy(id->name, name);
    id->type = VARIABLE_IDENTIFIER;
    id->u.value = v;
    return id;
}

static void _freeIdentifier(Identifier* id) {
    free(id->name);
    free(id);
}

static TrieNode* _createTrieNode() {
    TrieNode* t = (TrieNode*)malloc(sizeof(TrieNode));
    t->id = NULL;
    t->flag = 0;
    for (int i = 0; i < TRIE_NODE_SIZE; i++) t->children[i] = NULL;
}

static void _disposeTrie(TrieNode* root) {
    if (root == NULL) return;
    for (int i = 0; i < TRIE_NODE_SIZE; i++) {
        disposeTrie(root->children[i]);
    }
    if (root->id != NULL) _freeIdentifier(root->id);
    free(root);
}

TrieNode* _searchIdentifier(Identifier* trie, const char* name) {
    TrieNode *p = trie, *q;
    int i = 0;
    char c = name[0];
    while (1) {
        int index = _hashChar(c);
        q = p->children[index];
        if (q == NULL) {
            q = p->children[index] = _createTrieNode();
        }
        c = name[++i];
        if (c == '\0') {
            return q;
        }
        p = q;
    }
    return NULL;
}

Environment* createEnvironment() {
    Environment* env = (Environment*)malloc(sizeof(Environment));
    env->trie = _createTrieNode();
}

void disposeEnvironment(Environment* env) {
    _disposeTrie(env->trie);
    free(env);
}

void addGlobalVariable(Environment* globalEnv, const char* name, Value value) {
    TrieNode* t = _searchIdentifier(globalEnv->trie, name);
    if (t == NULL) {
        // TODO:error
    }
    if (t->id == NULL) {
        t->id = _createVariableIdentifier(name, value);
        t->flag = 1;
    } else {
        t->id->type = VARIABLE_IDENTIFIER;
        t->id->u.value = value;
    }
}

Interpreter* createInterpreter() {
    Interpreter* interpreter = (Interpreter*)malloc(sizeof(Interpreter));
    interpreter->globalEnv = createEnvironment();
    return interpreter;
}

void disposeInterpreter(Interpreter* interpreter) {
    disposeEnvironment(interpreter->globalEnv);
    free(interpreter);
}

static Interpreter* current_interpreter;
void interpret(Interpreter* interpreter, FILE* file) {
    extern int yyparse(void);
    extern FILE* yyin;
    yyin = file;
    current_interpreter = interpreter;
    if (yyparse()) {
        fprintf(stderr, "Error !\n");
        exit(1);
    }
}

Interpreter* getCurrentInterpreter() {
    return current_interpreter;
}
