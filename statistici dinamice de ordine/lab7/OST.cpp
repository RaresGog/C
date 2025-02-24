#include "OST.h"

#include "catch2.hpp"

#include <iostream>

/*
                    lab07
        GOG RARES FLAVIU      GROUP4
*create O(n)
*select O(log n)
* delete O(h)
*/

namespace lab07
{
    node* build_tree(int left,int right,Operation* ops) //seminar
    {
        if (left>right) return NULL;

        int mid = 0;
        if(right%2==1)
            mid = (left+right) / 2;
        else 
            mid = (left + right+1) / 2;

        node* root = (node*)malloc(sizeof(node));
        if (ops)ops->count();
        root->value = mid;

        root->left = build_tree(left, mid - 1, ops);
        root->right = build_tree(mid + 1, right, ops);

        root->size = 1;
        if (root->left != NULL)root->size = root->size + root->left->size;
        if (root->right != NULL)root->size = root->size + root->right->size;
        return root;
    }

    node* os_select(node* root, int index, Operation* ops)
    {   
        if (root == NULL) return NULL;
        int aux;
        if (root->left != NULL) 
            aux = root->left->size;
        else aux = 0;
        
        if (ops)ops->count();
        int r = aux + 1;
        
        if (index == r)
        {
            return root;
        }
        else if (index < r)
        {
            return os_select(root->left, index, ops);
        }
        else
        {   
            return os_select(root->right, index - r, ops);
        }
    }

    node* os_delete(node* root,int value, Operation* ops)
    {
        if (root == NULL) return NULL;
        
        if (value < root->value) //mai mic merge la stanga
        {
            root->left = os_delete(root->left, value, ops);
        }
        else if (value > root->value) //mai mare merge la dreapta
        {
            root->right = os_delete(root->right, value, ops);
        }
        else // egal
        {
            if (ops)ops->count();
            if (root->left == NULL && root->right == NULL) //fara copii
            {
                free(root);
                return NULL;
            }
            if (ops)ops->count();
            if (root->left == NULL) //un copil
            {
                node* aux = root->right;
                free(root);

                return aux;
            }
            else
            {
                if (ops)ops->count();
                if (root->right == NULL) //un copil
                {
                    node* aux = root->left;
                    free(root);

                    return aux;
                }
            }
            //2 copii
            node* next = root->right; // o data dreapta
            while (next->left != NULL) 
                next = next->left; //dupa numai stanga
            if (ops)ops->count();
            root->value = next->value;
            root->right = os_delete(root->right, next->value, ops); //delete succesor
        }

        root->size = 1;  //modificare size dupa delete 
        if (root->left != NULL) root->size = root->size + root->left->size;
        if (root->right != NULL) root->size = root->size + root->right->size;

        return root;
    }

    void pretty_print(node* root, int h)
    {
        if (root == NULL) return;
        pretty_print(root->right, h + 1);

        for (int i = 0;i < h;i++) std::cout << "   ";
        std::cout << root->value << "\n";

        pretty_print(root->left, h + 1);
    }

    void demonstrate(int size)
    {
        node* root = build_tree(1, size);

        std::cout << "Initial tree:\n";
        pretty_print(root, 0);
        std::cout << "\n";

        for (int i = 0;i < 3;i++)
        {
            int select = rand() % (size-i) + 1;
            std::cout << "Selecting the " << select << "th smallest element:\n";
            node* N = os_select(root, select);
            if (N != NULL) std::cout << "Value: " << N->value << "\n";

            std::cout << "Deleting the " << select << "th smallest element:\n";
            root = os_delete(root, N->value);
            pretty_print(root, 0);
            std::cout << "\n\n";
        } 
    }

    void performance(Profiler& profiler, AnalysisCase whichCase)
    {
        for (int i = 0;i < 5;i++)
        {
            for (int n = 100;n <= 10000;n += 100)
            {
                Operation createOps = profiler.createOperation("createOps", n);
                Operation selectOps = profiler.createOperation("selectOps", n);
                Operation deleteOps = profiler.createOperation("deleteOps", n);

                node* root = build_tree(1, n,&createOps);
                for (int t = 0; t<n ; t++)
                {
                    int select = rand() % (n - t) + 1;
                    node* N = os_select(root, select,&selectOps);
                    if(N)root = os_delete(root, N->value,&deleteOps);
                }
            }
        }
        profiler.divideValues("createOps", 5);
        profiler.divideValues("selectOps", 5);
        profiler.divideValues("deleteOps", 5);
        profiler.addSeries("TotalOps", "selectOps", "deleteOps");

        profiler.createGroup("OST", "createOps", "selectOps", "deleteOps","TotalOps");
    }

} // namespace lab03
