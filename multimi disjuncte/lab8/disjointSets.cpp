#include "disjointSets.h"

#include "catch2.hpp"

#include <iostream>

#define MAX_SIZE 10000
/*
                LAB 08
    GOG RARES FLAVIU GROUP 30424
*/
namespace lab08
{
    node* MAKE_SET(int x, Operation* ops)
    {
        node* N = (node*)malloc(sizeof(node));
        N->parent = N; 
        if (ops) ops->count();
        N->value = x;
        N->rank = 0;
        return N;
    }

    node* FIND_SET(node* x, Operation* ops)
    {
        if (ops) ops->count();
        if (x->parent != x)
            x->parent = FIND_SET(x->parent,ops);
        return x->parent;
    }

    void UNION(node* x, node* y, Operation* ops)
    {
        node* reprezentantX = FIND_SET(x,ops);
        node* reprezentantY = FIND_SET(y,ops);

        if (ops) ops->count();
        if (reprezentantX->rank > reprezentantY->rank)
        {
            reprezentantY->parent = reprezentantX;
        }
        else if (reprezentantX->rank < reprezentantY->rank)
        {
            reprezentantX->parent = reprezentantY;
        }
        else
        {
            reprezentantY->parent = reprezentantX;
            reprezentantX->rank++;
        }
    }

    void sortare(edge edges[], int amount)
    {
        for (int i = 0;i < amount;i++)
        {
            for (int j = 0;j < amount - i - 1;j++)
            {
                if (edges[j].weight > edges[j + 1].weight)
                {
                    std::swap(edges[j], edges[j + 1]); //bubble sort ordine minima
                }
            }
        }
    }

    int Kruskal_alg(int vertices_nr, int edges_nr, edge edges[],edge tree_edges[],Operation* MAKE_SET_ops, Operation* FIND_SET_ops, Operation* UNION_ops)
    {
        node* vertices[MAX_SIZE];
        for (int i = 0;i < vertices_nr;i++)
        {
            vertices[i] = MAKE_SET(i,MAKE_SET_ops);
        }

        sortare(edges, edges_nr);

        int edge_counter = 0;
        int aux_weight = 0;

        for(int i =0;i<edges_nr;i++)
        {
                int u = edges[i].u - 1;
                int v = edges[i].v - 1;

            if (FIND_SET(vertices[u],FIND_SET_ops) != FIND_SET(vertices[v],FIND_SET_ops)) //verificare ciclu
            {
                tree_edges[edge_counter++] = edges[i];
                aux_weight = aux_weight + edges[i].weight;

                UNION(vertices[u], vertices[v],UNION_ops);
            }

            if (edge_counter == vertices_nr - 1) break;
        }

        return aux_weight;
    }
    void demonstrate(int size)
    {
                            /* MAKESET~FIND_SET~UNION */
        int sets = 10;
        node* nodes[10];
        for (int i = 0;i < sets;i++)
            nodes[i] = MAKE_SET(i);

        std::cout << "Sets before UNION: \n";
        for (int i = 0;i < sets;i++)
        {
            std::cout << "node: " << nodes[i]->value << " ~parent: " << nodes[i]->parent->value;
            std::cout << std::endl;
        }
        std::cout << std::endl;

        for (int i = 0;i < sets / 2;i++)
        {
            UNION(nodes[i], nodes[sets - i - 1]); //union la primu cu ultimu si tot asa
        }

        std::cout << "Sets after UNION: \n";
        for (int i = 0;i < sets;i++)
        {
            std::cout << "node: " << nodes[i]->value << " ~parent: " << nodes[i]->parent->value;
            std::cout << std::endl;
        }
        std::cout << std::endl;

            /* KRUSKAL'S ALGORITHM EXEMPLE */
        int vertices_nr = 9;
        int edges_nr = 14;
        edge edges[] = { { 1,2,4 } , { 2,3,8 }, {3,4,7},{4,5,9 },{5,6,10},{4,6,14},{3,6,4},{6,7,2},{7,8,1},{7,9,6},{3,9,2},{8,9,7},{8,1,8},{2,8,11} };
        edge tree_edges[8];

        std::cout << "Graph print as ( vertex u , vertex v , edge weight ) : \n";
        for (int i = 0;i < edges_nr;i++)
        {
            std::cout << "(";
            std::cout << edges[i].u;
            std::cout << ",";
            std::cout << edges[i].v;
            std::cout << ",";
            std::cout << edges[i].weight;
            std::cout << ")";
            std::cout << "\n";
        }
        std::cout << "\n";

        std::cout << "Graph print as ( vertex u , vertex v , edge weight ) : \n";
       int minimum_weight = Kruskal_alg(vertices_nr, edges_nr, edges,tree_edges);
       for (int i = 0;i < vertices_nr - 1;i++)
       {
           std::cout << "(";
           std::cout << tree_edges[i].u;
           std::cout << ",";
           std::cout << tree_edges[i].v;
           std::cout << ",";
           std::cout << tree_edges[i].weight;
           std::cout << ")";
           std::cout << "\n";
       }
       std::cout << "WEIGHT: " << minimum_weight<<"\n";

    }

    void performance(Profiler& profiler, AnalysisCase whichCase)
    {
        edge edges[MAX_SIZE * 4];
        edge tree_edges[MAX_SIZE];
        int matrice[MAX_SIZE][MAX_SIZE];
        for (int i = 100;i <= 10000;i += 100)
        {
            Operation MAKE_SET_ops = profiler.createOperation("MAKE_SET_ops", i);
            Operation FIND_SET_ops = profiler.createOperation("FIND_SET_ops", i);
            Operation UNION_ops = profiler.createOperation("UNION_ops", i);
            int q = 0;
            int ed = 0;

            for (int a = 0;a < i;a++)
            {
                for (int b = 0;b < i;b++)
                {
                    matrice[a][b] = 0;
                }
            }

            while (ed < i * 4)
            {
                int a_aux = rand() % i;
                int b_aux = rand() % i;
                if (a_aux != b_aux and matrice[a_aux][b_aux] == 0)
                {
                    matrice[a_aux][b_aux] = 1;
                    matrice[b_aux][a_aux] = 1;
                    ed++;
                }
            }
            for (int a = 0;a < i;a++)
            {
                for (int b = 0;b < i;b++)
                {
                    if (matrice[a][b] == 1)
                    {
                        int weight = rand() % 1000 + 1;
                        matrice[b][a] = 0;
                        edges[q++] = { a + 1 , b + 1 , weight };
                    }
                }
            }
        
        int minimum_weight = Kruskal_alg(i, i * 4, edges, tree_edges, &MAKE_SET_ops, &FIND_SET_ops, &UNION_ops);
        }
         profiler.createGroup("Kruskal Ops", "MAKE_SET_ops", "FIND_SET_ops", "UNION_ops");
    }
} // namespace lab08
