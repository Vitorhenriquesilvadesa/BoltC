#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include <tokenizer.h>

typedef enum
{
    AST_TYPE_LITERAL_EXPRESSION_NODE,
    AST_TYPE_UNARY_EXPRESSION_NODE,
    AST_TYPE_BINARY_EXPRESSION_NODE,
    AST_TYPE_TERNARY_EXPRESSION_NODE,
    AST_TYPE_ASSIGNMENT_EXPRESSION_NODE,
    AST_TYPE_CALL_EXPRESSION_NODE,
    AST_TYPE_CAST_EXPRESSION_NODE,
} AstType;

typedef struct
{
    AstType type;
} Ast;

typedef struct
{
    Ast info;
    TokenAttribute value;
} AstLiteralExpression;

typedef struct
{
    Ast info;
    TokenType op;
    Ast *right;
} AstUnaryExpression;

typedef struct
{
    Ast info;
    Ast *left;
    Token op;
    Ast *right;
} AstBinaryExpression;

typedef struct
{
    Ast info;
    Ast *condition;
    Ast *thenBranch;
    Ast *elseBranch;
} AstTernaryExpression;

typedef struct
{
    Ast info;
    Ast *target;
    Token equal;
    Ast *value;
} AstAssignmentExpression;

typedef struct
{
    Ast info;
    Ast *callee;
    Token paren;
} AstCallExpression;

typedef struct
{
    Ast info;
    Ast *type;
    Ast *target;
} AstCastExpression;

typedef struct
{
    size_t count;
    size_t capacity;
    Ast **trees;
} AstArray;

typedef struct
{
    uint32_t current;
    TokenArray tokens;
    AstArray trees;
} Parser;

void initParser(Parser *parser);
void initAstArray(AstArray *array);
void parseTokens(Parser *parser, Tokenizer *tokenizer);
void growAstArray(AstArray *array);
void appendAstArray(AstArray *array, Ast *ast);
void printAst(Ast *ast);

void parseExpression(Parser *parser);
void parseLiteralExpression(Parser *parser);

bool isAtEndParser(Parser *parser);
bool checkParser(Parser *parser, TokenType type);
bool matchParser(Parser *parser, TokenType *types, size_t count);
Token peekParser(Parser *parser);
Token previous(Parser *parser);
Token advanceParser(Parser *parser);

#endif