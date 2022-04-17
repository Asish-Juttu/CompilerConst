cls
gcc -g -o  bin/testDriver  semError.c type.c parser.c token.c toString.c lexer.c symbolTable.c twinBuffer.c ast.c astPrint.c astType.c astList.c driver.c
gdb --args bin\testDriver testcases\s2.txt 