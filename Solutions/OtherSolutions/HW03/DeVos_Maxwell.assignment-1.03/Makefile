all: compile

compile: dungeon.c generator.c load_save.c render.c path_finding.c heap.c headers/generator.h
	gcc -Wall -Werror -ggdb2 -O0 dungeon.c generator.c load_save.c render.c path_finding.c heap.c -o bin/dungeon -lm

run: bin/dungeon
	bin/dungeon

clean:
	rm -rf bin/*