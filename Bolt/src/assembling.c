#include <assembling.h>
#include <stdlib.h>

void initAssembler(Assembler *assembler, const char *outputPath)
{
    assembler->trees.capacity = 0;
    assembler->trees.count = 0;
    assembler->trees.trees = NULL;
    assembler->currentAst = 0;
    assembler->output = fopen(outputPath, "w");

    if (!assembler->output)
    {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }
}

void setAssemblerAstArray(Assembler *assembler, AstArray array)
{
    assembler->trees = array;
}

void emitAssembly(Assembler *assembler)
{
    while (assemblerHasAst(assembler))
    {
        emitAssemblyForAst(assembler, getAssemblerNextAst(assembler));
    }

    fclose(assembler->output);
}

bool assemblerHasAst(Assembler *assembler)
{
    return assembler->currentAst < assembler->trees.count;
}

Ast *getAssemblerNextAst(Assembler *assembler)
{
    return assembler->trees.trees[assembler->currentAst++];
}

void emitAssemblyForAst(Assembler *assembler, Ast *ast)
{
    switch (ast->type)
    {
    case AST_TYPE_LITERAL_EXPRESSION_NODE:
        emitAssemblyForLiteralExpression(assembler, (AstLiteralExpression *)ast);
        break;
    }
}

void emitAssemblyForLiteralExpression(Assembler *assembler, AstLiteralExpression *ast)
{
    switch (ast->value.type)
    {
    case TOKEN_ATTRIBUTE_TYPE_INT_LITERAL:
        fprintf(assembler->output, "\tmov eax, %d\n", ast->value.value.integer);
        fprintf(assembler->output, "\tpush eax\n");
        break;

    case TOKEN_ATTRIBUTE_TYPE_FLOAT_LITERAL:
        fprintf(assembler->output, "section .data\n");
        fprintf(assembler->output, "\tfloat_literal_%p dq %f\n", ast, ast->value.value.floating);

        fprintf(assembler->output, "section .text\n");
        fprintf(assembler->output, "\tfld qword [float_literal_%p]\n", ast);
        break;

    case TOKEN_ATTRIBUTE_TYPE_STRING_LITERAL:
        fprintf(assembler->output, "section .data\n");
        fprintf(assembler->output, "\tstring_literal_%p db \"%s\", 0\n", ast, ast->value.value.string);

        fprintf(assembler->output, "section .text\n");
        fprintf(assembler->output, "\tmov eax, string_literal_%p\n", ast);
        fprintf(assembler->output, "\tpush eax\n");
        break;

    default:
        fprintf(stderr, "Unsupported literal type in AST.\n");
        break;
    }
}