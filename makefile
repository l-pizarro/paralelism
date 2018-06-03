HEADERS = lecturaArchivos.h programa.h registro.h buffer.h control.h

all: programa.c $(HEADERS)
	gcc -o laboratorio.out main.c programa.c lecturaArchivos.c registro.c buffer.c control.c -Wall -lm -g

clean:
	-rm -f laboratorio.out
run:
	./laboratorio.out
