#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"

struct prims_result
{
    int cost;
    p_graph mst;
};

struct prims_result *prims(p_graph g)
{
    p_graph mst = malloc(sizeof(struct graph));
    mst->V = g->V;
    mst->E = 0;
    mst->directed = 0;
    mst->edges = malloc(sizeof(p_linked_list) * g->V);
    for (int i = 0; i < mst->V; i++)
        mst->edges[i] = create_linked_list();
    p_priority_queue pq = create_priority_queue(g->E, 0);
    int added[g->V];
    int cost = 0;
    for (int i = 0; i < g->V; i++)
        added[i] = 0;
    int curr_vtx = 0;
    added[curr_vtx] = 1;
    int vtx_counter = 1;
    do
    {
        for (p_edge e = g->edges[curr_vtx]->head; e != NULL; e = e->next)
        {
            if (!added[e->dest])
                enqueue(pq, e);
        }
        p_edge min_edge = dequeue(pq);
        while (added[min_edge->source] && added[min_edge->dest])
            min_edge = dequeue(pq);
        add_edge_to_undirected_graph(mst, min_edge->source, min_edge->dest, min_edge->weight);
        printf("added edge (%d,%d)\n", min_edge->source, min_edge->dest);
        mst->E++;
        cost += min_edge->weight;
        added[min_edge->dest] = 1;
        vtx_counter++;
        curr_vtx = min_edge->dest;
    } while (vtx_counter < g->V);
    struct prims_result *pr = malloc(sizeof(struct prims_result));
    pr->cost = cost;
    pr->mst = mst;
    return pr;
}

int main()
{
    // freopen("input_2.txt", "r", stdin);
    p_graph g = take_undirected_graph_input();
    printf("\n\n");
    struct prims_result *pr = prims(g);
    printf("\nCost: %d\n", pr->cost);
    printf("Tree (graph):\n");
    display_graph(pr->mst);
}