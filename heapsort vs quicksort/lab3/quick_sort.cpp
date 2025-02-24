#include "quick_sort.h"

#include "catch2.hpp"

#include <iostream>

/*
                    LAB03
    GOG RARES-FLAVIU    GROUP 30424

    *QuickSort are complexitatea O (n log n), sau O(n^2) in cel mai rau caz.
    *Hybrid Quicksort are complexitatea O(n^2), dar prelucreaza datele mai rapid
    pentru siruri mici.
    *Heapsort este relativ mai ineficient pentru sortarea unui sir.
    *QuickSort se bazeaza pe "divide & conquer",alege un pivot si creeaza partitii 
    pentru a sorta sirul.
    *


*/

namespace lab03
{   
                    /* Insertion Sort  */
    void insertionSort(int* values, int left,int right, Operation* opAsg, Operation* opCmp)
    {
        for (int i = left; i <= right;i++)
        {
            if (opAsg != NULL)opAsg->count(); 
            int element = values[i];
            int j = i - 1;
     
            while (values[j] > element and j >= 0)
            {
                if (opCmp != NULL)opCmp->count(); 
                if (opAsg != NULL) opAsg->count(); 
                values[j + 1] = values[j];
                j--;
            }
            if (opCmp != NULL) opCmp->count();
            if (opAsg != NULL and j + 1 != i) opAsg->count();
            if (j + 1 != i) values[j + 1] = element;
        }
    }

                                    /* Heapsort */
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

            sink(values, smol, size, opAsg, opCmp);
        }
    }


    void buildHeap_BottomUp(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        for (int i = n / 2;i >= 1;i--)
        {
            sink(values, i, n, opAsg, opCmp);
        }
    }

    void heapSort(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        buildHeap_BottomUp(values, n, opAsg, opCmp);
        for (int i = n;i > 1;i--)
        {
            if (opAsg != NULL) opAsg->count();
            std::swap(values[1], values[i]);
            sink(values, 1, i - 1, opAsg, opCmp);
        }
    }
    

                        /* QuickSort */
    int partition(int* values, int left, int right, Operation* opAsg,Operation* opCmp)
    {
        if (opAsg != NULL) opAsg->count();
        int fix = values[right];
        int i = left - 1;
        for (int j = left; j <= right - 1;j++)
        {
            if (opCmp != NULL) opCmp->count();
            if (fix >= values[j])
            {
                i++;
                if (opAsg != NULL) opAsg->count(3);
                std::swap(values[i], values[j]);
            }
        }
        if (opAsg != NULL) opAsg->count(3);
        std::swap(values[i + 1], values[right]);
        return (i + 1);

    }
    void quickSort(int* values,int left ,int right , Operation* opAsg, Operation* opCmp)
    {
        if (left >= right) return;
        
            int q = partition(values, left, right, opAsg, opCmp);
            quickSort(values,left,q - 1, opAsg, opCmp);
            quickSort(values,q + 1, right, opAsg, opCmp);
     
    }
    
    void WorstCaseQuickSort(int* values, int left, int right, int* array) 
    {
        int aux = 0;
        if (left > right)
            return;

        int mid = left + (right - left) / 2;

        array[aux] = values[mid];
        aux++;

        WorstCaseQuickSort(values, left, mid - 1, array);
        WorstCaseQuickSort(values, mid + 1, right, array);
    }

    void BestCaseQuickSort(int* values, int left, int right)
    {
        int mid = left + (right - left) / 2;
        if (left < right)
        {
            BestCaseQuickSort(values, left, mid - 1 );
            BestCaseQuickSort(values, mid + 1, right);
            std::swap(values[mid], values[right]);
        }
    }

    void hybridizedQuickSort(int* values, int left,int right,int threshold, Operation* opAsg, Operation* opCmp)
    {
        if (right-left+1 <= threshold)
        {
            insertionSort(values, left,right, opAsg, opCmp);
        }
        else  if (left >= right) return;
        else 
        {
            int q = partition(values, left, right, opAsg, opCmp);
            hybridizedQuickSort(values, left, q - 1, threshold, opAsg, opCmp);
            hybridizedQuickSort(values, q + 1, right, threshold, opAsg, opCmp);
        }
    }

    void quickSelect(int* values, int n, Operation* opAsg, Operation* opCmp)
    {
        // TODO: Quick select implementation here
    }

    void demonstrate(int size)
    {   
                     /* QuickSort */
        int data[] = {1,2,32,12,10,0,7,3,4,5,6,89};
        int marime = 12;
        std::cout <<"Values before QuickSort: ";
        for (int i = 0;i < marime;i++)
        {
        std:: cout << data[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Values after QuickSort: ";
        quickSort(data, 0, marime - 1);
        for (int i = 0;i < marime;i++)
        {
            std::cout << data[i] << " ";
        }

        std::cout << std::endl;
        std::cout << std::endl;

                    /* Hybrid QuickSort */
        int data1[] = { 1,2,32,12,10,0,7,12,67,45,11,62 };
        int marime1 = 12;
        std::cout << "Values before Hybrid QuickSort: ";
        for (int i = 0;i < marime1;i++)
        {
            std::cout << data1[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Values after Hybrid QuickSort: ";
        hybridizedQuickSort(data1, 0,marime1-1,15);
        for (int i = 0;i < marime1;i++)
        {
            std::cout << data1[i] << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    TEST_CASE("Quick sort")
    {

           int data[101];
           FillRandomArray(data, 100);
           quickSort(data,0,100);
           REQUIRE( IsSorted(data, 100) );

           int data1[101];
           FillRandomArray(data1, 100);
           hybridizedQuickSort(data1,0,100,10);
           REQUIRE(IsSorted(data1, 100));
    }

    void performance(Profiler& profiler, AnalysisCase whichCase)
    {

        if (whichCase == AVERAGE)
        {
            for (int i = 1;i <= 100;i++)
            {
                int space = i * 100;
                int a[10001],b[10001],c[10001];
                FillRandomArray(a, space,10,50000,false,UNSORTED);
                CopyArray(b, a, space);
                CopyArray(c, a, space);

                Operation QuickSortOp = profiler.createOperation("QuickSortOp", space);
                quickSort(a, 0,space, &QuickSortOp, &QuickSortOp);

                Operation HybridQuickSortOp = profiler.createOperation("HybridQuickSortOp", space);
                hybridizedQuickSort(b, 0,space, 15 ,&HybridQuickSortOp, &HybridQuickSortOp);

                Operation HeapSortOp = profiler.createOperation("HeapSortOp", space);
                heapSort(c, space, &HeapSortOp, &HeapSortOp);

            }
            profiler.createGroup("AVG OPS", "QuickSortOp", "HybridQuickSortOp", "HeapSortOp");
            
            for (int j = 0;j <=50;j ++)
               for (int i = 2; i <= 50; i++)
                {
                    int a[10001], b[10001];
           
                    Operation Insertion = profiler.createOperation("Insertion", i);
                    Operation HQsort = profiler.createOperation("HQsort", i);

                    FillRandomArray(a, i);
                    CopyArray(b, a, i);

                    hybridizedQuickSort(a, 0,i,15, &HQsort, &HQsort);
                    insertionSort(b, 0,i, &Insertion, &Insertion);
                }
            profiler.divideValues("HQsort", 50);
            profiler.divideValues("Insertion", 50);

            profiler.createGroup("Difference HQS IS", "Insertion", "HQsort");

            
            int dat[10001],q[10001];
            FillRandomArray(dat, 10000);
            for ( int t = 0;t <= 50;t ++)
            {
                CopyArray(q, dat,10000);
                Operation CountTh = profiler.createOperation("CountTh", t);
                hybridizedQuickSort(q,0,10000, t, &CountTh, &CountTh);
            }
            profiler.createGroup("Operations for diff threshold","CountTh");
            
        }
        else if(whichCase == BEST)
        {
            for (int i = 1;i <= 100;i++)
            {
                int space = i * 100;
                int a[10001], b[10001];
                FillRandomArray(a, space, 10, 50000, false, ASCENDING);
                BestCaseQuickSort(a, 0, space - 1);
                CopyArray(b, a, space);

                Operation QuickSortOpBEST = profiler.createOperation("QuickSortOpBEST", space);
                quickSort(b, 0, space, &QuickSortOpBEST, &QuickSortOpBEST);

            }
            profiler.createGroup("BEST OPS", "QuickSortOpBEST");
        }
        else
        {
            for (int i = 1;i <= 100;i++)
            {
                int space = i * 100;
                int a[10001], c[10001];
                FillRandomArray(a, space, 10, 50000, false, ASCENDING);
                WorstCaseQuickSort(a, 0, space - 1, c);

                Operation QuickSortOpWORST = profiler.createOperation("QuickSortOpWORST", space);
                quickSort(c, 0, space, &QuickSortOpWORST, &QuickSortOpWORST);

            }
            profiler.createGroup("WORST OPS", "QuickSortOpWORST");
        }
    }

    void benchmark(Profiler& profiler, AnalysisCase whichCase)
    {
        int a[10001];
        for (int i = 100; i <= 10000; i += 100)
        {
            profiler.startTimer("qsort", i);
            for (int test = 0; test < 100; ++test) 
            {
                FillRandomArray(a, i, 10, 50000, false, UNSORTED);
                quickSort(a, 0, i);
            }
            profiler.stopTimer("qsort", i);

            profiler.startTimer("hqsort", i);
            for (int test = 0; test < 100; ++test) 
            {
                FillRandomArray(a, i, 10, 50000, false, UNSORTED);
                hybridizedQuickSort(a, 0,i,15);
            }
            profiler.stopTimer("hqsort", i);
        }
        profiler.createGroup("Benchmark", "qsort", "hqsort");

    }
    

} // namespace lab03
