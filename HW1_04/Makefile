all: dungeon

dungeon: dungeon.c load_save.c heap.c movement.c
	gcc dungeon.c load_save.c heap.c movement.c -o dungeon -lm

clean:
	rm -f dungeon