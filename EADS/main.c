#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node;

typedef struct node {
	struct node* next; 
	int dst;
	int weight;
	int visited;
	int unw_path_len;
} node_t;

node_t** adj_lists;
unsigned size_of_adj_lists;

node_t* create_node(int dst, int weight)
{
	node_t* new_node = malloc(sizeof(node_t));
	// memset();
	new_node->dst = dst;
	new_node->weight = weight;
	new_node->next = NULL;

	return new_node;
}

int init_adj_lists(unsigned num_of_vertices)
{
	adj_lists = malloc(num_of_vertices * sizeof(node_t*));
	size_of_adj_lists = num_of_vertices;

	unsigned i;
	for (i = 0; i < num_of_vertices; i++) {
		adj_lists[i] = NULL;
	}

	return 0;
}

void add_item_to_adj_lists(unsigned src, unsigned dst)
{
//	printf("src: %d, dst: %d\n", src, dst);

	node_t* new_node;
	node_t** list = &adj_lists[src];

//	printf("list: %p\n", list);
//	printf("brk 1\n");

	/* TODO: Need to check if this edge already saved */
	if (!(*list)) {
		*list = create_node(dst, -1);
		//printf();
		return;
	}
//	printf("brk 2\n");
	while ((*list)->next) {
		(*list) = (*list)->next;
	}
//	printf("brk 3\n");		
	(*list)->next = create_node(dst, -1);
}	

void print_adj_lists()
{
	node_t* list;

	int i;

	for (i = 0; i < size_of_adj_lists; i++) {
		list = adj_lists[i];

		while (list) {
			printf("%d, %d \n", i, list->dst);
			list = list->next;
		}
	}
}


void bfs(node_t** adj_list, int src)
{
	node_t* list;
	int i;

	for (i = 0; i < size_of_adj_lists; i++) {
		list = adj_list[i];

		while (list) {
			list->visited = -1;
			list->unw_path_len = -1;
			list = list->next;
		}
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

//	printf("### print created lists ###\n");
//	print_adj_lists();

	return 0;
}
