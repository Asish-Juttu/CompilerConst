driver: driver.c parser.c lexer.c symbolTable.c twinBuffer.c token.c toString.c 
	gcc -o driver driver.c parser.c lexer.c symbolTable.c twinBuffer.c token.c toString.c

clean: 
	rm -f driver
