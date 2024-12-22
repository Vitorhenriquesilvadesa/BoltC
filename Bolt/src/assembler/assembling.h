#ifndef ASSEMBER_H
#define ASSEMBER_H

#include <parsing.h>
#include <stdio.h>

typedef struct Assembler
{
    AstArray trees;
    FILE *output;
    size_t currentAst;
} Assembler;

void initAssembler(Assembler *assembler, const char *outputPath);
void setAssemblerAstArray(Assembler *assembler, AstArray array);
void emitAssembly(Assembler *assembler);
bool assemblerHasAst(Assembler *assembler);
Ast *getAssemblerNextAst(Assembler *assembler);
void emitAssemblyForAst(Assembler *assembler, Ast *ast);
void emitAssemblyForLiteralExpression(Assembler *assembler, AstLiteralExpression *ast);

#endif