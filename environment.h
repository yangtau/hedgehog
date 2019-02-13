/* Created by Tau on 08/02/2019 */
#ifndef _HG_IDENTIFIER_H_
#define _HG_IDENTIFIER_H_

#include "variable.h"

//interface
typedef struct EnvironmentTag Environment;
typedef struct TrieNodeTag ObjectTrie;

struct EnvironmentTag {
    void (*addFather)(Environment *self, Environment *father);

    void (*addVariable)(Environment *self, Variable *var);

    Variable *(*findVariable)(Environment *self, String *id);

    void (*free)(Environment *self);

    ObjectTrie *trie;
    Environment *father;
};

void *initEnvironment();

#endif /*_HG_IDENTIFIER_H_*/