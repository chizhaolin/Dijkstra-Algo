#include <stdio.h>
#include <malloc.h>
#include "dijkstra.h"

// The global set to store the visited vertix
int g_VisitedSet[MAX_VERTIX];
// The global set to store the remaining vertix
int g_RemainingSet[MAX_VERTIX];
// The global set to store the shortest path info
t_spset g_SpSet[MAX_VERTIX];
// The adjcent list adjacent list
t_queue* g_AdjList[MAX_VERTIX];

// Create the new node with the vertix and weight
t_node* createNode(int vertix, int weight)
{
    t_node* pNode;

    pNode = (t_node*)malloc(sizeof(t_node));

    if (pNode != NULL) {
        pNode->vertix = vertix;
        pNode->weight = weight;
        pNode->next = NULL;
        pNode->parent = NULL;
    }

    return pNode;
}

// Add new node to the queue
t_queue* enQueue(t_queue* queue, t_node* node)
{
    t_node* pTmp;

    if (queue == NULL)
        return node;

    pTmp = queue;
    while (pTmp->next != NULL)
        pTmp = pTmp->next;

    pTmp->next = node;

    return queue;
}

// Delete the 1st node of the queue
t_queue* deQueue(t_queue* queue)
{
    if (queue != NULL) {
        t_node* pDel;
        pDel = queue;
        queue = queue->next;
        free(pDel);
        pDel = NULL;
    }

    return queue;
}

// Destroy the whole queue
void destroyQueue(t_queue** queue)
{
    while (*queue) {
        *queue = deQueue(*queue);
    }
}

// Add the edge between the 2 verties
void addEdge(int n1, int n2, int weight)
{
    g_AdjList[n1] = enQueue(g_AdjList[n1], createNode(n2, weight));
    g_AdjList[n2] = enQueue(g_AdjList[n2], createNode(n1, weight));
}

// Initialization procedure
void init()
{
    int i;

    for (i = 0; i < MAX_VERTIX; i++) {
        g_VisitedSet[i] = -1;
        g_RemainingSet[i] = i;
        g_AdjList[i] = createNode(i, INFINITE);
        g_SpSet[i].vertix = i;
        g_SpSet[i].distance = INFINITE;
    }
}

void finalize()
{
    int i;
    
    for (i = 0; i < MAX_VERTIX; i++) {
        g_VisitedSet[i] = -1;
        g_RemainingSet[i] = i;
        destroyQueue(&g_AdjList[i]);
        destroyQueue(&g_SpSet[i].path);
        g_SpSet[i].vertix = i;
        g_SpSet[i].distance = INFINITE;
    }
}

// Function to check if the remaining set is empty
int isRemainingSetEmpty()
{
    int i;

    for (i = 0; i < MAX_VERTIX; i++) {
        if (g_RemainingSet[i] != -1)
            return 0;
    }

    return 1;
}

// Search the next vertix for the shortest path
int searchShortestPath(t_queue* pAdjList)
{
    int i;
    int min;
    int v = -1;
    t_node* pTmp;

    pTmp = pAdjList->next;
    min = INFINITE;
    
    while (pTmp) { 
        if (g_VisitedSet[pTmp->vertix] == -1) {

           if (g_SpSet[pTmp->vertix].distance < min) {
                min = g_SpSet[pTmp->vertix].distance;
                v = pTmp->vertix;
           }
        }
        
        pTmp = pTmp->next;
    }

    /* If v == -1, meaning all the vertix in this queue has been visit, we need to find another vertix */
    if (v == -1) {
        int i;
        for (i = 0; i < MAX_VERTIX; i++) {
            if (g_VisitedSet[i] != -1)
                continue;
            if (g_SpSet[i].distance < min) {
                min = g_SpSet[i].distance;
                v = i;
            }
        }
    }

    return v;
}

// dijkstra algorithm
void dijkstra(int vertix)
{
    int nextV;
    int i;

    nextV = vertix;
    g_SpSet[vertix].distance = 0;

    // Initialize the shortest path with current vertix
    for (i = 0; i < MAX_VERTIX; i++) {
        g_SpSet[i].path = enQueue(g_SpSet[i].path, createNode(vertix, 0));
    }

    while (!isRemainingSetEmpty()) {
        t_queue* pAdjList;

        g_VisitedSet[nextV] = nextV;
        g_RemainingSet[nextV] = -1;

        pAdjList = g_AdjList[nextV];
        while (pAdjList) {
            // Check if the current one is the shortest one
            if ((g_SpSet[pAdjList->vertix].distance > g_SpSet[nextV].distance + pAdjList->weight)) {
                t_queue* pTmp = g_SpSet[nextV].path;
                g_SpSet[pAdjList->vertix].distance = g_SpSet[nextV].distance + pAdjList->weight;
                // Destroy the shortest path and update it with the new one
                destroyQueue(&(g_SpSet[pAdjList->vertix].path));
                while (pTmp) {
                    g_SpSet[pAdjList->vertix].path = enQueue(g_SpSet[pAdjList->vertix].path, createNode(pTmp->vertix, pTmp->weight));
                    pTmp = pTmp->next;
                }
                g_SpSet[pAdjList->vertix].path = enQueue(g_SpSet[pAdjList->vertix].path, createNode(pAdjList->vertix, pAdjList->weight));
            }
            pAdjList = pAdjList->next;
        }

        nextV = searchShortestPath(g_AdjList[nextV]);
    }
}

int main()
{
    int i;
    init();
    addEdge(0, 1, 4);
    addEdge(0, 7, 8);
    addEdge(1, 2, 8);
    addEdge(1, 7, 11);
    addEdge(2, 3, 7);
    addEdge(2, 5, 4);
    addEdge(2, 8, 2);
    addEdge(3, 4, 9);
    addEdge(3, 5, 14);
    addEdge(4, 5, 10);
    addEdge(5, 6, 2);
    addEdge(6, 7, 1);
    addEdge(6, 8, 6);
    addEdge(7, 8, 7);

    // Output the adjacent list
    for (i = 0; i < MAX_VERTIX; i++) {
        t_queue* p;
        p = g_AdjList[i];
        while (p) {
            printf ("%d ", p->vertix);
            p = p->next;
        }
        printf("\n");
    }

    dijkstra(0);

    for (i = 0; i < MAX_VERTIX; i++) {
        t_queue* path = g_SpSet[i].path;

        printf("The shortest distance from 0 to %d is: %d, path is: ", i, g_SpSet[i].distance);
        while (path) {
            printf("%d ", path->vertix);
            path = path->next;
        }
        printf("\n");
    }

    finalize();

    return 0;
}
