all: gerar_arquivos_teste.o
	@gcc gerar_arquivos_teste.o -o exe
	@rm gerar_arquivos_teste.o

gerar_arquivos_teste.o: gerar_arquivos_teste.c gerar_arquivos_teste.h
	@gcc -c gerar_arquivos_teste.c -Wall
