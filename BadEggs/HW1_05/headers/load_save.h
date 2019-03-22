#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#ifdef __APPLE__

#include <libkern/OSByteOrder.h>
#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)
#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#else
#include <endian.h>
#endif
#include "dungeon.h"

#define MARK	"RLG327-S2019"

/**
 * Enum for specifying different actions
 * */
typedef enum action
{
    other,
    load,
    save,
    load_save,
    monsters,
    load_monsters,
} action_t;

typedef enum open
{
    readf,
    writef
}open_t;

/**
 * Loads a dungeon from a file
 * */
int load_dungeon(dungeon_t *d);

/**
 * Saves a dungeon to a file
 * */
int save_dungeon(dungeon_t *d);

/**
 * Opens a file for reading or writing
 * */
int open_file(FILE **f, open_t open);

#endif