#ifndef IO_H
# define IO_H

# define COLOR_DARKGRAY 8
typedef class dungeon dungeon_t;

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(dungeon_t *d);
void io_displayFog(dungeon_t *d);
void io_handle_input(dungeon_t *d);
void io_queue_message(const char *format, ...);

#endif
