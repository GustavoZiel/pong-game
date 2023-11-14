# Variáveis utilizadas no Makefile
CC = gcc # Compilador usado
CFLAGS = -lncurses -pthread # Flags de compilação
OBJECTS = game.o menu.o logic.o # Arquivos necessários


# Regra para compilação do arquivo executável
all: $(OBJECTS)
	$(CC) $(OBJECTS) -o game $(CFLAGS)


# Regra geral para gerar .o dado um .c
%.o: %.c
	$(CC) -c $< $(CFLAGS)


# Excluir objetos e executável
clean:
	rm *.o game 


# Executar programa
run:
	./game