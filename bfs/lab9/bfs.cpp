#include <stdlib.h>
#include <string.h>
#include "bfs.h"

#define infinit 100000000
#define max_size 10000

/*
                LAB 09
        GOG RARES FLAVIU        GROUP 30424

*complexitate bfs O(V+E) 
* 
* 

*/

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    Point neighbors_location[] = { {0,-1}/*sus*/, {0,1}/*jos*/, {-1,0}/*stanga*/, {1,0}/*dreapta*/};
    int neighbors = 0;
    for (int i = 0;i < 4;i++)
    {
        int row = p.row + neighbors_location[i].row;
        int col = p.col + neighbors_location[i].col;

        if (row >= 0 and col >= 0 and row < grid->rows and col < grid->cols)
        {
            if (grid->mat[row][col] == 0)
            {
                neighb[neighbors].col = col;
                neighb[neighbors].row = row;
                neighbors++;
            }
        }
    }

    return neighbors;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

typedef struct {
    Node* storage[max_size];
    int f;
    int r;
}Queue;

void initializare(Queue* q)
{
    q->f = 0;
    q->r = 0;
}

int Empty(Queue* q)
{
    return q->f == q->r;
}

void enq(Queue* q, Node* node)
{
    if (q->r + 1 != max_size)
    {
        q->storage[q->r] = node;
        q->r = q->r + 1;
    }
    else exit;
}

Node* deq(Queue* q)
{
    if(Empty(q)) exit;
    
    Node* node = q->storage[q->f];
    q->f = q->f + 1;
    return node;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    Queue q;
    initializare(&q);
    //complexitate O(V+E)
    for (int i = 0;i < graph->nrNodes;i++)
    {
        if (op)op->count(2);
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = infinit;
        graph->v[i]->parent = NULL;
    }
    s->color = COLOR_GRAY;
    s->dist = 0;
    enq(&q, s); 

    while (!Empty(&q))
    {
        Node* deqQ = deq(&q);
        for (int i = 0;i < deqQ->adjSize;i++)
        {
            Node* N = deqQ->adj[i];
            if (op)op->count();
            if (N->color == COLOR_WHITE)
            {
                N->color = COLOR_GRAY;
                N->dist = deqQ->dist + 1;
                N->parent = deqQ;
                enq(&q, N);
            }
        }
        deqQ->color = COLOR_BLACK;
    }
}

void printare(int index, int h, int* p,int n, Point* repr)
{
    for (int i = 0; i < h;i++)
    {
        printf("\t");
    }
    printf("[%d,%d] \n", repr[index].row, repr[index].col);

    for (int i = 0; i < n;i++)
    {
        if (p[i] == index) // printare pe baza vectorului de tati
        {
            printare(i,h+1,p,n,repr);
        }
    }
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){

        for (int i = 0; i < n;i++) //folosire pretty print
        {
            if (p[i] == -1)
            {
                printare(i,0,p,n,repr);
            }
        }
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    bfs(graph, start, NULL);
    if (end->dist == infinit)
    {
        return -1;
    }
    int lungime = 0;
    Node* actual = end;

    while (actual != NULL)
    {
        path[lungime++] = actual;
        actual = actual->parent;
    }
    for (int i = 0;i < lungime / 2;i++)
    {
        std::swap(path[i], path[lungime - 1 - i]);
    }
    return lungime;
}

void generare_edge(Graph* graph, int amount)
{
    int counter = 0;

    for (int i = 1; i < graph->nrNodes; ++i) 
    {
        Node* U = graph->v[i - 1];
        Node* V = graph->v[i];

        U->adj = (Node**)realloc(U->adj,(U->adjSize +1)*sizeof(Node*));
        U->adj[U->adjSize++] = V;

        V->adj = (Node**)realloc(V->adj,(V->adjSize +1)*sizeof(Node*));
        V->adj[V->adjSize++] = U;

        counter++;
    }

    while (counter < amount)
    {
        int u = rand() % graph->nrNodes;
        int v = rand() % graph->nrNodes;
        if (u != v)
        {
            Node* U = graph->v[u];
            Node* V = graph->v[v];

            int ok = 1;
            for (int i = 0;i < U->adjSize;i++)
            {
                if (U->adj[i] == V)
                {
                    ok = 0;
                    break;
                }
            }
            if (ok == 1)
            {
               U->adj = (Node**)realloc(U->adj,(U->adjSize +1)*sizeof(Node*));
               U->adj[U->adjSize++] = V;

               V->adj = (Node**)realloc(V->adj,(V->adjSize +1)*sizeof(Node*));
               V->adj[V->adjSize++] = U;

                counter++;
            }
        }
    }
}

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }

        generare_edge(&graph, n); //call generare edge

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        generare_edge(&graph, 4500); //call generare edge

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }
    p.divideValues("bfs-vertices", 2);
    p.divideValues("bfs-edges", 2);
    p.showReport();
}
