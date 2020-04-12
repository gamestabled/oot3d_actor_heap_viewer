#ifndef HEAP_H
#define HEAP_H

#include "z3d.h"

typedef uint16_t ActorType;
typedef uint32_t ActorSize;

enum ActorType_enum {
    Switch = 0,
    Background = 1,
    Player = 2,
    Bomb = 3,
    Npc = 4,
    Enemy = 5,
    Prop = 6,
    Item = 7,
    Misc = 8,
    Boss = 9,
    Door = 10,
    Chest = 11,
};

typedef struct {
    // heap_node_t* heap_node_begin;
    actor_t*  actor_instance_begin;
    ActorSize size;
    uint16_t  actor_id;
    uint16_t  params;
    ActorType category;
    uint16_t  free;
} actor_data;

#define num_actors_addr 0x5c66fc
#define actor_list_addr 0x5c6700

#define num_actors (*(uint32_t *)num_actors_addr)
#define actor_list ((actor_data *)actor_list_addr)
#define NULL 0x00000000

#endif //HEAP_H
