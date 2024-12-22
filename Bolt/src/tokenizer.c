#include <token.h>
#include <array.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tokenizer.h"

static void initTokenArray(TokenArray *array)
{
    array->count = 0;
    array->capacity = MIN_ARRAY_SIZE;
    array->tokens = ALLOCATE(Token, MIN_ARRAY_SIZE);
}

static void growTokenArray(TokenArray *array)
{
    size_t oldCapacity = array->capacity;
    size_t newCapacity = oldCapacity * ARRAY_GROW_FACTOR;
    array->capacity = newCapacity;
    array->tokens = REALLOCATE(Token, array->tokens, oldCapacity, newCapacity);
}

void initTokenizer(Tokenizer *tokenizer)
{
    initTokenArray(&tokenizer->tokens);
    tokenizer->line = 1;
    tokenizer->start = 0;
    tokenizer->length = 0;
}

void setTokenizerSourceCode(Tokenizer *tokenizer, const char *source)
{
    size_t length = strlen(source);
    tokenizer->source = source;
    tokenizer->sourceLength = length;
}

ScannerStatus scanToken(Tokenizer *tokenizer)
{
    tokenizer->start += tokenizer->length;
    tokenizer->length = 0;

    char c = advance(tokenizer);

    switch (c)
    {
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        tokenizer->line++;
        break;

    case '#':
        addToken(tokenizer, TOKEN_TYPE_PREPROCESSOR);
        break;
    case ',':
        addToken(tokenizer, TOKEN_TYPE_COMMA);
        break;
    case ':':
        addToken(tokenizer, TOKEN_TYPE_COLON);
        break;
    case ';':
        addToken(tokenizer, TOKEN_TYPE_SEMICOLON);
        break;
    case '[':
        addToken(tokenizer, TOKEN_TYPE_LEFT_BRACKET);
        break;
    case ']':
        addToken(tokenizer, TOKEN_TYPE_RIGHT_BRACKET);
        break;
    case '(':
        addToken(tokenizer, TOKEN_TYPE_LEFT_PAREN);
        break;
    case ')':
        addToken(tokenizer, TOKEN_TYPE_RIGHT_PAREN);
        break;
    case '{':
        addToken(tokenizer, TOKEN_TYPE_LEFT_BRACE);
        break;
    case '}':
        addToken(tokenizer, TOKEN_TYPE_RIGHT_BRACE);
        break;
    case '^':
        addToken(tokenizer, TOKEN_TYPE_BITWISE_XOR);
        break;
    case '~':
        addToken(tokenizer, TOKEN_TYPE_BITWISE_NOT);
        break;
    case '-':
        if (match(tokenizer, '-'))
        {
            addToken(tokenizer, TOKEN_TYPE_MINUS_MINUS);
            break;
        }
        else if (match(tokenizer, '='))
        {
            addToken(tokenizer, TOKEN_TYPE_SUBTRACT_AND_ASSIGN);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_MINUS);
            break;
        }

    case '+':
        if (match(tokenizer, '+'))
        {
            addToken(tokenizer, TOKEN_TYPE_PLUS_PLUS);
            break;
        }
        else if (match(tokenizer, '='))
        {
            addToken(tokenizer, TOKEN_TYPE_ADD_AND_ASSIGN);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_PLUS);
            break;
        }

    case '*':
        if (match(tokenizer, '='))
        {
            addToken(tokenizer, TOKEN_TYPE_MULTIPLY_AND_ASSIGN);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_STAR);
            break;
        }

    case '/':
        if (match(tokenizer, '='))
        {
            addToken(tokenizer, TOKEN_TYPE_DIVIDE_AND_ASSIGN);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_SLASH);
            break;
        }

    case '=':
        if (match(tokenizer, '='))
        {
            addToken(tokenizer, TOKEN_TYPE_EQUAL_EQUAL);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_EQUAL);
            break;
        }

    case '!':
        if (match(tokenizer, '='))
        {
            addToken(tokenizer, TOKEN_TYPE_NOT_EQUAL);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_LOGICAL_NOT);
            break;
        }

    case '>':
        if (match(tokenizer, '='))
        {
            addToken(tokenizer, TOKEN_TYPE_GREATER_EQUAL);
            break;
        }
        else if (match(tokenizer, '>'))
        {
            addToken(tokenizer, TOKEN_TYPE_RIGHT_SHIFT);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_GREATER);
            break;
        }

    case '<':
        if (match(tokenizer, '='))
        {
            addToken(tokenizer, TOKEN_TYPE_LESS_EQUAL);
            break;
        }
        else if (match(tokenizer, '<'))
        {
            addToken(tokenizer, TOKEN_TYPE_LEFT_SHIFT);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_LESS);
            break;
        }

    case '&':
        if (match(tokenizer, '&'))
        {
            addToken(tokenizer, TOKEN_TYPE_LOGICAL_AND);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_BITWISE_AND);
            break;
        }

    case '|':
        if (match(tokenizer, '|'))
        {
            addToken(tokenizer, TOKEN_TYPE_LOGICAL_OR);
            break;
        }
        else
        {
            addToken(tokenizer, TOKEN_TYPE_BITWISE_OR);
            break;
        }

    case '"':
        stringLiteral(tokenizer);
        break;

    default:
        if (isDigit(c))
        {
            numberLiteral(tokenizer);
            break;
        }
        else if (isAlpha(c) || peek(tokenizer) == '_')
        {
            identifier(tokenizer);
        }
    }

    return SCANNER_STATUS_OK;
}

ScannerStatus scanTokens(Tokenizer *tokenizer)
{
    while (!isAtEnd(tokenizer))
    {
        scanToken(tokenizer);
    }

    addTokenWithLexeme(tokenizer, TOKEN_TYPE_EOF, "EOF");

    return SCANNER_STATUS_OK;
}

void appendTokenArray(TokenArray *array, Token token)
{
    if (array->count >= array->capacity - 1)
    {
        growTokenArray(array);
    }

    array->tokens[array->count++] = token;
}

void numberLiteral(Tokenizer *tokenizer)
{
    while (isDigit(peek(tokenizer)))
    {
        advance(tokenizer);
    }

    bool isFloat = false;

    char *lexeme = ALLOCATE(char, tokenizer->length + 1);
    memcpy(lexeme, &tokenizer->source[tokenizer->start], tokenizer->length);
    lexeme[tokenizer->length] = '\0';

    if (!isFloat)
    {
        int literal = atoi(lexeme);
        addTokenWithLiteral(tokenizer, TOKEN_TYPE_INT, lexeme, &literal, TOKEN_ATTRIBUTE_TYPE_INT_LITERAL);
    }
    else
    {
        float literal = atof(lexeme);
        addTokenWithLiteral(tokenizer, TOKEN_TYPE_FLOAT, lexeme, &literal, TOKEN_ATTRIBUTE_TYPE_FLOAT_LITERAL);
    }
}

void stringLiteral(Tokenizer *tokenizer)
{
    while (!check(tokenizer, '"'))
    {
        advance(tokenizer);
    }

    if (isAtEnd(tokenizer))
    {
        printf("Unterminated string at line %u.", tokenizer->line);
    }

    advance(tokenizer);

    char *lexeme = ALLOCATE(char, tokenizer->length - 1);
    memcpy(lexeme, &tokenizer->source[tokenizer->start + 1], tokenizer->length - 2);
    lexeme[tokenizer->length - 2] = '\0';

    addTokenWithLiteral(tokenizer, TOKEN_TYPE_STRING_LITERAL, lexeme, lexeme, TOKEN_ATTRIBUTE_TYPE_STRING_LITERAL);
}

void identifier(Tokenizer *tokenizer)
{
    while (isAlphanumeric(peek(tokenizer)) || peek(tokenizer) == '_')
    {
        advance(tokenizer);
    }

    char *lexeme = ALLOCATE(char, tokenizer->length + 1);
    memcpy(lexeme, &tokenizer->source[tokenizer->start], tokenizer->length);
    lexeme[tokenizer->length] = '\0';

    static const struct
    {
        const char *keyword;
        TokenType type;
    } keywords[] = {
        {"auto", TOKEN_TYPE_AUTO},
        {"break", TOKEN_TYPE_BREAK},
        {"case", TOKEN_TYPE_CASE},
        {"char", TOKEN_TYPE_CHAR},
        {"const", TOKEN_TYPE_CONST},
        {"continue", TOKEN_TYPE_CONTINUE},
        {"default", TOKEN_TYPE_DEFAULT},
        {"do", TOKEN_TYPE_DO},
        {"double", TOKEN_TYPE_DOUBLE},
        {"else", TOKEN_TYPE_ELSE},
        {"enum", TOKEN_TYPE_ENUM},
        {"extern", TOKEN_TYPE_EXTERN},
        {"float", TOKEN_TYPE_FLOAT},
        {"for", TOKEN_TYPE_FOR},
        {"goto", TOKEN_TYPE_GOTO},
        {"if", TOKEN_TYPE_IF},
        {"inline", TOKEN_TYPE_INLINE},
        {"int", TOKEN_TYPE_INT},
        {"long", TOKEN_TYPE_LONG},
        {"register", TOKEN_TYPE_REGISTER},
        {"return", TOKEN_TYPE_RETURN},
        {"short", TOKEN_TYPE_SHORT},
        {"signed", TOKEN_TYPE_SIGNED},
        {"sizeof", TOKEN_TYPE_SIZEOF},
        {"static", TOKEN_TYPE_STATIC},
        {"struct", TOKEN_TYPE_STRUCT},
        {"switch", TOKEN_TYPE_SWITCH},
        {"typedef", TOKEN_TYPE_TYPEDEF},
        {"union", TOKEN_TYPE_UNION},
        {"unsigned", TOKEN_TYPE_UNSIGNED},
        {"void", TOKEN_TYPE_VOID},
        {"volatile", TOKEN_TYPE_VOLATILE},
        {"while", TOKEN_TYPE_WHILE},
    };

    TokenType type = TOKEN_TYPE_IDENTIFIER;
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(lexeme, keywords[i].keyword) == 0)
        {
            type = keywords[i].type;
            break;
        }
    }

    addToken(tokenizer, type);
}

bool isAtEnd(Tokenizer *tokenizer)
{
    return tokenizer->start + tokenizer->length >= tokenizer->sourceLength;
}

char advance(Tokenizer *tokenizer)
{
    return tokenizer->source[tokenizer->start + tokenizer->length++];
}

char peek(Tokenizer *tokenizer)
{
    if (isAtEnd(tokenizer))
    {
        return '\0';
    }

    return tokenizer->source[tokenizer->start + tokenizer->length];
}

char peekNext(Tokenizer *tokenizer)
{
    return tokenizer->source[tokenizer->start + tokenizer->length + 1];
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isAlpha(char c)
{
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool isAlphanumeric(char c)
{
    return isDigit(c) || isAlpha(c);
}

bool match(Tokenizer *tokenizer, char c)
{
    if (isAtEnd(tokenizer))
    {
        return false;
    }

    if (peek(tokenizer) == c)
    {
        advance(tokenizer);
        return true;
    }

    return false;
}

bool check(Tokenizer *tokenizer, char c)
{
    if (isAtEnd(tokenizer))
    {
        return false;
    }

    if (peek(tokenizer) != c)
    {
        return false;
    }

    return true;
}

void addToken(Tokenizer *tokenizer, TokenType type)
{
    Token token = {};
    token.line = tokenizer->line;
    token.type = type;
    token.lexeme = ALLOCATE(char, tokenizer->length + 1);
    memcpy(token.lexeme, &tokenizer->source[tokenizer->start], tokenizer->length);
    token.lexeme[tokenizer->length] = '\0';
    token.attribute.type = TOKEN_ATTRIBUTE_TYPE_NULL_ATTRIBUTE;
    token.attribute.value.integer = 0;
    appendTokenArray(&tokenizer->tokens, token);

    tokenizer->start += tokenizer->length;
    tokenizer->length = 0;
}

void addTokenWithLexeme(Tokenizer *tokenizer, TokenType type, char *lexeme)
{
    Token token = {};
    token.line = tokenizer->line;
    token.type = type;
    token.lexeme = lexeme;
    token.attribute.type = TOKEN_ATTRIBUTE_TYPE_NULL_ATTRIBUTE;
    token.attribute.value.integer = 0;
    appendTokenArray(&tokenizer->tokens, token);

    tokenizer->start += tokenizer->length;
    tokenizer->length = 0;
}

void addTokenWithLiteral(Tokenizer *tokenizer, TokenType type, char *lexeme, void *attribute, TokenAttributeType attributeType)
{
    Token token = {};
    token.line = tokenizer->line;
    token.type = type;
    token.lexeme = lexeme;
    token.attribute.type = attributeType;

    switch (attributeType)
    {
    case TOKEN_ATTRIBUTE_TYPE_FLOAT_LITERAL:
        token.attribute.value.floating = *(float *)attribute;
        break;

    case TOKEN_ATTRIBUTE_TYPE_INT_LITERAL:
        token.attribute.value.integer = *(int *)attribute;
        break;

    case TOKEN_ATTRIBUTE_TYPE_STRING_LITERAL:
        token.attribute.value.string = (char *)attribute;
        break;
        break;

    case TOKEN_ATTRIBUTE_TYPE_NULL_ATTRIBUTE:
        token.attribute.value.integer = 0;
        break;
    }

    appendTokenArray(&tokenizer->tokens, token);

    tokenizer->start += tokenizer->length;
    tokenizer->length = 0;
}