#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int *arr, int i, int j)
{
	int temp;

	temp = arr[j];
	arr[j] = arr[i];
	arr[i] = temp;

#if 0
	arr[i] = arr[i] ^ arr[j];
	arr[j] = arr[i] ^ arr[j];
	arr[i] = arr[i] ^ arr[j];
#endif
}

/* O(N^2) */
void bubble_sort(int *arr, int arr_size)
{
	int e, i;

	if (arr == NULL || arr_size < 2) {
		return;
	}

	for (e = arr_size - 1; e > 0; e--) {
		for (i = 0; i < e; i++) {
			if (arr[i] > arr[i + 1]) {
				swap(arr, i, i + 1);
			}
		}
	}
}

/* O(N^2) depends on data */
void insertion_sort(int arr[], int arr_size)
{
	int i, j;

	if (arr == NULL || arr_size < 2) {
		return;
	}

	for (i = 1; i < arr_size; i++) {
		for (j = i - 1; j >= 0 && arr[j] > arr[j + 1]; j--) {
			swap(arr, j, j + 1);
		}
	}
}

/* O(N^2) */
void selection_sort(int arr[], int arr_size)
{
	if (arr == NULL || arr_size < 2) {
		return;
	}

	for (int i = 0; i < arr_size - 1; i++) {
		int min_idx = i;
		for (int j = i + 1; j < arr_size; j++) {
			min_idx = arr[j] < arr[min_idx] ? j : min_idx;
		}
		swap(arr, i, min_idx);
	}
}


static void do_merge(int *arr, int l, int m, int r)
{
	if (r < l)
		return;

	/* C99 support */
	int help[r - l + 1];
	int i = 0;
	int p1 = l;
	int p2 = m + 1;

	while (p1 <= m && p2 <= r) {
		help[i++] = arr[p1] < arr[p2] ? arr[p1++] : arr[p2++];
	}
	while (p1 <= m) {
		help[i++] = arr[p1++];
	}
	while (p2 <= r) {
		help[i++] = arr[p2++];
	}
	/* copy back */
	for (i = 0; i < sizeof(help)/sizeof(int); i++) {
		arr[l + i] = help[i];
	}
}

static void __merge_sort(int *arr, int l, int r)
{
	if (l == r) {
		return;
	}

	int mid = l + ((r - l) >> 1);

	/* left zone */
	__merge_sort(arr, l, mid);
	/* right zone */
	__merge_sort(arr, mid + 1, r);

	do_merge(arr, l, mid, r);
}

/* O(N*logN) */
void merge_sort(int *arr, int arr_size)
{
	if (arr == NULL || arr_size < 2) {
		return;
	}
	__merge_sort(arr, 0, arr_size - 1);
}


static void print_array(int *arr, int l, int r)
{
	if (l > r)
		return;

	for (int i= l; i<=r; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	return;	
}

/* Parition is used to divide array into three zones.
 *    || <key || =key || >key ||
 *
 * Array liks,  [2, 3, 6, 1, 5, 5, 7, 5]
 *
 * l = 0
 * r = 7
 * key = arr[7] = 5
 *
 * so, the boundary of,
 * < zone is (l-1)
 * > zone is (r-1)
 */
static void partition(int *arr, int l, int r, int *nl, int *nr)
{
	/* arr[r] is the one selected randomly as Key */
	int key = arr[r];
	int less = l - 1;	/* right boundary of < zone */
	int more = r;	/* left boundary of > zone */

	//printf("key = %d\n", key);
	/* from left to right */
	while (l < more) {
		if (arr[l] < key) {
			/* < zone increase, check next one */
			swap(arr, ++less, l++);
		} else if (arr[l] > key) {
			/* > zone increase, check the one after swap */
			swap(arr, --more, l);
		} else {
			l++;
		}
	}
	/* swap key number with last element in > zone */
	swap(arr, more, r);

	*nl = less + 1;
	*nr = more;

	return;
}

void __quick_sort(int *arr, int l, int r)
{
	if (l < r) {
		/* 
		 * The normal quick sort is O(N^2). we select
		 * key randomly, the possibility would be 1/N.
		 * then become O(N*logN)
		 */
		int rnd = l + (int) (rand() % (r - l + 1));
		//printf("Sort from [%d] to [%d], key=[%d]\n", l, r, arr[rnd]);
		swap(arr, rnd, r);

		int p[2] = {l+1, r};

		/* p[0] ~ p[1] is = zone */
		partition(arr, l, r, &p[0], &p[1]);

		/* left sort. p[0]-1 is right boundary of < zone */
		__quick_sort(arr, l, p[0] - 1);

		/* right sort. p[1]+1 is left boundary of > zone */
		__quick_sort(arr, p[1] + 1, r);
	}
}

/* O(N*logN) */
void quick_sort(int *arr, int arr_size)
{
	if (arr == NULL || arr_size < 2) {
		return;
	}
	__quick_sort(arr, 0, arr_size - 1);
}


/* 
 * left child = i * 2 + 1
 * right child = i * 2 + 2
 * parent node = (i -1) / 2 
 */

/* 
 * Upward adjustment to create a MAX-heap. 
 * 
 * 1. Insert a node to the last position of tree,
 * 2. Adjust(compare&swap) the node upwards until the heap is MAX-heap. (parent node always bigger than children)
 */
static void __heap_insert(int *arr, int index)
{
	/* compare with parent node */
	while (arr[index] > arr[(index - 1) / 2]) {
		swap(arr, index, (index - 1) / 2);
		index = (index - 1) / 2;
	}
}


/* 
 * Downward adjustment.
 *
 * 1. Remove the root node of MAX-heap which is the biggest number
 * 2. Swap the root node with the last child node, then disconnect the root node from heap. (Heap size--)
 * 3. Adjust(compare&swap) the new root node downwards until the heap is MAX-heap. (parent node always bigger than children)
 */
static void __heapify(int *arr, int index, int size) 
{
	int left = index * 2 + 1;
	while (left < size) {
		/* compare children */
		int largest = left + 1 < size && arr[left + 1] > arr[left] ? left + 1 : left;

		/* compare with parent */
		largest = arr[largest] > arr[index] ? largest : index;
		if (largest == index) {
			break;
		}
		swap(arr, largest, index);
		index = largest;
		left = index * 2 + 1;
	}
}

/* O(N*logN) */
void heap_sort(int *arr, int arr_size)
{
	if (arr == NULL || arr_size < 2) {
		return;
	}

	for (int i = 0; i < arr_size; i++) {
		__heap_insert(arr, i);
	}

	int size = arr_size;
	swap(arr, 0, --size);
	while (size > 0) {

		__heapify(arr, 0, size);
		swap(arr, 0, --size);
	}
}


int main(int argc, char **argv)
{
	int a[] = {1, 5, 3, 7, 9, 5, 8, 2, 3, 6, 10, 100, 231, 12};
	int i = 0;
	char *method = "quick";

	printf("Before: ");
	for (i = 0; i < sizeof(a)/sizeof(int); i++)
		printf(" %d", a[i]);
	printf("\n");

	if (argc < 2)
		method = "quick";
	else
		method = argv[1];

	if (strcmp(method, "heap") == 0) {
		printf("Heap sort: \n");
		heap_sort(a, sizeof(a)/sizeof(int));
	} else if (strcmp(method, "quick") == 0) {
		printf("Quick sort: \n");
		quick_sort(a, sizeof(a)/sizeof(int));
	} else if (strcmp(method, "select") == 0) {
		printf("Selection sort: \n");
		selection_sort(a, sizeof(a)/sizeof(int));
	} else if (strcmp(method, "insert") == 0) {
		printf("Insert sort: \n");
		insertion_sort(a, sizeof(a)/sizeof(int));
	} else if (strcmp(method, "bubble") == 0) {
		printf("Bubble sort: \n");
		bubble_sort(a, sizeof(a)/sizeof(int));
	} else if (strcmp(method, "merge") == 0) {
		printf("Merge sort: \n");
		merge_sort(a, sizeof(a)/sizeof(int));
	}

	printf("After: ");
	for (i = 0; i < sizeof(a)/sizeof(int); i++)
		printf(" %d", a[i]);
	printf("\n");

	return 0;
}
