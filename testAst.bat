cls
gcc -g -o bin/testAst testAst.c parser.c token.c toString.c lexer.c symbolTable.c twinBuffer.c ast.c astPrint.c astType.c astList.c
gdb bin\testAst core