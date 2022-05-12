all: exec

c-files: myshell.o LineParser.o
	gcc -g -c -o myshell.o myshell.c
	gcc -g -c -o LineParser.o LineParser.c

exec: c-files myshell.o LineParser.o
	gcc -g myshell.o LineParser.o -o myshell
	rm -f myshell.o LineParser.o
	
.PHONY: clean
clean:
	rm -rf ./*.o main
