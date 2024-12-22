#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include <tokenizer.h>

typedef enum AstType
{
    AST_TYPE_LITERAL_EXPRESSION_NODE,
    AST_TYPE_UNARY_EXPRESSION_NODE,
    AST_TYPE_BINARY_EXPRESSION_NODE,
    AST_TYPE_TERNARY_EXPRESSION_NODE,
    AST_TYPE_ASSIGNMENT_EXPRESSION_NODE,
    AST_TYPE_CALL_EXPRESSION_NODE,
    AST_TYPE_CAST_EXPRESSION_NODE,
} AstType;

typedef struct Ast
{
    AstType type;
} Ast;

typedef struct AstLiteralExpression
{
    Ast info;
    TokenAttribute value;
} AstLiteralExpression;

typedef struct AstUnaryExpression
{
    Ast info;
    TokenType op;
    Ast *right;
} AstUnaryExpression;

typedef struct AstBinaryExpression
{
    Ast info;
    Ast *left;
    Token op;
    Ast *right;
} AstBinaryExpression;

typedef struct AstTernaryExpression
{
    Ast info;
    Ast *condition;
    Ast *thenBranch;
    Ast *elseBranch;
} AstTernaryExpression;

typedef struct AstAssignmentExpression
{
    Ast info;
    Ast *target;
    Token equal;
    Ast *value;
} AstAssignmentExpression;

typedef struct AstCallExpression
{
    Ast info;
    Ast *callee;
    Token paren;
} AstCallExpression;

typedef struct AstCastExpression
{
    Ast info;
    Ast *type;
    Ast *target;
} AstCastExpression;

typedef struct AstArray
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
void printLiteralAst(AstLiteralExpression *ast);
void printUnaryAst(AstUnaryExpression *ast);

Ast *parseExpression(Parser *parser);
Ast *parseLiteralExpression(Parser *parser);
Ast *parseUnaryExpression(Parser *parser);
Ast *parseTermExpression(Parser *parser);
Ast *parseFactorExpression(Parser *parser);
Ast *parseComparisonExpression(Parser *parser);
Ast *parseEqualityExpression(Parser *parser);
Ast *parseTernaryExpression(Parser *parser);

bool isAtEndParser(Parser *parser);
bool checkParser(Parser *parser, TokenType type);
bool matchParser(Parser *parser, TokenType *types, size_t count);
Token peekParser(Parser *parser);
Token previous(Parser *parser);
Token advanceParser(Parser *parser);

#endif