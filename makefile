HEADERS = lecturaArchivos.h programa.h registro.h pipeline.h

all: programa.c $(HEADERS)
	gcc -o laboratorio.out main.c programa.c lecturaArchivos.c registro.c pipeline.c -Wall -lm -g

clean:
	-rm -f laboratorio.out
run:
	./laboratorio.out
