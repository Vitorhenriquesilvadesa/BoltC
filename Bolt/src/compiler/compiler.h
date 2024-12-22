#ifndef COMPILER_H
#define COMPILER_H

#include <tokenizer.h>
#include <parsing.h>

typedef struct
{
    Tokenizer tokenizer;
    Parser parser;
} Compiler;

void initCompiler(Compiler *compiler);
void setCompilerRoot(Compiler *compiler, const char *filepath);
void compileCode(Compiler *compiler);

#endif