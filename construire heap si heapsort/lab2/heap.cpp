#include "heap.h"

#include "catch2.hpp"

#include <iostream>


/*
                    DOCUMENTATIE
            GOG RARES-FLAVIU GRUPA 30424

      1. Insertion Sort folosit recursiv si iterativ are acelasi numar de operatii realizate.
      Din punct de vedere al timpului necesar, varianta iterativa este mai rapida, chiar daca 
      ambele variante au aceeasi complexitate in cazul cel mai rau.

      2. Varianta de construire a heap-ului Top-Down, unde am folosit swim,de complexitate
      O( log n ) ~ inaltimea unui arbore balansat, va avea complexitatea O (n log n).
      Varianta de construire a heap-ului Bottom-Up, unde am folosit sink, de complexitate
      O( log n ), va avea complexitatea O(n).

        Diferenta dintre cele doua tipuri de construire a unui heap se poate vedea in tabele.
      Din acestea se poate observa ca top-down va avea mai multe operatii realizate decat bottom-up.

*/



namespace lab02
{
    void swim(int* values, int i, int size, Operation* opAsg, Operation* opCmp)
    {

        while (i > 1 and values[i] > values[ i / 2])
        {
            if (opCmp != NULL)opCmp->count(); //comparison count ++
            if (opAsg != NULL) opAsg->count(3); // assignment count ++
            std::swap(values[i], values[ i / 2]);
            i = i/ 2;
        }
        if (opCmp != NULL and i > 1)opCmp->count(); //comparison count ++ ~ iesire din while
    }

    void sink(int* values, int i, int size, Operation* opAsg, Operation* opCmp)
    {
        int left = 2 * i;
        int right = 2 * i + 1;
        int smol = i;
        if (opCmp != NULL)opCmp->count(); //comparison count ++
        if (left <= size and values[left] > values[smol])
        {
            smol = left;
        }
        if (opCmp != NULL)opCmp->count(); //comparison count ++
        if (right <= size and values[right] > values[smol])
        {
            smol = right;
        }
        if (smol != i)
        {
            if (opAsg != NULL) opAsg->count(3); // assignment count ++
            std::swap(values[i], values[smol]);

            sink(values, smol, size,opAsg,opCmp);
        }
    }

    void iterativeSort(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        for (int i = 1; i < n;i++)
        {
            if (opAsg != NULL)opAsg->count(); //assign
            int element = values[i]; //extract operation
            int j = i - 1;
            // dont count assig for indexes
            while (values[j] > element and j >= 0)
            {
                if (opCmp != NULL)opCmp->count(); //comparison count ++
                if (opAsg != NULL) opAsg->count(); // assignment count ++
                values[j + 1] = values[j];
                j--;
            }
            if (opCmp != NULL) opCmp->count();//comparison count +1 atunci cand iese din while mai face o comparatie
            if (opAsg != NULL and j + 1 != i) opAsg->count(); //assign
            if (j + 1 != i) values[j + 1] = element;
        }
    }

    void recursiveSort(int* values, int n, int i, Operation* opAsg, Operation* opCmp)
    {
        if (i == n) return;
        if (opAsg != NULL)opAsg->count(); //assign
        int element = values[i]; //extract operation
        int j = i - 1;
        // dont count assig for indexes
        while (values[j] > element and j >= 0)
        {
            if (opCmp != NULL)opCmp->count(); //comparison count ++
            if (opAsg != NULL) opAsg->count(); // assignment count ++
            values[j + 1] = values[j];
            j--;
        }
        if (opCmp != NULL) opCmp->count();//comparison count +1 atunci cand iese din while mai face o comparatie
        if (opAsg != NULL and j + 1 != i) opAsg->count(); //assign
        if (j + 1 != i) values[j + 1] = element;
        recursiveSort(values, n , i + 1, opAsg, opCmp);

    }
   
    void buildHeap_BottomUp(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        for (int i = n / 2;i >= 1;i--)
        {
            sink(values, i, n,opAsg,opCmp);
        }
    }

    void buildHeap_TopDown(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        for (int i = 2;i <= n;i++)
        {
            swim(values, i, n,opAsg,opCmp);
        }
    }

    void heapSort(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        buildHeap_BottomUp(values, n, opAsg, opCmp);
        for (int i = n;i > 1;i--)
        {
            std::swap(values[1], values[i]);
            sink(values, 1 , i-1, opAsg, opCmp);
        }
    }

    void demonstrate(int size)
    {
                /* Iterative and Recursive Demo */
        int data1[] = {10,9,8,6,4,32,1,8,7,10};
        int data2[] = { 10,9,8,6,4,32,1,8,7,10 };
        int numar_data = 10;
        std::cout << "Initial Values: ";
        for (int i = 0;i < 10;i++)
            std::cout << data1[i] << " ";
        std::cout << std::endl;
        
        recursiveSort( data1 , numar_data , 0);
        std::cout << "Recursive Insertion Sort: ";
        for (int i = 0;i < 10;i++)
            std::cout << data1[i] << " ";
        std::cout << std::endl;
        iterativeSort(data2, numar_data);
        std::cout << "Iterative Insertion Sort: ";
        for (int i = 0;i < 10;i++)
            std::cout << data2[i] << " ";
        std::cout << std::endl;

                /* BottomUP Demo */
        std::cout << std::endl;
        int btup[] = { 0,51,3,82,4,12,0 };
        std::cout << "Before BottomUP Heap: ";
        for (int i = 1;i <= 6;i++)
        {
            std::cout << btup[i] << " ";
        }
        std::cout << std::endl;
        buildHeap_BottomUp(btup, 6);
        std::cout << "After BottomUP Heap: ";
        for (int i = 1;i <= 6;i++)
        {
            std::cout << btup[i] << " ";
        }
        std::cout << std::endl;

                /* TopDown Demo */
        std::cout << std::endl;
        int topd[] = { 0,51,3,82,4,12,0 };
        std::cout << "Before TopDown Heap: ";
        for (int i = 1;i <= 6;i++)
        {
            std::cout << topd[i] << " ";
        }
        std::cout << std::endl;
        buildHeap_TopDown(topd, 6);
        std::cout << "After TopDown Heap: ";
        for (int i = 1;i <= 6;i++)
        {
            std::cout << topd[i] << " ";
        }
        std::cout << std::endl;

            /* HeapSort Demo */

        std::cout << std::endl;
        int valori[] = { 0,21,2,31,1,3,4 };
        std::cout << "Before HeapSort: ";
        for (int i = 1;i <= 6;i++)
        {
            std::cout << valori[i] << " ";
        }
        std::cout << std::endl;
        heapSort(valori, 6);
        std::cout << "After HeapSort: ";
        for (int i = 1;i <= 6;i++)
        {
            std::cout << valori[i] << " ";
        }
        std::cout << std::endl;
    }

    TEST_CASE("Heap methods")
    {
           int data[100],data1[100];
           FillRandomArray(data, 100);
           iterativeSort(data, 100);
           REQUIRE( IsSorted(data, 100) );

           FillRandomArray(data1, 100);
           recursiveSort(data1, 100, 0);
           REQUIRE( IsSorted(data1, 100));
    }

    void performance(Profiler& profiler, AnalysisCase whichCase)
    {
        int a[10001], b[10001], c[10001], d[10001];
        if (whichCase == AVERAGE)
        {
            for (int i = 1;i <= 100;i++)
            {
                int aux = 100 * i;
                FillRandomArray(a, aux);
                CopyArray(b, a, aux);
                CopyArray(c, a, aux);
                CopyArray(d, a, aux);

                Operation ItInsertionOp = profiler.createOperation("ItInsertionOp", aux);
                iterativeSort(a, aux, &ItInsertionOp, &ItInsertionOp);

                Operation ReInsertionOp = profiler.createOperation("ReInsertionOp", aux);
                iterativeSort(b, aux, &ReInsertionOp, &ReInsertionOp);

                Operation BottomUpOps = profiler.createOperation("BottomUpOps", aux);
                buildHeap_BottomUp(c, aux, &BottomUpOps, &BottomUpOps);

                Operation TopDownOps = profiler.createOperation("TopDownOps", aux);
                buildHeap_TopDown(d, aux, &TopDownOps, &TopDownOps);
            }
            profiler.createGroup("Total Operations Insertion", "ItInsertionOp", "ReInsertionOp");
            profiler.createGroup("Total Ops AVG-Heap","BottomUpOps","TopDownOps");



        }
        else if (whichCase == WORST)
        {
            for (int i = 1;i <= 100;i++)
            {
                int aux = 100 * i;
                FillRandomArray(a, aux, 10, 50000, false, DESCENDING);
                CopyArray(b, a, aux);

                Operation BottomUpOpsW = profiler.createOperation("BottomUpOpsW", aux);
                buildHeap_BottomUp(a, aux, &BottomUpOpsW, &BottomUpOpsW);

                Operation TopDownOpsW = profiler.createOperation("TopDownOpsW", aux);
                buildHeap_TopDown(b, aux, &TopDownOpsW, &TopDownOpsW);

            }
            profiler.createGroup("Total Ops W-Heap", "BottomUpOpsW", "TopDownOpsW");
        }

    }
    void benchmark(Profiler& profiler, AnalysisCase whichCase)
    {
        int nr_tests = 1000;
        int var[10001];
        for (int aux = 100;aux <3000;aux +=100)
        {
            profiler.startTimer("iterativeSort",aux );
            for (int test = 0;test < nr_tests; test++)
            {
                FillRandomArray(var, aux,10,50000,false,UNSORTED);
                iterativeSort(var, aux);
            }
            profiler.stopTimer("iterativeSort",aux);

            profiler.startTimer("recursiveSort", aux);
            for (int test = 0;test < nr_tests; test++)
            {
                FillRandomArray(var, aux,10,50000,false,UNSORTED);
                recursiveSort(var, aux, 0);
            }
            profiler.stopTimer("recursiveSort", aux);
        }
        profiler.createGroup("Benchmark Test","iterativeSort","recursiveSort");
    }

} // namespace lab02
