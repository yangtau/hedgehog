/* Created by Tau on 08/02/2019 */
#include "environment.h"
#include <stdlib.h>
#include "debug.h"
#include "value.h"

static int _hashChar(char c) {
    if (c == '_')
        return 0;
    else if (c <= 'Z' && c >= 'A')
        return 1 + c - 'A';
    else
        return 27 + c - 'a';
}

static Identifier* _createVariableIdentifier(String* name, Value v) {
    Identifier* id = (Identifier*)malloc(sizeof(Identifier));
    id->name = name;
    id->type = VARIABLE_IDENTIFIER;
    id->u.value = v;
    // name.refer(name);
    // name.release(name);
    return id;
}

static void _freeIdentifier(Identifier* id) {
    id->name->release(id->name);
    free(id);
}

static TrieNode* createTrie() {
    TrieNode* t = (TrieNode*)malloc(sizeof(TrieNode));
    t->id = NULL;
    t->flag = 0;
    for (int i = 0; i < TRIE_NODE_SIZE; i++)
        t->children[i] = NULL;
    return t;
}

static void freeTrie(TrieNode* root) {
    for (int i = 0; i < TRIE_NODE_SIZE; i++) {
        if (root->children[i] != NULL)
            freeTrie(root->children[i]);
    }
    if (root->id != NULL)
        _freeIdentifier(root->id);
    free(root);
}

static TrieNode* _searchIdentifier(IdentifierTrie* trie, String* name) {
    TrieNode *p = trie, *q;
    int i = 0;
    char c = name->str[0];
    while (1) {
        int index = _hashChar(c);
        q = p->children[index];
        if (q == NULL) {
            q = p->children[index] = createTrie();
        }
        c = name->str[++i];
        if (c == '\0') {
            name->release(name);
            return q;
        }
        p = q;
    }
}

static void addVariable(Environment* Env, String* name, Value value) {
    // name.refer(name);
    log("add %s", name->str);
    TrieNode* t = _searchIdentifier(Env->trie, name);
    if (t == NULL) {
        panic("%s", "unexpected error, maybe memory...");
    } else {
        if (t->id == NULL) {
            name->refer(name);
            t->id = _createVariableIdentifier(name, value);
            t->flag = 1;
        } else {
            t->id->type = VARIABLE_IDENTIFIER;
            t->id->u.value = value;
        }
    }
    // name.release(name);
}

static Value findVariable(Environment* Env, String* name) {
    log("find %s", name->str);
    name->refer(name);
    TrieNode* t = _searchIdentifier(Env->trie, name);
    Value v;
    v.type = NULL_VALUE;
    if (t == NULL) {
        panic("%s", "unexpected error, maybe memory...");
    } else if (t->id == NULL) {
        panic("\"%s\" not found", name->str);
    } else {
        v = t->id->u.value;
    }
    name->release(name);
    return v;
}

static void freeEnvironment(Environment* env) {
    freeTrie(env->trie);
    free(env);
}

Environment* initEnvironment(Environment* father) {
    Environment* env = (Environment*)malloc(sizeof(Environment));
    env->trie = createTrie();
    env->father = father;
    env->free = freeEnvironment;
    env->addVariable = addVariable;
    env->findVariable = findVariable;
    return env;
}
