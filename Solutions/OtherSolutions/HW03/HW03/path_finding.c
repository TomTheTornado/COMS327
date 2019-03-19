#include <limits.h>
#include <stdlib.h>
#include <stdint.h>

#include "headers/heap.h"
#include "headers/dungeon.h"
#include "headers/path_finding.h"

typedef struct path_finding
{
    heap_node_t *hn;
    pos_t pos;
    pos_t from;
    int32_t cost;
} path_finding_t;

static void prepare_arrays(dungeon_t *d)
{
    uint8_t i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            d->non_tunneling_map[i][j] = 0;
            d->tunneling_map[i][j] = 0;
        }
    }
    return;
}

static int32_t path_finding_cmp(const void *key, const void *with)
{
    return ((path_finding_t *)key)->cost - ((path_finding_t *)with)->cost;
}

static void dijkstra(dungeon_t *d, uint8_t mode)
{
    static path_finding_t path[MAP_HEIGHT][MAP_WIDTH], *p;
    heap_t h;
    uint32_t i, j;

    heap_init(&h, path_finding_cmp, NULL);

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            path[i][j].pos.y = i;
            path[i][j].pos.x = j;
            path[i][j].cost = (d->player.pos.x != j || d->player.pos.y != i) ? INT_MAX : 0;

            if (mapxy(j, i) != c_h_wall && mapxy(j, i) != c_v_wall && mode) //fill heap if not hard wall
                path[i][j].hn = heap_insert(&h, &path[i][j]);
            else if (d->hardness[i][j] == 0) //fill heap if a room or corridor
                path[i][j].hn = heap_insert(&h, &path[i][j]);
            else
                path[i][j].hn = NULL;
        }
    }

    while ((p = heap_remove_min(&h)))
    {
        p->hn = NULL;

        int8_t offset[3] = {-1, 0, 1};

        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if ((path[p->pos.y + offset[i]][p->pos.x + offset[j]].hn) && (path[p->pos.y + offset[i]][p->pos.x + offset[j]].cost > p->cost + costxy(p->pos.x,p->pos.y)))
                {
                    path[p->pos.y + offset[i]][p->pos.x + offset[j]].cost = p->cost + costxy(p->pos.x,p->pos.y);
                    path[p->pos.y + offset[i]][p->pos.x + offset[j]].from.y = p->pos.y;
                    path[p->pos.y + offset[i]][p->pos.x + offset[j]].from.x = p->pos.x;
                    heap_decrease_key_no_replace(&h, path[p->pos.y + offset[i]][p->pos.x + offset[j]].hn);
                }
            }
        }

        //add cost
        if (mode)
            d->tunneling_map[p->pos.y][p->pos.x] = p->cost;
        else
            d->non_tunneling_map[p->pos.y][p->pos.x] = (p->cost == INT_MAX) ? p->cost + 1 : p->cost;
    }

    heap_delete(&h);
    return;
}

void get_distance_maps(dungeon_t *d)
{
    prepare_arrays(d); //init distance arrays
    dijkstra(d, 0);    //tunneling
    dijkstra(d, 1);    //non tunneling
}
