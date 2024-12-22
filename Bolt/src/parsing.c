#include <parsing.h>
#include <memory.h>
#include <array.h>
#include <stdio.h>

void initParser(Parser *parser)
{
    parser->current = 0;
    initAstArray(&parser->trees);
    parser->tokens.capacity = 0;
    parser->tokens.count = 0;
    parser->tokens.tokens = NULL;
}

void initAstArray(AstArray *array)
{
    array->count = 0;
    array->capacity = MIN_ARRAY_SIZE;
    array->trees = ALLOCATE(Ast *, MIN_ARRAY_SIZE);
}

void parseTokens(Parser *parser, Tokenizer *tokenizer)
{
    parser->tokens = tokenizer->tokens;

    while (!isAtEndParser(parser))
    {
        parseExpression(parser);
    }

    for (size_t i = 0; i < parser->trees.count; i++)
    {
        printAst(parser->trees.trees[i]);
    }
}

void growAstArray(AstArray *array)
{
    size_t oldCapacity = array->capacity;
    size_t newCapacity = oldCapacity * ARRAY_GROW_FACTOR;
    array->capacity = newCapacity;
    array->trees = REALLOCATE(Ast *, array->trees, oldCapacity, newCapacity);
}

void appendAstArray(AstArray *array, Ast *ast)
{
    if (array->count >= array->capacity - 1)
    {
        growAstArray(array);
    }

    array->trees[array->count++] = ast;
}

void printAst(Ast *ast)
{
    switch (ast->type)
    {
    case AST_TYPE_LITERAL_EXPRESSION_NODE:
        AstLiteralExpression *expression = (AstLiteralExpression *)ast;

        switch (expression->value.type)
        {
        case TOKEN_ATTRIBUTE_TYPE_INT_LITERAL:
            printf("Literal(%d)\n", expression->value.value.integer);
            break;

        default:
            break;
        }

        break;

    default:
        break;
    }
}

bool isAtEndParser(Parser *parser)
{
    return parser->tokens.tokens[parser->current].type == TOKEN_TYPE_EOF;
}

bool checkParser(Parser *parser, TokenType type)
{
    if (isAtEndParser(parser))
    {
        return false;
    }

    return peekParser(parser).type == type;
}

bool matchParser(Parser *parser, TokenType *types, size_t count)
{
    if (isAtEndParser(parser))
    {
        return false;
    }

    for (size_t i = 0; i < count; i++)
    {
        if (peekParser(parser).type == types[i])
        {
            advanceParser(parser);
            return true;
        }
    }

    return false;
}

Token peekParser(Parser *parser)
{
    return parser->tokens.tokens[parser->current];
}

Token previous(Parser *parser)
{
    return parser->tokens.tokens[parser->current - 1];
}

Token advanceParser(Parser *parser)
{
    parser->current++;
    return parser->tokens.tokens[parser->current];
}

void parseExpression(Parser *parser)
{
    parseLiteralExpression(parser);
}

void parseLiteralExpression(Parser *parser)
{
    TokenType types[] = {TOKEN_TYPE_IDENTIFIER, TOKEN_TYPE_INT};

    if (matchParser(parser, types, 2))
    {
        Ast ast = {AST_TYPE_LITERAL_EXPRESSION_NODE};
        AstLiteralExpression *expression = ALLOCATE(AstLiteralExpression, 1);
        expression->info = ast;
        expression->value = previous(parser).attribute;

        appendAstArray(&parser->trees, (Ast *)expression);
    }
}
