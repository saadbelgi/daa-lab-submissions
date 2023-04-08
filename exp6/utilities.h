typedef struct edge
{
    int dest;
    int source;
    int weight;
    int shortest_distance;
    struct edge *next;
} *p_edge;
typedef struct linked_list
{
    p_edge head;
    p_edge tail;
} *p_linked_list;

p_edge create_node(int dest, int weight);
p_linked_list create_linked_list();
void insert_edge(p_linked_list ll, int dest, int weight);

typedef struct priority_queue
{
    int arr_size;
    int heap_size;
    p_edge *arr;
    int type; // 0 -> min heap based on weights, 1 -> min heap based on shortest distance
} *p_priority_queue;

p_priority_queue create_priority_queue(int max_size, int type);
void enqueue(p_priority_queue pq, p_edge edge);
p_edge dequeue(p_priority_queue pq);
int is_empty(p_priority_queue pq);

typedef struct graph
{
    int directed;
    int V;                // no. of vertices
    int E;                // no. of edges
    p_linked_list *edges; // array of pointers to linked lists of edges...index of the array represents source vertex and each linked list containes all outgoing edges of that source vertex
} *p_graph;
p_graph take_directed_graph_input();
p_graph take_undirected_graph_input();
void add_edge_to_undirected_graph(p_graph g, int source, int dest, int weight);
void display_graph(p_graph g);