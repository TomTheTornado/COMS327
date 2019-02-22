all: dungeon

dungeon: dungeon.c load_save.c heap.c
	gcc dungeon.c load_save.c heap.c -o dungeon -lm

clean:
	rm -f dungeon