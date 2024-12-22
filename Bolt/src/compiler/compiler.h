#ifndef COMPILER_H
#define COMPILER_H

#include <tokenizer.h>
#include <parsing.h>
#include <assembling.h>

typedef struct
{
    Tokenizer tokenizer;
    Parser parser;
    Assembler assembler;
} Compiler;

void initCompiler(Compiler *compiler);
void setCompilerRoot(Compiler *compiler, const char *filepath);
void compileCode(Compiler *compiler);

#endif