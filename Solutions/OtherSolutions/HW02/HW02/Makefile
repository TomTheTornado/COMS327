all: c_debug

c_debug: generator.c headers/generator.h
	gcc -Wall -Werror -ggdb2 -O0 dungeon.c generator.c load_save.c -o bin/debug/dungeon -lm

c_release: generator.c headers/generator.h
	gcc -O2 dungeon.c generator.c load_save.c -o bin/release/dungeon -lm

run: bin/debug/dungeon
	bin/debug/dungeon

release: c_release bin/release/dungeon
	bin/release/dungeon
	
clean:
	rm -rf bin/release/* bin/debug/*
