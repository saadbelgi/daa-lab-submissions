#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// linked list implementation:

typedef struct edge
{
    int dest;
    int source; // for prims
    int weight;
    int shortest_distance;
    struct edge *next;
} *p_edge;
typedef struct linked_list
{
    p_edge head;
    p_edge tail;
} *p_linked_list;

p_edge create_edge(int dest, int weight)
{
    p_edge new = malloc(sizeof(struct edge));
    new->next = NULL;
    new->dest = dest;
    new->weight = weight;
    return new;
}

p_linked_list create_linked_list()
{
    p_linked_list ll = malloc(sizeof(struct linked_list));
    ll->head = NULL;
    ll->tail = NULL;
}

void insert_edge(p_linked_list ll, int dest, int weight)
{
    if (ll->tail == NULL)
    {
        ll->head = create_edge(dest, weight);
        ll->tail = ll->head;
    }
    else
    {
        ll->tail->next = create_edge(dest, weight);
        ll->tail = ll->tail->next;
    }
}

// priority queue implementation:

typedef struct priority_queue
{
    int arr_size;
    int heap_size;
    p_edge *arr;
    int type; // 0 -> min heap based on weights, 1 -> min heap based on shortest distance
} *p_priority_queue;

p_priority_queue create_priority_queue(int max_size, int type)
{
    p_priority_queue ppq = malloc(sizeof(struct priority_queue));
    ppq->arr = malloc(sizeof(p_edge) * max_size);
    ppq->arr_size = max_size;
    ppq->heap_size = 0;
    ppq->type = type;
    return ppq;
}

void enqueue(p_priority_queue pq, p_edge edge)
{
    if (pq->heap_size >= pq->arr_size)
    {
        printf("Queue is full. Element cannot be inserted.\n");
        return;
    }
    // heap insertion:
    pq->arr[pq->heap_size] = edge;
    int parent = ((pq->heap_size + 1) / 2) - 1;
    int child = pq->heap_size;
    pq->heap_size++;
    p_edge temp;
    if (pq->type == 1)
    {
        while (parent >= 0)
        {
            if (pq->arr[child]->shortest_distance < pq->arr[parent]->shortest_distance)
            {
                temp = pq->arr[child];
                pq->arr[child] = pq->arr[parent];
                pq->arr[parent] = temp;
                child = parent;
                parent = ((parent + 1) / 2) - 1;
            }
            else
                return;
        }
    }
    else
    {
        while (parent >= 0)
        {
            if (pq->arr[child]->weight < pq->arr[parent]->weight)
            {
                temp = pq->arr[child];
                pq->arr[child] = pq->arr[parent];
                pq->arr[parent] = temp;
                child = parent;
                parent = ((parent + 1) / 2) - 1;
            }
            else
                return;
        }
    }
}

p_edge dequeue(p_priority_queue pq)
{
    if (pq->heap_size == 0)
        return NULL;
    p_edge ret_val = pq->arr[0];
    pq->heap_size--;
    pq->arr[0] = pq->arr[pq->heap_size];
    // heapify:
    int parent = 0, smallest, left, right;
    p_edge temp;
    if (pq->type == 1)
    {
        while (parent < pq->heap_size)
        {
            left = parent * 2 + 1;
            right = left + 1;
            smallest = parent;
            if (left < pq->heap_size && pq->arr[left]->shortest_distance < pq->arr[smallest]->shortest_distance)
                smallest = left;
            if (right < pq->heap_size && pq->arr[right]->shortest_distance < pq->arr[smallest]->shortest_distance)
                smallest = right;
            if (smallest != parent)
            {
                temp = pq->arr[smallest];
                pq->arr[smallest] = pq->arr[parent];
                pq->arr[parent] = temp;
                parent = smallest;
            }
            else
                break;
        }
    }
    else
    {
        while (parent < pq->heap_size)
        {
            left = parent * 2 + 1;
            right = left + 1;
            smallest = parent;
            if (left < pq->heap_size && pq->arr[left]->weight < pq->arr[smallest]->weight)
                smallest = left;
            if (right < pq->heap_size && pq->arr[right]->weight < pq->arr[smallest]->weight)
                smallest = right;
            if (smallest != parent)
            {
                temp = pq->arr[smallest];
                pq->arr[smallest] = pq->arr[parent];
                pq->arr[parent] = temp;
                parent = smallest;
            }
            else
                break;
        }
    }
    return ret_val;
}

int is_empty(p_priority_queue pq)
{
    return pq->heap_size == 0;
}

// graphs:
typedef struct graph
{
    int directed;
    int V;                // no. of vertices
    int E;                // no. of edges
    p_linked_list *edges; // array of pointers to linked lists of edges...index of the array represents source vertex and each linked list containes all outgoing edges of that source vertex
} *p_graph;

p_graph take_directed_graph_input()
{
    p_graph g = malloc(sizeof(struct graph));
    g->directed = 1;
    printf("Enter number of vertices: ");
    scanf("%d", &g->V);
    printf("Enter number of edges: ");
    scanf("%d", &g->E);
    g->edges = malloc(sizeof(p_linked_list) * g->V);
    for (int i = 0; i < g->V; i++)
        g->edges[i] = create_linked_list();
    int source, dest, weight;
    for (int i = 0; i < g->E; i++)
    {
        printf("For edge %d:\nEnter source vertex: ", i + 1);
        scanf("%d", &source);
        printf("Enter destination vertex: ");
        scanf("%d", &dest);
        printf("Enter weight: ");
        scanf("%d", &weight);
        insert_edge(g->edges[source], dest, weight);
    }
    return g;
}

void add_edge_to_undirected_graph(p_graph g, int source, int dest, int weight)
{
    insert_edge(g->edges[source], dest, weight);
    g->edges[source]->tail->source = source;
    insert_edge(g->edges[dest], source, weight);
    g->edges[dest]->tail->source = dest;
}

p_graph take_undirected_graph_input()
{
    p_graph g = malloc(sizeof(struct graph));
    g->directed = 0;
    printf("Enter number of vertices: ");
    scanf("%d", &g->V);
    printf("Enter number of edges: ");
    scanf("%d", &g->E);
    g->edges = malloc(sizeof(p_linked_list) * g->V);
    for (int i = 0; i < g->V; i++)
        g->edges[i] = create_linked_list();
    int source, dest, weight;
    for (int i = 0; i < g->E; i++)
    {
        printf("For edge %d:\nEnter source vertex: ", i + 1);
        scanf("%d", &source);
        printf("Enter destination vertex: ");
        scanf("%d", &dest);
        printf("Enter weight: ");
        scanf("%d", &weight);
        add_edge_to_undirected_graph(g, source, dest, weight);
    }
    return g;
}

void display_graph(p_graph g)
{
    for (int i = 0; i < g->V; i++)
    {
        printf("%d : ", i);
        if (g->edges[i]->head != NULL)
        {
            printf("(%d,%d) ", g->edges[i]->head->dest, g->edges[i]->head->weight);
            for (p_edge e = g->edges[i]->head->next; e != NULL; e = e->next)
                printf(", (%d,%d) ", e->dest, e->weight);
        }
        printf("\n");
    }
}