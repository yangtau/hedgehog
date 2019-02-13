/* Created by Tau on 08/02/2019 */
#include <stdlib.h>
#include "environment.h"
#include "debug.h"
#include "oop.h"


//trie
#define TRIE_NODE_SIZE 53
// a-zA-Z_
typedef struct TrieNodeTag {
    struct TrieNodeTag *children[TRIE_NODE_SIZE];
    Variable *obj;
} TrieNode;

static int _hashChar(char c) {
    if (c == '_')
        return 0;
    else if (c <= 'Z' && c >= 'A')
        return 1 + c - 'A';
    else
        return 27 + c - 'a';
}

static TrieNode *createTrie() {
    TrieNode *t = (TrieNode *) malloc(sizeof(TrieNode));
    t->obj = NULL;
    for (int i = 0; i < TRIE_NODE_SIZE; i++)
        t->children[i] = NULL;
    return t;
}

static void freeTrie(TrieNode *root) {
    for (int i = 0; i < TRIE_NODE_SIZE; i++) {
        if (root->children[i] != NULL)
            freeTrie(root->children[i]);
    }
    if (root->obj != NULL)
        del(root->obj);
    free(root);
}

static TrieNode *searchTrieNode(ObjectTrie *trie, String *name) {
    TrieNode *p = trie, *q;
    int i = 0;
    char c = name->str[0];
    while (1) {
        int index = _hashChar(c);
        q = p->children[index];
        if (q == NULL) {
            name->release(name);
            return NULL;
        }
        c = name->str[++i];
        if (c == '\0') {
            name->release(name);
            return q;
        }
        p = q;
    }
}

static TrieNode *createTrieNode(ObjectTrie *trie, String *name) {
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


//environment

static void addVariable(Environment *self, Variable *var) {
    log(("add %s", var->id->str));
    TrieNode *t = NULL;
    on_self(var->id, refer);
    t = searchTrieNode(self->trie, var->id);
//    Environment *pEnv = self;
//    while (pEnv != NULL) {
//        on_self(var->id, refer);
//        t = searchTrieNode(pEnv->trie, var->id);
//        if (t != NULL) break;
//        pEnv = pEnv->father;
//    }
    if (t == NULL) {
        on_self(var->id, refer);
        t = createTrieNode(self->trie, var->id);
    }
    t->obj = var;
}

static Variable *findVariable(Environment *self, String *name) {
    log(("find %s", name->str));
    TrieNode *t = NULL;
    Environment *pEnv = self;
    while (pEnv != NULL) {
        on_self(name, refer);
        t = searchTrieNode(pEnv->trie, name);
        if (t != NULL) break;
        pEnv = pEnv->father;
    }
    if (t == NULL || t->obj == NULL) {
        panic("\"%s\" not found", name->str);
    } else {
        name->release(name);
        return t->obj;
    }
}


static void freeEnvironment(Environment *self) {
    freeTrie(self->trie);
    free(self);
}

static void addFather(Environment *self, Environment *father) {
    self->father = father;
}

const static Environment EnvironmentBase = {
        addFather, addVariable, findVariable, freeEnvironment
};

void *initEnvironment() {
    Environment *p = malloc(sizeof(Environment));
    *p = EnvironmentBase;
    p->trie = createTrie();
    return p;
}

