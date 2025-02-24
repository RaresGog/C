#include "merge_lists.h"
#include "catch2.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>



/***
							LAB 4
		GOG RARES FLAVIU			GROUP 4

		*Merging k lits has complexity O (n log k), where k is the nr of lists
		* using a min heap we select the smallest element from one of the lists
		* that element gets added to the merged list
		* always the first element will be the smallest from each list
		* if we have more list we will have more operations

*/

namespace lab04
{
	NodeT* create_node(int value)
	{
		NodeT* newNode = (NodeT*)malloc(sizeof(NodeT));
		if (newNode)
		{
			newNode->value = value;
			newNode->next = NULL;
		}
		return newNode;
	}

	ListT* create_list(int* values, int n)
	{
		ListT* list = (ListT*)malloc(sizeof(ListT));
		if (!list) return NULL;
		list->first = list->last = NULL;
		for (int i = 0; i < n; i++)
		{
			NodeT* newnode = create_node(values[i]);
			insert_last(list, newnode);
		}
		return list;
	}

	NodeT* remove_first(ListT* list)
	{
		if (!list || !list->first) return NULL;
		NodeT* node = list->first;
		list->first = list->first->next;
		if (!list->first)
		{
			list->last = NULL;
		}
		node->next = NULL;
		return node;
	}

	void insert_last(ListT* list, NodeT* node)
	{
		if (!list || !node)
			return;
		node->next = NULL;
		if (!list->first)
		{
			list->first = list->last = node;
		}
		else
		{
			list->last->next = node;
			list->last = node;
		}
	}

	void print_list(ListT* list)
	{
		if (!list || !list->first)
		{
			std::cout << "There is no List or it is set to NULL";
			std::cout << std::endl;
			return;
		}
		NodeT* actual = list->first;
		while (actual)
		{
			std::cout << actual->value << " ";
			actual = actual->next;
		}
		std::cout << std::endl;
	}

	bool is_empty(ListT* list)
	{
		return !list || !list->first;
	}

	void destroy_list(ListT** list)
	{
		if (is_empty(list[0])) return;

		NodeT* actual = (*list)->first;
		while (actual)
		{
			NodeT* temporar = actual;
			actual = actual->next;
			free(temporar);
		}
		free(*list);
		*list = NULL;
	}

	void NodeAdder(ListT* list, int value) 
	{
		NodeT* N = create_node(value);

		if (N == NULL)	return;
		if (list->first == NULL) 
		{
			list->first = N;
			list->last = N;
		}
		else 
		{
			list->last->next = N;
			list->last = N;
		}
	}

	ListT** generate_k_sorted_lists(int n, int k)
	{
		ListT** list = (ListT**)malloc(k * sizeof(ListT));
		int a[10001];
		if (list == NULL) return NULL;

		int div = n / k;
		int aux = n;
		for (int i = 0; i < k; i++) 
		{
			list[i] = create_list();
			if (i == k - 1) 
			{
				FillRandomArray(a, aux, 10, 10000, false, ASCENDING);
				for (int j = 0; j < aux;j++)
				{
					NodeAdder(list[i], a[j]);
				}

			}
			else {
				FillRandomArray(a, div, 10, 10000, false, ASCENDING);
				for (int j = 0; j < div; j++)
				{
					NodeAdder(list[i], a[j]);
					aux--;
				}

			}
		}
		return list;
	}

	ListT* merge_2_lists(ListT* list_A, ListT* list_B)
	{
		ListT* mergeList = create_list();

		NodeT* a = list_A->first;
		NodeT* b = list_B->first;
		while (a && b)
		{
			NodeT* node;
			if (a->value <= b->value)
			{
				node = remove_first(list_A);
				a = list_A->first;
			}
			else
			{
				node = remove_first(list_B);
				b = list_B->first;
			}
			insert_last(mergeList, node);
		}
		while (a)
		{
			NodeT* node = remove_first(list_A);
			insert_last(mergeList, node);
			a = list_A->first;
		}
		while (b)
		{
			NodeT* node = remove_first(list_B);
			insert_last(mergeList, node);
			b = list_B->first;
		}

		return mergeList;
	}

	void min_heapify(ListT* lists[], int size, int i, Operation* op)
	{
		// TODO: Implement min-heapify logic
		int aux = i;
		int left = 2 * i + 1;
		int right = 2 * i + 2;

		if (op and left < size) op->count();
		if (left < size and lists[left]->first->value < lists[aux]->first->value)
		{
			aux = left;
		}

		if (op and right < size) op->count();
		if (right < size and lists[right]->first->value < lists[aux]->first->value)
		{
			aux = right;
		}
		if (aux != i)
		{
			if (op)op->count(3);
			std::swap(lists[i], lists[aux]);

			min_heapify(lists, size, aux, op);
		}


	}

	void build_heap_buttomup(ListT* lists[], int size, Operation* op)
	{
		// TODO: Implement heap building logic
		for (int i = size / 2 - 1;i >= 0;i--)
		{
			min_heapify(lists, size, i, op);
		}
	}

	ListT* merge_k_lists(ListT* lists[], int size, Operation* op)
	{
		ListT* Mlist = create_list();
		build_heap_buttomup(lists, size, op);

		while (size)
		{
			NodeT* N = remove_first(lists[0]);
			if (op)op->count();

			if (is_empty(lists[0]))
			{
				free(lists[0]);
				size--;
				if (op)op->count();
				lists[0] = lists[size];
			}

			if (op)op->count();
			insert_last(Mlist, N);
			min_heapify(lists, size, 0, op);
		}
		return Mlist;
	}

	void printare(ListT** lists, int k)
	{
		for (int i = 0; i < k; i++)
		{
			std::cout << "LIST " << i + 1 << std::endl;
			print_list(lists[i]);
		}
	}

	void demonstrate(int n, int k)
	{
		ListT** lists = generate_k_sorted_lists(20, 5);
		ListT** twolists = generate_k_sorted_lists(20, 2);

		std::cout << "2 List to merge: ";
		std::cout << std::endl;
		printare(twolists, 2);
		std::cout << std::endl;

		std::cout << "K (5) lists to merge: ";
		std::cout << std::endl;
		printare(lists, 5);
		std::cout << std::endl;


		ListT* merged = merge_k_lists(lists, 5);
		std::cout << "Merged Lists: ";
		print_list(merged);
		std::cout << std::endl;


		ListT* merged_list = merge_2_lists(twolists[0], twolists[1]);
		std::cout << "MergeSort la primele doua siruri: ";
		print_list(merged_list);
		std::cout << std::endl;

		free(lists);

	}

	bool IsListSorted(ListT* list)
	{
		NodeT* actual = list->first;
		while (actual->next != NULL)
		{
			if (actual->value > actual->next->value)
				return false;
			actual = actual->next;
		}
		return true;
	}
	TEST_CASE("Merge lists")
	{
		for (int i = 1;i <= 100;i++)
		{
			ListT** lists = generate_k_sorted_lists(20, 5);
			ListT* merged = merge_k_lists(lists, 5);
			REQUIRE(IsListSorted(merged));
		}

	}

	void performance(Profiler& profiler, ListsCase whichCase)
	{
		if (whichCase == AVERAGE)
		{
			int K5 = 5, K10 = 10, K100 = 100;
			for (int j = 0;j < 5;j++)
			{
				for (int i = 100;i <= 10000;i += 100)
				{
					Operation K5_OP = profiler.createOperation("K5_OP", i);
					ListT** lists = generate_k_sorted_lists(i, K5);
					ListT* merged = merge_k_lists(lists, K5, &K5_OP);

				}
				for (int i = 100;i <= 10000;i += 100)
				{
					Operation K10_OP = profiler.createOperation("K10_OP", i);
					ListT** lists = generate_k_sorted_lists(i, K10);
					ListT* merged = merge_k_lists(lists, K10, &K10_OP);

				}
				for (int i = 100;i <= 10000;i += 100)
				{
					Operation K100_OP = profiler.createOperation("K100_OP", i);
					ListT** lists = generate_k_sorted_lists(i, K100);
					ListT* merged = merge_k_lists(lists, K100, &K100_OP);

				}
			}

			profiler.divideValues("K5_OP", 5);
			profiler.divideValues("K10_OP", 5);
			profiler.divideValues("K100_OP", 5);
			profiler.createGroup("Total OP", "K5_OP", "K10_OP", "K100_OP");

			int n = 10000;
			for (int j = 0;j < 5;j++)
			{
				for (int k = 10;k <= 500;k += 10)
				{
					Operation Ops = profiler.createOperation("Ops", k);
					ListT** lists = generate_k_sorted_lists(n, k);
					ListT* merged = merge_k_lists(lists, k, &Ops);
				}
			}
			profiler.divideValues("Ops", 5);
			profiler.createGroup("Ops_k_variabil", "Ops");

		}
	}
} // namespace lab04
