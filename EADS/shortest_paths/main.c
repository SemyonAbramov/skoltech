#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* 
 * TODO:
 * 1) Early exit: +
 * 2) Deinit memory, check with valgrind: (need to check)
 * 3) Clean code
 * 4) Try to avoid using bfs
 * 5) Performance optimizations
 * 6) Remove printfs	
 */

/* FIXME: Need to change number here to deal with huge graphs */
#define INFINITY	(0x7FFFFFFF / 2)

struct successor_node;

typedef struct successor_node {
	struct successor_node* next; 
	int dst;
	int weight;
} successor_node_t;

typedef struct vertice {
	successor_node_t* next_adj_node;
	int d_val;
	int pred;
	int idx;
	int val;
	int unw_shortest;
} vertice_t;

vertice_t* adj_lists;
unsigned size_of_adj_lists;

/* adj list API */

void init_adj_lists(unsigned num_of_vertices)
{
	adj_lists = malloc(num_of_vertices * sizeof(vertice_t));
	size_of_adj_lists = num_of_vertices;

	unsigned i;
	for (i = 0; i < num_of_vertices; i++) {
		adj_lists[i].next_adj_node = NULL;
		adj_lists[i].val = i;
	}
}

void deinit_adj_lists()
{
	int i;
	successor_node_t* del_node, *list;

	for (i = 0; i < size_of_adj_lists; i++) {
		list = adj_lists[i].next_adj_node;

		while (list) {
			del_node = list;
			list = list->next;
			free(del_node);
		}
	}

	free(adj_lists);
}

successor_node_t* create_node(int dst, int weight)
{
	successor_node_t* new_node = malloc(sizeof(successor_node_t));
	// memset();
	new_node->dst = dst;
	new_node->weight = weight;
	new_node->next = NULL;
	return new_node;
}

/* Slow method */
void add_item_to_adj_lists(unsigned src, unsigned dst)
{
	successor_node_t* new_node;
	successor_node_t** list = &adj_lists[src].next_adj_node;

	/* TODO: Need to check if this edge already saved */
	if (!(*list)) {
		*list = create_node(dst, -1);
		return;
	}

	while ((*list)->next) {
		(*list) = (*list)->next;
	}
	
	(*list)->next = create_node(dst, -1);
}	

void print_adj_lists()
{
	successor_node_t* list;

	int i;
	for (i = 0; i < size_of_adj_lists; i++) {
		list = adj_lists[i].next_adj_node;

		while (list) {
			printf("%d -> %d, weight =  %d\n", i, list->dst, list->weight);
			list = list->next;
		}
	}
}

/* Queue impementation */

typedef struct queue
{ 
	int front;
	int rear; 
	int size;
	unsigned capacity;
	int* array;
}queue_t; 
  
queue_t* init_queue(unsigned capacity) 
{ 
	queue_t* queue = (queue_t*) malloc(sizeof(queue_t));
	queue->capacity = capacity;
	queue->front = queue->size = 0;
	queue->rear = capacity - 1;
	queue->array = (int*) malloc(queue->capacity * sizeof(int));
	return queue;
}

void deinit_queue(queue_t* queue)
{
	free(queue->array);
	free(queue);
}

int is_queue_full(queue_t* queue)	{	return (queue->size == queue->capacity);	} 
int is_queue_empty(queue_t* queue)	{	return (queue->size == 0);	}

void enqueue(queue_t* queue, int item) 
{ 
	if (is_queue_full(queue)) 
		return;
	queue->rear = (queue->rear + 1)%queue->capacity; 
	queue->array[queue->rear] = item; 
	queue->size = queue->size + 1; 
	printf("%d enqueued to queue\n", item); 
} 
 
int dequeue(queue_t* queue) 
{ 
	if (is_queue_empty(queue))
		return -1; 
	int item = queue->array[queue->front];
	queue->front = (queue->front + 1)%queue->capacity;
	queue->size = queue->size - 1;
	return item;
} 

int front(queue_t* queue)
{
	if (is_queue_empty(queue))
		return -1;
	return queue->array[queue->front];
}

int rear(queue_t* queue)
{
	if (is_queue_empty(queue))
		return -1;
	return queue->array[queue->rear];
}

/* Finish of queue implementation */

void bfs(vertice_t* adj_list, int src)
{
	successor_node_t* list;
	int visited[size_of_adj_lists];

	int i;
	for (i = 0; i < size_of_adj_lists; i++) {
		visited[i] = 0;
	}

	adj_list[src].unw_shortest = 0;

	queue_t* queue = init_queue(size_of_adj_lists);
	enqueue(queue, src);

	int dq = 0;

	while (is_queue_empty(queue) != 1) {
		dq = dequeue(queue);

		list = adj_list[dq].next_adj_node;

		while (list) {
			if (!visited[list->dst]) {
				visited[list->dst] = 1;
				adj_list[list->dst].unw_shortest = adj_list[dq].unw_shortest + 1;
				enqueue(queue, list->dst);
			}
			list = list->next;
		}
	}

	deinit_queue(queue);
}

void adjust_weights(vertice_t* adj_list)
{
	successor_node_t* list;
	int i;
	int w = 0;

	for (i = 0; i < size_of_adj_lists; i++) {
		list = adj_list[i].next_adj_node;
		w = adj_list[i].unw_shortest;

		printf("vertice: %d\n", i);

		while (list) {
			printf("dst node: %d    ", list->dst);

			printf("unw shortest path: %d\n", w);

			if (w % 2 == 0)
				list->weight = 1;
			else
				list->weight = 2;

			list = list->next;
		}
	}
}

void print_unw_shortest(vertice_t* adj_list, int s)
{
	int i;
	for (i = 0; i < size_of_adj_lists; i++) {
		printf("shortest path from %d to %d: %d\n", s, i, adj_list[i].unw_shortest);
	}
}

/* min heap impl */
typedef struct minheap {
	int capacity;
	int size;
	vertice_t** array;
} minheap_t;

minheap_t* init_minheap(int capacity)
{
	minheap_t* heap = malloc(sizeof(minheap_t));
	heap->size = 0;
	heap->capacity = capacity;
	heap->array = malloc(sizeof(vertice_t*) * capacity);
	
	int i;
	for (i = 0; i < capacity; i++)
		heap->array[i] = NULL;

	return heap;
}

void deinit_minheap(minheap_t* heap)
{
	free(heap->array);
	free(heap);
}

int get_parent_idx(int i) {	return (i - 1)/2;	}
int get_left_child(int i) {	return (2*i + 1);	}
int get_right_child(int i) {	return (2*i + 2);	}

void exchange(vertice_t** a, vertice_t** b)
{
	int tmp_idx = (*a)->idx;
	(*a)->idx = (*b)->idx;
	(*b)->idx = tmp_idx;

	vertice_t* tmp = *a;
	*a = *b;
	*b = tmp;
}

void insert_node(minheap_t* heap, vertice_t* node)
{
	if (heap->size == heap->capacity){
		printf("The binary heap is full\n");
		return;
	}

	heap->size += 1;
	int i = heap->size - 1;		//
	heap->array[i] = node;
	heap->array[i]->idx = i;

	while ((i != 0) && ((heap->array[get_parent_idx(i)]->d_val) > (heap->array[i]->d_val))) {
		exchange(&(heap->array[i]), &(heap->array[get_parent_idx(i)]));			
		i = get_parent_idx(i);
	}
}

void decrease_dval(minheap_t* heap, int i, int new_val)
{
	int idx = i;
  	heap->array[idx]->d_val = new_val; 
    while ((idx != 0) && ((heap->array[get_parent_idx(idx)]->d_val) > (heap->array[idx]->d_val)))
    { 
       	exchange(&(heap->array[idx]), &(heap->array[get_parent_idx(idx)])); 
       	idx = get_parent_idx(idx); 
    }
}

void min_heapify(minheap_t* heap, int i)
{
    int l = get_left_child(i);
    int r = get_right_child(i);
    int smallest = i;
    if ((l < heap->size) && ((heap->array[l]->d_val) < (heap->array[i]->d_val))) 
        smallest = l; 
    if ((r < heap->size) && ((heap->array[r]->d_val) < (heap->array[smallest]->d_val)))
        smallest = r;
    if (smallest != i)	{ 
        exchange(&(heap->array[i]), &(heap->array[smallest])); 
        min_heapify(heap, smallest); 
	}
}

vertice_t* extract_node_with_min_dval(minheap_t* heap)
{
	printf("extract node with min dval\n");

	if (heap->size <= 0) {
		printf("Heap size <= 0\n");
		return NULL;						// FIXME:
	}

	if (heap->size == 1) {
		heap->size -= 1;
		return heap->array[0];
	}

	vertice_t* minval_node = heap->array[0];
	heap->array[0] = heap->array[heap->size - 1];
	heap->array[0]->idx = 0;
	
	heap->size -= 1;

	min_heapify(heap, 0);			// FIXME: !!!
	
	return minval_node;
}

vertice_t* get_node_with_min_dval(minheap_t* heap)
{
	return heap->array[0];
}

void delete_key(minheap_t* heap, int key)
{
	decrease_dval(heap, key, INT_MIN);
	extract_node_with_min_dval(heap);
}

void delete_node(minheap_t* heap, vertice_t* node)
{
	delete_key(heap, node->idx);
}

void print_minheap(minheap_t* heap)
{
	int i;

	for (i = 0; i < heap->size; i++)
		printf("vertice num: %d, dval: %d, idx: %d \n", heap->array[i]->val, heap->array[i]->d_val, heap->array[i]->idx);
}

int is_minheap_empty(minheap_t* heap)
{	
	return heap->size == 0;
}

/************/

void relax(vertice_t* vertice, successor_node_t* node)
{
	int relaxed = vertice->d_val + node->weight;

	printf("relax vertice: %d, node: %d, vertice d_val = %d, node weight = %d, relaxed val = %d \n", vertice->val, node->dst, vertice->d_val, node->weight, relaxed);

	if (relaxed < adj_lists[node->dst].d_val) {
		adj_lists[node->dst].d_val = relaxed;
	}
}


void dijkstra(vertice_t* adj_list, int src)
{
	vertice_t* list = adj_list;

	int i;
	for (i = 0; i < size_of_adj_lists; i++) {
		list[i].d_val = INFINITY;
		list[i].pred = -1;
	}

//	list[src].weight = 1;	// FIXME: ??? may be 0 ???
	list[src].d_val = 0;

	vertice_t* processed_nodes = NULL;

	minheap_t* heap = init_minheap(size_of_adj_lists);
	
	for (i = 0; i < size_of_adj_lists; i++)
		insert_node(heap, &list[i]);	

	vertice_t* proc_node;
	successor_node_t* successor_node;

	while (is_minheap_empty(heap) != 1) {
		
		print_minheap(heap);
		
		proc_node = get_node_with_min_dval(heap);
		successor_node = proc_node->next_adj_node;

		while (successor_node) {
			relax(proc_node, successor_node);
			successor_node = successor_node->next;		
		}
		extract_node_with_min_dval(heap);	
	}
	deinit_minheap(heap);
}

int dijkstra_with_early_exit(vertice_t* adj_list, int src, int dst)
{
	vertice_t* list = adj_list;

	int i;
	for (i = 0; i < size_of_adj_lists; i++) {
		list[i].d_val = INFINITY;
		list[i].pred = -1;
	}

	list[src].d_val = 0;
	vertice_t* processed_nodes = NULL;
	
	// Here comes priority queue (hah)

	minheap_t* heap = init_minheap(size_of_adj_lists);
	for (i = 0; i < size_of_adj_lists; i++)
		insert_node(heap, &list[i]);

	vertice_t* proc_node;
	successor_node_t* successor_node;

	while (is_minheap_empty(heap) != 1) {
		proc_node = extract_node_with_min_dval(heap);
		
		if (proc_node->val == dst) {
			deinit_minheap(heap);
			return adj_list[dst].d_val;
		}

		successor_node = proc_node->next_adj_node;

		while (successor_node) {
			relax(proc_node, successor_node);
			successor_node = successor_node->next;		
		}
	}

	deinit_minheap(heap);
	return -1;
}

void print_dval(vertice_t* adj_list)
{
	int i;
	for (i = 0; i < size_of_adj_lists; i++) {
		printf("for i: %d dval = %d\n", i, adj_list[i].d_val);
	}
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("Please, provide input.txt file");
		return 1;
	}

	if (argc < 2) {
		printf("Please, provide only one input.txt file");
		return 1;
	}

	char* input_file = argv[1];
	
	FILE* ifp = fopen(input_file, "r");
	if (!ifp) {
		printf("Can not open file: %s", input_file);
		return 1;
	}

	unsigned num_of_edges = 0;
	unsigned num_of_vertices = 0;

	fscanf(ifp, "%u\n", &num_of_vertices);
	fscanf(ifp, "%u\n", &num_of_edges);

//	printf("vertices: %u\n", num_of_vertices);
//	printf("edges: %u\n", num_of_edges);

	unsigned source_node_index = 0;
	unsigned destination_node_index = 0;

	fscanf(ifp, "%u\n", &source_node_index);
	fscanf(ifp, "%u\n", &destination_node_index);

//	printf("source: %u\n", source_node_index);
//	printf("destination: %u\n", destination_node_index);

	int i;
	int src = 0, dst = 0;

	init_adj_lists(num_of_vertices);

//	printf("Read graph from the file\n");

	for (i = 0; i < num_of_edges; i++) {
		fscanf(ifp, "%d %d\n", &src, &dst);

//		printf("%d, %d \n", src, dst);
		
		add_item_to_adj_lists(src, dst);
	}

	printf("### print created lists ###\n");
	print_adj_lists();

/*
	bfs(adj_lists, source_node_index);
	adjust_weights(adj_lists);

	print_unw_shortest(adj_lists, source_node_index);
*/

	adj_lists[0].next_adj_node->weight = 3;
	adj_lists[0].next_adj_node->next->weight = 5;
	adj_lists[1].next_adj_node->weight = 5;
	adj_lists[1].next_adj_node->next->weight = 1;
	adj_lists[2].next_adj_node->weight = 1;
	adj_lists[3].next_adj_node->weight = 5;
	adj_lists[3].next_adj_node->next->weight = 5;
	adj_lists[4].next_adj_node->weight = 3;	

	print_adj_lists();

	int min_path = dijkstra_with_early_exit(adj_lists, source_node_index, destination_node_index);
	print_dval(adj_lists);

	printf("The shortest path from %d to %d is %d\n", source_node_index, destination_node_index, min_path);

	deinit_adj_lists();

	return 0;
}
