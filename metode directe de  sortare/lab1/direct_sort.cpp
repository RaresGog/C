#include "direct_sort.h"
#include "catch2.hpp"

#include <iostream>

/*
          DOCUMENTATIE LAB_1
     GOG RARES-FLAVIU GRUPA 30424

    1.  "Bubble Sort" este un algoritm care schimba elementele adiacente intre ele
    pentru a sorta (crescator) sirul de elemente primit. In cazul cel mai rau, complexitatea
    algoritmului este O(n^2) (atunci cand sirul e descrescator) deoarece in prima citire face 
    n-1 pasi, in a doua n-2 si tot asa. Astfel, avem n(n-1)/2 comparatii realizate in acest caz.
    In cazul cel mai bun avem complexitatea O(n) deoarece algoritmul va verfica daca elementele 
    sunt asezate corespunzator ,astfel va face n-1 comparatii.
        In ceea ce priveste numarul de atribuiri, in cel mai rau caz va avea 3n(n-1)/2 si in cel 
    mai bun caz numarul atribuirilor este 0.    
        EX: 3*100*(100-1)/2=14850  (WORST)   (pentru fiecare comparatie face si o atribuire 1:3 )
    Algoritmul este stabil, elementele is pastreaza ordinea.

    2.  "Selection Sort" este un algoritm ce selecteaza cel mia mic element din partea nesortata
    si il schimba cu primul element din aceeasi parte. In ambele cazuri, complexitatea este O(n^2) 
    deoarece in prima trecere prin sir va face n-1 comparatii, in a doua n-2 comparatii si tot asa. 
    Astfel numarul de comparatii si media cazurilor este n(n-1)/2. Ordinea elementelor nu conteaza 
    pentru acest tip de sortare.
       In acest algoritm numarul atribuirilor este 0 in cel mai bun caz,iar in cel mai rau caz
    va fi 3(n-1) deoarece algoritmul va face n-1 schimbari.
    EX: 100,99,98,...,51,1,50,49,48,...,2
    Algoritmul nu este stabil deoarece elementele egale nu isi pastreaza ordinea.

    3.  "Insertion Sort" este un algoritm ce imparte sirul in doua: partea sortata si partea nesortata.
    Elementele din stanga elementului la care am ajus vor fi sortate si cele din dreapta vor fi nesortate.
    Comparam elementul cu cele sortate pentru ai gasi locul in aceasta parte.
        In cazul cel mai rau, algoritmul va compara fiecare element cu toate elementele din partea sortata.
    Astfel, vom avea n(n-1)/2 comparatii plus inca n-1 pentru a iesi din "while", complexitatea fiind O(n^2).
    In cazul cel mai bun, vom  avea numai n-1 comparari deoarece sirul este deja crescator si nu vom intra in 
    "while", astfel complexitatea este O(n).
        Numarul atribuirilor, in cel mai bun caz va fi n-1 deoarece numai selectam fiecare element pe rand, 
    iar in cazul cel mai rau, vom avea:
        * n-1 atribuiri -> alegem elementul
        * n(n-1)/2 atribuiri -> mutari ale elementlor
        * n-1 atribuiri -> asezarea elementului
        EX: 99 + 4950 + 99 = 5148 ~ pentru vector cu marimea de 100 de elemente

*/


namespace lab01
{

void bubbleSort(int* values, int n, Operation* opAsg, Operation* opCmp)
{
    bool verificare;
    do 
    {
        verificare = false;
        int i = 1;
        while(i<n)
        {
            if (opCmp != NULL)opCmp->count();//comp
            if (values[i] < values[i - 1])
            {
                if (opAsg != NULL) opAsg->count(3);//assign
                std::swap(values[i], values[i - 1]); 
                verificare = true;
            }
            i++;
        }
        n--;
    } while (verificare == true); // continue if there was a swap
}

void selectionSort(int* values, int n, Operation* opAsg, Operation* opCmp)
{
    // nu e stabil 7(1) 7(2) 1(3) -> 1(3) 7(2) 7(1) 
    for (int i = 0;i < n - 1;i++)
    {
        int pozitia = i;
        for (int j = i + 1;j < n;j++)
        {    
            if (opCmp != NULL)opCmp->count();   //comp
            if (values[j] < values[pozitia])
            {
            pozitia = j;
            }
        }
        if (opAsg != NULL and pozitia != i)opAsg->count(3); //assign
       if(pozitia!= i)
        std:: swap(values[pozitia], values[i]);
    }
}
void insertionSort(int* values, int n, Operation* opAsg, Operation* opCmp)
{
    for (int i = 1; i < n;i++)
    {
        if (opAsg != NULL)opAsg->count(); //assign
        int element = values[i]; //extract operation
        int j = i - 1;
        // dont count assig for indexes
        while (values[j] > element and j >= 0)
        {
            if(opCmp!= NULL)opCmp->count(); //comparison count ++
            if (opAsg != NULL) opAsg->count(); // assignment count ++
            values[j + 1] = values[j];
            j--;
        }
        if (opCmp != NULL) opCmp->count();//comparison count +1 atunci cand iese din while mai face o comparatie
        if (opAsg != NULL and j + 1 != i) opAsg->count(); //assign
       if(j+1 != i) values[j + 1] = element;
    }
}

void binaryInsertionSort(int* values, int n, Operation* opAsg, Operation* opCmp)
{
    // TODO: Binary Insertion sort implementation here
}

void demonstrate(int size)
{
    int a[] = { 13,4,12,32,23,12 };
    int b[] = { 6,5,4,3,2,1 };
    int c[] = { 54,10,43,12,1,0 };

    std::cout <<"Original Array: ";
    for (int i = 0;i < 6;i++)
        std::cout << a[i] << " ";
    std::cout << std::endl;
    insertionSort(a,6);
    std::cout << "Insertion Sorted Array: ";
    for (int i = 0;i < 6;i++)
      std::  cout<<a[i] << " ";
    std::cout << std:: endl;
    std::cout << std::endl;

    std::cout << "Original Array: ";
    for (int i = 0;i < 6;i++)
        std::cout << b[i] << " ";
    std::cout << std::endl;
    selectionSort(b, 6);
    std::cout << "Selection Sorted Array: ";
    for (int i = 0;i < 6;i++)
        std::cout<< b[i] << " ";
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Original Array: ";
    for (int i = 0;i < 6;i++)
        std::cout << c[i] << " ";
    std::cout << std::endl;
    bubbleSort(c, 6);
    std::cout << "Bubble Sorted Array: ";
    for (int i = 0;i < 6;i++)
        std::cout<< c[i] << " ";
    std::cout << std::endl;
    std::cout << std::endl;
    
}

TEST_CASE("Direct sorting methods")
{
    int data1[100],data2[200],data3[300];

    FillRandomArray(data1, 100);
    insertionSort(data1, 100);
    REQUIRE(IsSorted(data1, 100));

    FillRandomArray(data2, 200);
    selectionSort(data2, 200);
    REQUIRE(IsSorted(data2, 200));

    FillRandomArray(data3, 300);
    bubbleSort(data3, 300);
    REQUIRE(IsSorted(data3, 300));
}

void performance(Profiler& profiler, AnalysisCase whichCase)
{
 
    int a[1000],b[1000],c[1000],d[1000];
    int n = 5;

    if (whichCase == AVERAGE)
    {
        for(int i = 1 ;i<=5;i++)
        {
            int aux = 100 * i;
            FillRandomArray(a, aux);

            Operation InsertionAssignment = profiler.createOperation("InsertionAssignment", aux);
            Operation InsertionComparison = profiler.createOperation("InsertionComparison", aux);
            CopyArray(b, a, aux);

            insertionSort(b, aux, &InsertionAssignment, &InsertionComparison);


            Operation SelectionAssignment = profiler.createOperation("SelectionAssignment", aux);
            Operation SelectionComparison = profiler.createOperation("SelectionComparison", aux);
            CopyArray(c, a, aux);
            selectionSort(c, aux, &SelectionAssignment, &SelectionComparison);


            Operation BubbleAssignment = profiler.createOperation("BubbleAssignment", aux);
            Operation BubbleComparison = profiler.createOperation("BubbleComparison", aux);
            CopyArray(d, a, aux);
            bubbleSort(d, aux, &BubbleAssignment, &BubbleComparison);

        }

        profiler.addSeries("TotalOperationInsertion", "InsertionAssignment", "InsertionComparison");
        profiler.addSeries("TotalOperationSelection", "SelectionAssignment", "SelectionComparison");
        profiler.addSeries("TotalOperationBubble", "BubbleAssignment", "BubbleComparison");


        profiler.createGroup("ASSIGNMENTS AVG", "InsertionAssignment", "SelectionAssignment", "BubbleAssignment");
        profiler.createGroup("COMPARISON AVG", "InsertionComparison", "SelectionComparison", "BubbleComparison");
        profiler.createGroup("TOTAL AVG", "TotalOperationInsertion", "TotalOperationSelection", "TotalOperationBubble");

        n = 1;
    }
    else 
        if (whichCase == BEST)
        {
            for (int i = 1;i <= 5;i++)
            {
                int aux = 100 * i;
                FillRandomArray(a, aux, 10, 50000, false, ASCENDING); //best case

                Operation InsertionAssignmentB = profiler.createOperation("InsertionAssignmentB", aux);
                Operation InsertionComparisonB = profiler.createOperation("InsertionComparisonB", aux);
                CopyArray(b, a, aux);
                insertionSort(b, aux, &InsertionAssignmentB, &InsertionComparisonB);


                Operation SelectionAssignmentB = profiler.createOperation("SelectionAssignmentB", aux);
                Operation SelectionComparisonB = profiler.createOperation("SelectionComparisonB", aux);
                CopyArray(c, a, aux);
                selectionSort(c, aux, &SelectionAssignmentB, &SelectionComparisonB);


                Operation BubbleAssignmentB = profiler.createOperation("BubbleAssignmentB", aux);
                Operation BubbleComparisonB = profiler.createOperation("BubbleComparisonB", aux);
                CopyArray(d, a, aux);
                bubbleSort(d, aux, &BubbleAssignmentB, &BubbleComparisonB);

            }

            profiler.addSeries("TotalOperationInsertionB", "InsertionAssignmentB", "InsertionComparisonB");
            profiler.addSeries("TotalOperationSelectionB", "SelectionAssignmentB", "SelectionComparisonB");
            profiler.addSeries("TotalOperationBubbleB", "BubbleAssignmentB", "BubbleComparisonB");

            profiler.createGroup("ASSIGNMENTS BEST", "InsertionAssignmentB", "SelectionAssignmentB", "BubbleAssignmentB");
            profiler.createGroup("COMPARISON BEST", "InsertionComparisonB", "SelectionComparisonB", "BubbleComparisonB");
            profiler.createGroup("TOTAL BEST", "TotalOperationInsertionB", "TotalOperationSelectionB", "TotalOperationBubbleB");

        }
        else
            if(whichCase == WORST)
        {
                for (int i = 1;i <= 5;i++)
            {
                int aux = 100 * i;
                FillRandomArray(a, aux, 10, 50000, false, DESCENDING); //worst

                Operation InsertionAssignmentW = profiler.createOperation("InsertionAssignmentW", aux);
                Operation InsertionComparisonW = profiler.createOperation("InsertionComparisonW", aux);
                CopyArray(b, a, aux);
                insertionSort(b, aux, &InsertionAssignmentW, &InsertionComparisonW);


                Operation SelectionAssignmentW = profiler.createOperation("SelectionAssignmentW", aux);
                Operation SelectionComparisonW = profiler.createOperation("SelectionComparisonW", aux);
                CopyArray(c, a, aux);

                int aux_valoare = c[aux - 1];
                for (int j = aux-2;j >= aux/2;j--)
                {
                    c[j + 1] = c[j];
                }
                c[aux / 2] = aux_valoare;

                selectionSort(c, aux, &SelectionAssignmentW, &SelectionComparisonW);


                Operation BubbleAssignmentW = profiler.createOperation("BubbleAssignmentW", aux);
                Operation BubbleComparisonW = profiler.createOperation("BubbleComparisonW", aux);
                CopyArray(d, a, aux);
                bubbleSort(d, aux, &BubbleAssignmentW, &BubbleComparisonW);

            }

            profiler.addSeries("TotalOperationInsertionW", "InsertionAssignmentW", "InsertionComparisonW");
            profiler.addSeries("TotalOperationSelectionW", "SelectionAssignmentW", "SelectionComparisonW");
            profiler.addSeries("TotalOperationBubbleW", "BubbleAssignmentW", "BubbleComparisonW");

            profiler.createGroup("ASSIGNMENTS WORST", "InsertionAssignmentW", "SelectionAssignmentW", "BubbleAssignmentW");
            profiler.createGroup("COMPARISON WORST", "InsertionComparisonW", "SelectionComparisonW", "BubbleComparisonW");
            profiler.createGroup("TOTAL WORST", "TotalOperationInsertionW", "TotalOperationSelectionW", "TotalOperationBubbleW");
            
        }
}

void benchmark(Profiler& profiler, AnalysisCase whichCase)
{
    // TODO: Write your benchmarks here, as explained in the readme
    // You should use profiler.startTimer/stopTimer for measuring and plotting measured time
}

} // namespace lab01
