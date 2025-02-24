#include "hash.h"
#include "catch2.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#define MAX_SIZE 10007

/*
							LAB 5
		GOG RARES FLAVIU			GROUP 4      

		* hash tables are usefull becouse it has complexity of O(1) for insert and delete,if there are colisions the
		* complexity increases ,but the avg is still O(1)
		* if the table is filled more and more the operations needed for that will also increase

*/

namespace lab04  //lab 5
{
	int hash(int key,int size)
	{
		return key % size;
	}

	void initializare(hashTable *hashTable,int size)
	{
		for (int i = 0;i < size;i++)
		{
			hashTable->table[i] = NULL;
		}
	}

	int quadProbe(int key, int i,int size)
	{
		int x = 1, y = 2;
		return (hash(key,size) + x * i + y * i * i) % size;
	}

	int quadInsert(hashTable* hashTable,int id,const char* nume,int size)
	{
		int i = 0;
		while (i < size)
		{
			int j = quadProbe(id, i,size);

			if (hashTable->table[j] == NULL or hashTable->table[j]->id == -1)
			{
				if (hashTable->table[j] == NULL)
					hashTable->table[j] = (entry*)malloc(sizeof(entry));

				hashTable->table[j]->id = id;
				strncpy(hashTable->table[j]->nume, nume, 30);
				return j;
			}
			i++;
		}
		std::cout << "Table full \n";
		return -1;
	}

	int search(hashTable* hashTable, int id,int* ops,int size)
	{
		int i = 0;
		*ops = 0;

		while (i < size)
		{
			(*ops)++;
			int index = quadProbe(id, i,size);

			(*ops)++;
			if (hashTable->table[index] != NULL && hashTable->table[index]->id == id)
				return index; // Found

			if (hashTable->table[index] == NULL)
				break;

			i++;
		}
		return -1; // Not found
	}

	int del(hashTable* hashTable,int id,int size)
	{
		int i = 0;

		while (i < size)
		{
			int j = quadProbe(id, i,size);
			
			if (hashTable->table[j] != NULL and hashTable->table[j]->id==id) //check if not null and if not deleted before
			{
				hashTable->table[j]->id = -1; //changing id with -1 for deleted element
				return j;
			}

			if (hashTable->table[j] == NULL)
				break;

			i++;
		}
		return -1; //not found
	}

	void demonstrate(int n, int k)
	{

		hashTable hashTable;
		initializare(&hashTable,MAX_SIZE);

		int ids[11];
		FillRandomArray(ids, 10, 10, 100, false, UNSORTED);
		int y = 0;
		float fillingfactor = 0.99;
		int max =(int)(fillingfactor * 10);
		//std::cout << "A" << max << "\n";
		for (int id = 0; id <max; id++)
		{
			quadInsert(&hashTable, ids[id], "ADAUGA NUME", 10);
		}
		std::cout << "TABLE BEFORE ELIMINATING ELEMENTS (0.99 filling factor): \n";
		for (int i = 0;i < 10;i++)
		{
			if (hashTable.table[i] != NULL and hashTable.table[i]->id != -1)
			{
				std::cout << "INDEX " << i << ": " << hashTable.table[i]->id << "\n";
				y++;
			}
			else if (hashTable.table[i] != NULL and hashTable.table[i]->id == -1)
			{
				std::cout << "INDEX " << i << ": DELETED\n";
				y++;
			}
			else if (hashTable.table[i] == NULL)
				std::cout << "INDEX " << i << ": - \n";
		}
		while (y > 8)
		{
			int id = 0;
			int index = del(&hashTable, ids[id], 10);
			id++;
			if (index != -1) y--;

		}

		std::cout << "TABLE AFTER ELIMINATING ELEMENTS (0.8 filling factor) : \n";

			for (int i = 0;i < 10;i++)
			{
				if (hashTable.table[i] != NULL and hashTable.table[i]->id != -1)
				{
					std::cout << "INDEX " << i << ": " << hashTable.table[i]->id << "\n";
				}
				else if (hashTable.table[i] != NULL and hashTable.table[i]->id == -1)
				{
					std::cout << "INDEX " << i << ": DELETED\n";
				}
				else if (hashTable.table[i] == NULL)
					std::cout << "INDEX " << i << ": - \n";
			}

	}


	void performance(Profiler& profiler, ListsCase whichCase)
	{
		float fillingfactor[] = {0.8,0.85,0.9,0.95,0.99};
		hashTable hashTable;
		if (whichCase == AVERAGE)
		{
			std::cout << "fillingfactor\t" << "avgEffortFound\t" << "MaxOpsFound\t" << "avgEffortNotFound\t" << "MaxOpsNotFound\n";
			for (int t = 0; t < 5;t++)
			{
				int n = (int)(fillingfactor[t] * MAX_SIZE);
				int MaxOpsFound = 0;
				int MaxOpsNotFound = 0;
				int TotalOpsFound = 0;
				int TotalOpsNotFound = 0;
				initializare(&hashTable,MAX_SIZE);

			
				for (int i = 0;i < n;i++)
				{
					int id[10000];
					FillRandomArray(id, 10000, 10, 10000, false, UNSORTED);
					int pass = 0;
					char nume[30];
					quadInsert(&hashTable, id[pass++], nume, MAX_SIZE);
				}
				
				for (int i = 0;i < 3000;i++)
				{
					int id[10000];
					FillRandomArray(id, 10000, 10, 10000, false, UNSORTED);
					int pass =0;
					int ops = 0;
					int index = search(&hashTable, id[pass], &ops, MAX_SIZE);

					if (index != -1) 
					{	//found
						TotalOpsFound = TotalOpsFound + ops;
						if (MaxOpsFound < ops) MaxOpsFound = ops;
					}
					else
					{	//not found
						TotalOpsNotFound = TotalOpsNotFound + ops;
						if (MaxOpsNotFound < ops) MaxOpsNotFound = ops;

					}
				}

				float avgEffortFound = (float)TotalOpsFound / 1500;
				float avgEffortNotFound = (float)TotalOpsNotFound / 1500;

				std::cout << fillingfactor[t] << "\t\t" <<std::fixed<<std::setprecision(2)<< avgEffortFound << "\t\t" << MaxOpsFound << "\t\t" << avgEffortNotFound << "\t\t\t" << MaxOpsNotFound << "\n";
			}
		
		}
	}

} // namespace lab05
