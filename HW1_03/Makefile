all: dungeon

dungeon: dungeon.c load_save.c
	gcc dungeon.c load_save.c -o dungeon -lm

clean:
	rm -f dungeon