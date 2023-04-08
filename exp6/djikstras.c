#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "utilities.h"

struct djikstra_result
{
    int *parent;
    int *shortest_distance;
    int size;
    int source;
};

struct djikstra_result *djikstras(p_graph g, int source)
{
    p_priority_queue pq = create_priority_queue(g->V, 1); // min heap of shortest distance
    int visited[g->V];
    struct djikstra_result *dr = malloc(sizeof(struct djikstra_result));
    dr->parent = malloc(sizeof(int) * g->V);
    dr->shortest_distance = malloc(sizeof(int) * g->V);
    dr->parent[source] = -1;
    dr->size = g->V;
    dr->source = source;
    for (int i = 0; i < g->V; i++)
    {
        visited[i] = 0;
        dr->shortest_distance[i] = INT_MAX;
    }
    dr->shortest_distance[source] = 0;
    struct edge self = {.dest = 0, .weight = 0, .shortest_distance = 0, .next = g->edges[source]->head};
    enqueue(pq, &self);
    while (!is_empty(pq))
    {
        // extracting the edge with minimum weight
        int curr_vtx = dequeue(pq)->dest;
        // traversing neighbours of current vertex
        for (p_edge i = g->edges[curr_vtx]->head; i != NULL; i = i->next)
        {
            if (!visited[i->dest])
            {
                if (dr->shortest_distance[curr_vtx] + i->weight < dr->shortest_distance[i->dest])
                {
                    dr->shortest_distance[i->dest] = dr->shortest_distance[curr_vtx] + i->weight;
                    dr->parent[i->dest] = curr_vtx;
                }
                i->shortest_distance = dr->shortest_distance[i->dest];
                enqueue(pq, i);
            }
        }
        visited[curr_vtx] = 1;
    }
    return dr;
}

void print_path(int *parents, int dest)
{
    if (parents[dest] == -1)
    {
        printf("%d", dest);
        return;
    }
    print_path(parents, parents[dest]);
    printf(" -> %d", dest);
}

void print_all_shortest_paths(struct djikstra_result *dr)
{
    for (int i = 0; i < dr->size; i++)
    {
        printf("To %d : ", i);
        print_path(dr->parent, i);
        printf("  :  %d\n", dr->shortest_distance[i]);
    }
}

int main()
{
    freopen("input_2.txt", "r", stdin);
    p_graph g = take_directed_graph_input();
    printf("Enter source vertex: ");
    int source;
    scanf("%d", &source);
    struct djikstra_result *dr = djikstras(g, source);
    printf("\n\nAll single source (%d) shortest paths and their costs:\n\n", source);
    print_all_shortest_paths(dr);
}