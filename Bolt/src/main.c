#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include <compiler.h>

int main(void)
{
    Compiler compiler;
    initCompiler(&compiler);
    setCompilerRoot(&compiler, "C:/Github/CDev/BoltC/test.txt");
    compileCode(&compiler);

    return EXIT_SUCCESS;
}