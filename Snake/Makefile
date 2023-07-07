CFLAGS = -Wall -lncurses

all: csnake 

csnake: main.o Snake.o
	@echo "Linking and producing the final application"
	gcc $(CFLAGS) main.o Snake.o -o	csnake 
	chmod +x csnake 

main.o: main.c
	@echo "Compiling main file"
	gcc $(CFLAGS) -c main.c

Snake.o: Snake.c
	@echo "Compiling Snake file"
	gcc $(CFLAGS) -c Snake.c

clean:
	@echo "Removing everything but the source files"
	rm main.o Snake.o csnake 
