#include <compiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

static const char *readFile(const char *filepath)
{
    FILE *file;
    file = fopen(filepath, "r");

    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *buffer = ALLOCATE(char, size + 1);

    if (buffer == NULL)
    {
        fprintf(stderr, "Error to allocate buffer for read file!");
        fclose(file);
        return NULL;
    }

    fread(buffer, sizeof(char), size, file);
    buffer[size] = '\0';
    fclose(file);

    return buffer;
}

void initCompiler(Compiler *compiler)
{
    initTokenizer(&compiler->tokenizer);
    initParser(&compiler->parser);
}

void setCompilerRoot(Compiler *compiler, const char *filepath)
{
    const char *sourceCode = readFile(filepath);
    setTokenizerSourceCode(&compiler->tokenizer, sourceCode);
}

void compileCode(Compiler *compiler)
{
    scanTokens(&compiler->tokenizer);
    parseTokens(&compiler->parser, &compiler->tokenizer);
}
