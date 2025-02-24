#include "MultiTree.h"

#include "catch2.hpp"

#define MAX_SIZE 10000

#include <iostream>

namespace lab06
{
    /*
                    LAB06
        GROUP 4             GOG RARES FLAVIU

        * PREORDER  : root -> left -> right
        * POSTORDER : left -> right -> root
        * INORDER   : left -> root -> right
        * O(n) T1 and T2
        * 
        * 
    */

    

    void recPreorder(nodet* root, int* array, int* index, Operation* ops)  //operation count when added in print array
    {
        if (root!=NULL)
        {
            if (ops)ops->count();
            array[(*index)++] = root->value;
            recPreorder(root->left, array, index, ops);
            recPreorder(root->right, array, index, ops);
            //root -> left -> right
        }
    }

    void recPostorder(nodet* root, int* array, int* index, Operation* ops) //operation count when added in print array
    {
        if (root!=NULL)
        {
            recPostorder(root->left, array, index, ops);
            recPostorder(root->right, array, index, ops);
            if (ops)ops->count();
            array[(*index)++] = root->value;
            //left->right->root
        }
    }

    void recInorder(nodet* root, int* array, int* index, Operation* ops) //operation count when added in print array
    {
        if (root!=NULL)
        {
            recInorder(root->left, array, index, ops);
            if (ops)ops->count();
            array[(*index)++] = root->value;
            recInorder(root->right, array, index, ops);
            //left->root->right
        }
    }

    void itePreorder(nodet* root, int* array, int* index, Operation* ops) //operation count when added in print array
    {
        if (root==NULL) return;
        int aux = -1;
        //root -> left -> right
        nodet* buff[MAX_SIZE]; //buffer used as a stack
        buff[++aux] = root;

        while (aux >= 0)
        {
            nodet* actual = buff[aux--]; //pop

            if (ops)ops->count();
            array[(*index)++] = actual->value;

            if (actual->right) buff[++aux] = actual->right;     //push
            if (actual->left) buff[++aux] = actual->left;       //push
        }
    }

    void itePostorder(nodet* root, int* array, int* index, Operation* ops) //operation count when added in print array
    {
        if (root==NULL) return;
        nodet* buff[MAX_SIZE]; //stack
        int aux = -1;
        nodet* actual = root;
        //left -> right ->root
        do
        {
            while (actual !=NULL)
            {
                if (actual->right)
                {
                    buff[++aux] = actual->right; //push
                }
                buff[++aux] = actual; //push node
                actual = actual->left;
            }

            actual = buff[aux--]; //pop node

            if (aux >= 0 and buff[aux] == actual->right)
            {
                aux--;
                buff[++aux] = actual; //push node
                actual = actual->right;
            }
            else
            {
                if (ops)ops->count();
                array[(*index)++] = actual->value;
                actual = NULL;
            }
        } while (aux >= 0);
    }

    void iteInorder(nodet* root, int* array, int* index, Operation* ops) //operation count when added in print array
    {
        if (!root) return;
        nodet* buff[MAX_SIZE];  //stack
        int aux = -1;
        nodet* actual = root;
        //left -> root -> right
        while (actual!=NULL or aux >= 0)
        {
            while (actual !=NULL)
            {
                buff[++aux] = actual;  //push node
                actual = actual->left;
            }
            actual = buff[aux--]; //pop

            if (ops)ops->count();
            array[(*index)++] = actual->value;

            actual = actual->right;

        }
    }

    nodet* createNode(int value)
    {
        nodet* node = (nodet*)malloc(sizeof(nodet));
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->value = value;
        return node;
    }

    MultiNode* createMultiNode(int value)
    {
        MultiNode* node = (MultiNode*)malloc(sizeof(MultiNode));
        node->value = value;
        node->kidsNumber = 0;
        return node;
    }

    nodet* createTree(int* array, int size)
    {
        if (size == 0) return NULL;

        nodet* nodes[MAX_SIZE];
        for (int i = 0;i < size;i++)
        {
            nodes[i] = createNode(array[i]);
        }
        for (int i = 0; i < size; i++)
        {
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < size)
            {
                nodes[i]->left = nodes[left];
                nodes[left]->parent = nodes[i];
            }
            if (right < size)
            {
                nodes[i]->right = nodes[right];
                nodes[right]->parent = nodes[i];
            }
        }
        return nodes[0];
    }

    void printare(const char* type, int* array, int size)
    {
        std::cout<<type<<":\n";

        for (int i = 0;i < size;i++)
            std::cout << array[i] << " ";
        
        std::cout << "\n";
    }

    void R1(int* tree, int size, int parent, int tab)
    {
        for (int i = 0; i < size; i++)
        {
            if (tree[i] == parent) 
            {
                for (int j = 0;j < tab;j++)
                    std::cout << "  ";

                std::cout<< i + 1 << std::endl;
                R1(tree, size, i + 1, tab + 1);
            }
        }
    }

    MultiNode* T1(int parent[], int n)
    {
        MultiNode* nodes[100];
        MultiNode* root = NULL;

        for (int i = 1;i <= n;i++)
            nodes[i] = createMultiNode(i);

        for (int i = 1;i <= n;i++)
        {
            if (parent[i-1] == -1)
                root = nodes[i];
            else
            {
                MultiNode* newNode = nodes[parent[i-1]];
                newNode->child[newNode->kidsNumber++] = nodes[i];
            }
        }
        return root;
    }

    void R2(MultiNode* root, int h)
    {
        if (root == NULL) return;
       
        for (int i = 0; i < h; i++) std::cout<<"  ";
        std::cout << root->value << "\n";

        for (int i = 0; i < root->kidsNumber; i++) 
            R2(root->child[i], h + 1);
     
    }

    nodet* T2(MultiNode* root) 
    {
        if (root == NULL) return NULL;
        nodet* Root = createNode(root->value);

        if (root->kidsNumber > 0) 
        {
            Root->left = T2(root->child[0]);
            nodet* current = Root->left;

            for (int i = 1; i < root->kidsNumber; i++) 
            {
                current->right = T2(root->child[i]);
                current->right->parent = Root;
                current = current->right;
            }
        }
        return Root;
    }

    void R3(nodet* root, int h)
    {
        if (root == NULL) return;
        R3(root->right, h + 1);

        for (int i = 0;i < h;i++)
            std::cout << "  ";
        
        std::cout << root->value << "\n";
        R3(root->left, h + 1);
    }

    void demonstrate(int n, int k)
    {
        // TODO: Write a small demo for the list operations, list generation and merging functions.
        int preorder[100], postorder[100], inorder[100];
        int a[] = { 11,34,21,23,54,39,71 };
        nodet* root = createTree(a, 7);

        std::cout << "Pretty print for the traversal tree: \n";
        R3(root, 0);
        std::cout << "\n";

                        /*TRAVERSARI*/
        int i = 0;
        recPreorder(root, preorder, &i);
        printare("RECURSIVE PREORDER", preorder, i);
        std::cout << "\n";

        i = 0;
        recPostorder(root, postorder, &i);
        printare("RECURSIVE POSTORDER", postorder, i);
        std::cout << "\n";

        i = 0;
        recInorder(root, inorder, &i);
        printare("RECURSIVE INORDER", inorder, i);
        std::cout << "\n";

        i = 0;
        itePreorder(root, preorder, &i);
        printare("ITERATIVE PREORDER", preorder, i);
        std::cout << "\n";

        i = 0;
        itePostorder(root, postorder, &i);
        printare("ITERATIVE POSTORDER", postorder, i);
        std::cout << "\n";

        i = 0;
        iteInorder(root, inorder, &i);
        printare("ITERATIVE INORDER", inorder, i);
        std::cout << "\n";

                    /* R1/R2/R3/T1/T2 */
        int parentArray[] = { 2,7,5,2,7,7,-1,5,2 };

        std::cout << "R1 PrettyPrint: \n";
        R1(parentArray, 9,-1,0);

        std::cout << "\nR2 PrettyPrint: \n";
        MultiNode* start = T1(parentArray, 9);
        R2(start, 0);

        std::cout << "\nR3 PrettyPrint: \n";
        nodet* N = T2(start);
        R3(N, 0);
         
                /*Exemplu Preorder la R3*/
        i = 0;
        int data[100];
        itePreorder(N, data, &i);
        printare("PREORDER R3", data, i);


    }

    void performance(Profiler& profiler, ListsCase whichCase)
    {
        // TODO: Write your performance analysis here, as explained in the readme.
        // You should use profiler.createOperation to pass the counter to each sorting function
        for (int i = 100; i <= 10000;i += 100)
        {
            int valori[MAX_SIZE];
            int inorder[MAX_SIZE], preorder[MAX_SIZE], postorder[MAX_SIZE];
            int index = 0;
            FillRandomArray(valori, i, 10, 50000, true, UNSORTED);
            nodet* root = createTree(valori, i);


                                    /*ITERATIV*/
            Operation IteInorder = profiler.createOperation("IteInorder", i);
            iteInorder(root, inorder, &index, &IteInorder);

            index = 0;
            Operation ItePostorder = profiler.createOperation("ItePostorder", i);
            itePostorder(root, inorder, &index, &ItePostorder);

            index = 0;
            Operation ItePreorder = profiler.createOperation("ItePreorder", i);
            itePreorder(root, inorder, &index, &ItePreorder);

                                    /*RECURSIV*/
            index = 0;
            Operation RecInorder = profiler.createOperation("RecInorder", i);
            recInorder(root, inorder, &index, &RecInorder);

            index = 0;
            Operation RecPostorder = profiler.createOperation("RecPostorder", i);
            recPostorder(root, inorder, &index, &RecPostorder);

            index = 0;
            Operation RecPreorder = profiler.createOperation("RecPreorder", i);
            recPreorder(root, inorder, &index, &RecPreorder);

        }
        profiler.createGroup("ITERATIV", "IteInorder", "ItePostorder", "ItePreorder");
        profiler.createGroup("RECURSIV", "RecInorder", "RecPostorder", "RecPreorder");

    }

} // namespace lab06
