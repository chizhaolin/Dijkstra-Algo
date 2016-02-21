#ifndef _dijkstra_h
#define _dijkstra_h

#define MAX_VERTIX  9
#define INFINITE    100000

typedef struct node {
    int vertix;
    int weight;
    struct node* next;
    struct node* parent;
} t_node, t_queue;

typedef struct spset {
    t_queue* path;
    int vertix;
    int distance;
} t_spset;

#endif

