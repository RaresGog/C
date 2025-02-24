#include "dfs.h"

#include "catch2.hpp"

#include <iostream>

#define WHITE 0
#define GRAY 1
#define BLACK 2

/*
                LAB10
    GOG RARES FLAVIU    Group 30424

*/

namespace lab10
{
    graph* init_graph(int vertices)
    {
        graph* gr = (graph*)malloc(sizeof(graph));
        gr->v_nr = vertices;

        gr->adjList = (node**)malloc(vertices * sizeof(node*));
        gr->vertices = (point*)malloc(vertices * sizeof(point));

        for (int i = 0;i < vertices;i++)
        {
            gr->adjList[i] = NULL;
            gr->vertices[i].color = WHITE;
            gr->vertices[i].parent = -1;
            gr->vertices[i].lowlink = -1;
            gr->vertices[i].onstack = false;
            gr->vertices[i].index = -1;
        }
        return gr;
    }
    
    void edge(graph* gr, int u, int v)
    {
        node* N = (node*)malloc(sizeof(node));
        N->v = v;
        N->next = gr->adjList[u];
        gr->adjList[u] = N;
    }

    lists* init_node(int v)
    {
        lists* N = (lists*)malloc(sizeof(lists));
        N->v = v;
        N->next = NULL;
        return N;
    }

    void ins_in_list(lists** h, int v) //topological sort list
    {
        lists* N = init_node(v); 
        N->next = *h;
        *h = N;
    }

    void print_graph(graph* gr)
    {
        for (int i = 0;i < gr->v_nr;i++)
        {
            node* aux = gr->adjList[i];
            printf("ADJ LIST FOR %d :", i);
            int ok = 0;
            while (aux != NULL)
            {
                ok = 1;
                printf("%d ", aux->v);
                aux = aux->next;
            }
            if (ok == 0) printf("UNCONNECTED VERTEX");
            printf("\n");
        }
    }

    void print_list(lists* h)
    {
        printf("TOPOLOGICAL ORDER: ");
        while (h != NULL)
        {
            printf("%d ", h->v);
            h = h->next;
        }
        printf("\n");
    }
   
    void dfs_visit(graph* gr, int i, lists** list,Operation* ops) //t-sort inclus
    {
        gr->vertices[i].color = GRAY;
        node* aux = gr->adjList[i];
    
        while (aux != NULL)
        {
            if (ops)ops->count();
            int V = aux->v;
            if (gr->vertices[V].color == WHITE)
            {
                gr->vertices[V].parent = i;
                dfs_visit(gr, V, list,ops);
            }
            aux = aux->next;
        }
        gr->vertices[i].color = BLACK;

        ins_in_list(list,i); // topological sort 
    }

    void dfs(graph* gr, lists** list,Operation* ops) // t-sort inclus
    {
        for (int i = 0;i < gr->v_nr;i++)
        {
            if (ops)ops->count();
            gr->vertices[i].color = WHITE;
            gr->vertices[i].parent = -1;
        }

        for (int i = 0;i < gr->v_nr;i++)
        {
            if (gr->vertices[i].color == WHITE)
            {
                dfs_visit(gr, i, list,ops);
            }
        }
    }

    void printare(graph* gr, int index, int h)
    {
        for (int i = 0; i < h; i++)
        {
            printf("\t");
        }
        printf("%d \n", index);

        for (int i = 0; i < gr->v_nr; i++) 
        {
            if (gr->vertices[i].parent == index) 
            {
                printare(gr, i, h + 1);
            }
        }
    }

                /* TARJAN ALG */
    void tarjan_visit(graph* gr,int u,int &index,lists** stk)
    {
        gr->vertices[u].index = index;
        gr->vertices[u].lowlink = index;
        index++;
        ins_in_list(stk, u);
        gr->vertices[u].onstack = true;

        node* neighb = gr->adjList[u];
        while (neighb != NULL)
        {
            int v = neighb->v;
            if (gr->vertices[v].index == -1)
            {
                tarjan_visit(gr, v, index, stk);
                gr->vertices[u].lowlink = min(gr->vertices[u].lowlink, gr->vertices[v].lowlink);
            }
            else if (gr->vertices[u].onstack)
            {
                gr->vertices[u].lowlink = min(gr->vertices[u].lowlink, gr->vertices[v].index);
            }
            neighb = neighb->next;
        }

        if (gr->vertices[u].lowlink == gr->vertices[u].index)
        {
            int aux;
            printf("CONNECTED COMPONENET: ");
            do 
            {
                aux = (*stk)->v;    //top element
                *stk = (*stk)->next;//goes to the next one
                gr->vertices[aux].onstack = false;
                printf("%d ", aux);

            } while (aux != u);
            printf("\n");
        }
    }

    void tarjan(graph* gr)
    {
        int index = 0;
        lists* stk = NULL;

        for (int i = 0; i < gr->v_nr; i++)
        {
            if (gr->vertices[i].index == -1)
            {
                tarjan_visit(gr, i, index, &stk);
            }
        }
    }

    void demonstrate(int size)
    {
        graph* gr = init_graph(7);
        edge(gr, 0, 1);
        edge(gr, 0, 2);
        edge(gr, 2, 1);
        edge(gr, 1, 3);
        edge(gr, 3, 2);
        edge(gr, 4, 3);
        edge(gr, 4, 5);
        edge(gr, 5, 5);
    
        printf("\n");
        print_graph(gr);

        lists* top_sort = NULL;
        dfs(gr, &top_sort);
        
            /*  DFS PRINT   */
        printf("\n");
        for (int i = 0;i < 7;i++)
        {
            if (gr->vertices[i].parent == -1)
            {
                printare(gr, i, 0);
            }
        }
            /*  TOPOLOGICAL SORT PRINT */
        printf("\n");
        print_list(top_sort);

            /*  TARJAON PRINT   */
        printf("\n");
        tarjan(gr);

    }
    void generare_edge(graph* gr, int amount)
    {
        int edges_nr = 0;
        int edges_v[4500];
        int edges_u[4500];
        int aux = -1;
        while (edges_nr != amount)
        {
            int u = rand() % gr->v_nr;
            int v = rand() % gr->v_nr;

            int ok = 0;
            if (u != v)
            {
                for (int i = 0;i < aux;i++)
                {
                    if (edges_v[i] == v and edges_u[i] == u) ok = 1;
                }
                if (ok == 0)
                {
                    aux++;
                    edge(gr, u, v);
                    edges_v[aux] = v;
                    edges_u[aux] = u;
                    edges_nr++;
                }
            }
        }
    }
    void performance(Profiler& profiler, AnalysisCase whichCase)
    {
        int V = 100;
        int E = 4500;

        for (int i = 1000;i <= 4500;i += 100)
        {
            graph* gr = init_graph(V);
            Operation var_edges = profiler.createOperation("var_edges", i);
            generare_edge(gr, i);
            lists* top_sort = NULL;
            dfs(gr, &top_sort,&var_edges);
        }

        for (int i = 100;i <= 200;i += 10)
        {
            graph* gr = init_graph(i);
            Operation var_nodes = profiler.createOperation("var_nodes", i);
            generare_edge(gr, E);
            lists* top_sort = NULL;
            dfs(gr, &top_sort, &var_nodes);
        }
        
    }

} // namespace lab10
