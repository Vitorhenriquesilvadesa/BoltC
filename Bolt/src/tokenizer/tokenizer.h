#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <token.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    size_t count;
    size_t capacity;
    Token *tokens;
} TokenArray;

typedef struct
{
    uint32_t line;
    uint32_t start;
    uint32_t length;
    uint32_t sourceLength;
    TokenArray tokens;
    const char *source;
} Tokenizer;

void initTokenizer(Tokenizer *tokenizer);
void setTokenizerSourceCode(Tokenizer *tokenizer, const char *source);
ScannerStatus scanToken(Tokenizer *tokenizer);
ScannerStatus scanTokens(Tokenizer *tokenizer);
void appendTokenArray(TokenArray *array, Token token);

void numberLiteral(Tokenizer *tokenizer);
void stringLiteral(Tokenizer *tokenizer);
void identifier(Tokenizer *tokenizer);
bool isAtEnd(Tokenizer *tokenizer);
char advance(Tokenizer *tokenizer);
char peek(Tokenizer *tokenizer);
char peekNext(Tokenizer *tokenizer);
bool isDigit(char c);
bool isAlpha(char c);
bool isAlphanumeric(char c);
bool match(Tokenizer *tokenizer, char c);
bool check(Tokenizer *tokenizer, char c);
void addToken(Tokenizer *tokenizer, TokenType type);
void addTokenWithLexeme(Tokenizer *tokenizer, TokenType type, char *lexeme);
void addTokenWithLiteral(Tokenizer *tokenizer, TokenType type, char *lexeme, void *literal, TokenAttributeType attributeType);

#endif