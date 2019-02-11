/* Created by Tau on 08/02/2019 */
#ifndef _HG_IDENTIFIER_H_
#define _HG_IDENTIFIER_H_

#include "function.h"
#include "value.h"
typedef struct {
    enum {
        FUNCTION_IDENTIFIER,
        VARIABLE_IDENTIFIER,
    } type;
    union {
        Value value;
        FunctionDefine *function;
    } u;
    String* name;
} Identifier;

#define TRIE_NODE_SIZE 53
// a-zA-Z_
typedef struct TrieNodeTag {
    struct TrieNodeTag* children[TRIE_NODE_SIZE];
    Identifier* id;
} TrieNode;

typedef TrieNode IdentifierTrie;

typedef struct EnvironmentTag {
    IdentifierTrie* trie;
    struct EnvironmentTag* father;

    void (*addVariable)(struct EnvironmentTag* self, String* id, Value v);

    Value (*findVariable)(struct EnvironmentTag* self, String* id);

    FunctionDefine (*findFunction)(struct EnvironmentTag* self, String* id);

    void (*addFunction)(struct EnvironmentTag* self,
                        String* id,
                        FunctionDefine *v);

    void (*free)(struct EnvironmentTag* self);
} Environment;

Environment* initEnvironment(Environment* father);

#endif /*_HG_IDENTIFIER_H_*/