#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

#include <time.h>

using namespace std;

class Node {
	private:
		int index;
		int first_parent;
		int second_parent;

	public:
		int width;
		int height;
		int length;

		Node(int w = 0, int h = 0, int l = 0, int i = 0);
		~Node();		

		int get_width();
		int get_height();
		int get_length();
		int get_big_parent();
		int get_small_parent(); 
		int get_node_index();
		void set_parents(int first, int second);
		int is_glued();
};

Node::Node(int w, int h, int l, int i)
{
	width = w;
	height = h;
	length = l;
	index = i;

	first_parent = 0;
	second_parent = 0;
}

Node::~Node()	{}

int Node::get_width() { return width; }
int Node::get_height() { return height; }
int Node::get_length() { return length; }

int Node::is_glued()
{
	if ((first_parent != 0) || (second_parent != 0))
		return 1;
	
	return 0;
}

void Node::set_parents(int first, int second)
{
	first_parent = first;
	second_parent = second;
}

int Node::get_big_parent()	
{	
	if (second_parent < first_parent)
		return first_parent;

	return second_parent;
}

int Node::get_small_parent()	
{	
	if (first_parent < second_parent)
		return first_parent;

	return second_parent;
}

int Node::get_node_index()	{	return index;	}

Node* current_max_node;
std::vector<Node*> new_glued_nodes;

int get_min_side_of_box(Node* node)
{
	int w = node->width;
	int h = node->height;
	int l = node->length;

	if (w < h) {
		if (l < w)
			return l;
		else
			return w;
	} else {
		if (l < h)
			return l;
		else
			return h;
	}
}

int check_max_node(Node** cur_node, Node** new_node)
{
	Node* c_node = *cur_node;
	Node* n_node = *new_node;

	int cur_min = get_min_side_of_box(c_node);
	int new_min = get_min_side_of_box(n_node);

	if (cur_min < new_min) {
 		*cur_node = *new_node;
		return 1;
	}

	return 0;
}

#define HTABLE_SIZE		997

class HashTable {
	private:
		int num_of_buckets;
		vector<Node*> *hash_table;

	public:
		HashTable(int sz);
		~HashTable();

		int hash_func(int key);
		void add_node(Node* node, int side_0, int side_1);
		int get_num_of_nodes_in_chain(int key_0, int key_1);

		Node* get_node_from_chain_by_index(int key_0, int key_1, int index);
		vector<Node*> get_chain_vector(int key_0, int key_1);


};

HashTable::HashTable(int sz)
{
	num_of_buckets = sz;
	hash_table = new vector<Node*>[num_of_buckets];
}

HashTable::~HashTable()	
{
	for (int i = 0;  i < num_of_buckets; i++) {
		for (std::vector<Node*>::iterator it = hash_table[i].begin(); it != hash_table[i].end(); ++ it) {
			delete (*it);
		}
	}
}

int HashTable::hash_func(int key) { return key % HTABLE_SIZE; }

void HashTable::add_node(Node* node, int side_0, int side_1) 
{
	int idx = hash_func(side_0 + side_1 + side_0 * side_1);
	hash_table[idx].push_back(node);
}

int HashTable::get_num_of_nodes_in_chain(int key_0, int key_1)
{
	int idx = hash_func(key_0 + key_1 + key_0 * key_1);
	return hash_table[idx].size();
}

Node* HashTable::get_node_from_chain_by_index(int key_0, int key_1, int index)
{
	vector <Node*> :: iterator i;
	int idx = hash_func(key_0 + key_1 + key_0 * key_1);
	return (hash_table[idx])[index];
}

vector<Node*> HashTable::get_chain_vector(int key_0, int key_1)
{
	int idx = hash_func(key_0 + key_1 + key_0 * key_1);
	return (hash_table[idx]);
}

void create_dual_node_and_check_max(int w2, int h2, int l2, int node0_idx, int node1_idx)
{
	Node* dual_node = new Node(w2, h2, l2, 0);
	dual_node->set_parents(node0_idx, node1_idx);

	if (check_max_node(&current_max_node, &dual_node))
		new_glued_nodes.push_back(dual_node);
	else
		delete dual_node;
}

int compare_max(int w, int h, int l)
{
	int min;

	if (w < h) {
		if (l < w)
			min = l;
		else
			min = w;
	} else {
		if (l < h)
			min = l;
		else
			min = h;
	}

	int cur_min = get_min_side_of_box(current_max_node);

	if (cur_min < min)
		return 1;
	else
		return 0;
}

void glue_two_nodes(Node* node0, Node* node1)
{
	int w0 = node0->width;
	int h0 = node0->height;
	int l0 = node0->length;
	int w1 = node1->width;
	int h1 = node1->height;
	int l1 = node1->length;
	int w2 = 0, h2 = 0, l2 = 0;
	int node0_idx = node0->get_node_index();
	int node1_idx = node1->get_node_index();

	if ((w0 == w1) || (w0 == l1) || (w0 == h1)) {
		
		if (w0 == w1) {
			w2 = w0;
			if (l0 == l1) {
				l2 = l0;
				h2 = h0 + h1;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
			if (h0 == h1) {
				h2 = h0;
				l2 = (l0 + l1);
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
			if (h0 == l1) {
				l2 = h0;
				h2 = h1 + l0;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
			if (h1 == l0) {
				l2 = l0;
				h2  = h0 + l1;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
		}

		if (w0 == l1) {
			w2 = w0;
			if (l0 == w1) {
				l2 = l0;
				h2 = h0 + h1;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
			if (l0 == h1) {
				l2 = l0;
				h2 = h0 + w1;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
			if (h0 == w1) {
				l2 = h0;
				h2 =  h1 + l0;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
			if (h0 == h1) {
				l2 = h0;
				h2 =  w1 + l0;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
		}

		if (w0 == h1) {
			w2 = w0;
			if (l0 == w1) {
				l2 = l0;
				h2 = h0 + l1;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
			if (l0 == l1) {
				l2 = l0;
				h2 =  h0 + w1;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
			if (h0 == w1) {
				l2 = h0;
				h2 = l0 + l1;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
			if (h0 == l1) {
				l2 = h0;
				h2 = l0 + w1;
				if (compare_max(w2, h2, l2)) {
					create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
				}
			}
		}
	} else {
		if (((l0 == l1) && (h0 == h1)) || ((l0 == h1) && (h0 == l1))) {
			w2 = w0 + w1;
			l2 = l0;
			h2 = h0;
			create_dual_node_and_check_max(w2, h2, l2, node0_idx, node1_idx);
			return;
		} else {
			return;
		}
	}
}

void check_two_sides(HashTable* hash_table, Node* new_node, int side_0, int side_1)
{
	std::vector<Node*> chain_vec = hash_table->get_chain_vector(side_0, side_1);
	for (std::vector<Node*>::iterator it_0 = chain_vec.begin(); it_0 != chain_vec.end(); ++ it_0) {
		glue_two_nodes(*it_0, new_node);
	}
}

void check_dual_nodes(HashTable* hash_table, Node* new_node)
{
	int w = new_node->width;
	int h = new_node->height;
	int l = new_node->length;

	if ((w == h) && (h == l)) {
		check_two_sides(hash_table, new_node, w, h);
		return;
	}

	if ((w != h) && (h != l) && (w != l)) {
		check_two_sides(hash_table, new_node, w, h);
		check_two_sides(hash_table, new_node, w, l);
		check_two_sides(hash_table, new_node, l, h);
		return;
	}

	if ((w == h) && (h != l)) {
		check_two_sides(hash_table, new_node, w, h);
		check_two_sides(hash_table, new_node, w, l);
		return;
	}

	if ((w != h) && (h == l)) {
		check_two_sides(hash_table, new_node, w, h);
		check_two_sides(hash_table, new_node, l, h);
		return;
	}

	if ((w == l) && (l != h)) {
		check_two_sides(hash_table, new_node, w, l);
		check_two_sides(hash_table, new_node, l, h);
		return;
	}
}

void hash_box(HashTable** hash_table, Node* node)
{
	int w = node->width;
	int h = node->height;
	int l = node->length;

	HashTable* htable = *hash_table;

	if ((w == h) && (h == l)) {
		htable->add_node(node, w, h);
		return;
	}

	if ((w != h) && (h != l) && (w != l)) {
		htable->add_node(node, w, h);
		htable->add_node(node, w, l);
		htable->add_node(node, l, h);
		return;
	}

	if ((w == h) && (h != l)) {
		htable->add_node(node, w, h);
		htable->add_node(node, w, l);
		return;
	}

	if ((w != h) && (h == l)) {
		htable->add_node(node, h, w);
		htable->add_node(node, h, l);
		return;
	}

	if ((w == l) && (l != h)) {
		htable->add_node(node, w, l);
		htable->add_node(node, h, l);		
		return;
	}

	return;
}

int main()
{
	HashTable* hash_table = new HashTable(HTABLE_SIZE);
	current_max_node = new Node(0, 0, 0, 0);
	
	std::ifstream infile("input.txt");

	int num_of_boxes = 0;
	infile >> num_of_boxes;

	int width  = 0, height = 0, length = 0;
	Node* new_node;

	for (int i = 1; i <= num_of_boxes; i++) {
		infile >> width >> height >> length;
		new_node = new Node(width, height, length, i);

		check_max_node(&current_max_node, &new_node);
		check_dual_nodes(hash_table, new_node);
		hash_box(&hash_table, new_node);
	}

	int num_of_glued_boxes = current_max_node->is_glued() + 1;

	ofstream outfile;
	outfile.open ("output.txt");

	outfile << num_of_glued_boxes << endl;
	
	if (current_max_node->is_glued()) {
		outfile << current_max_node->get_small_parent() << " " << current_max_node->get_big_parent() << endl;
	} else {
		outfile << current_max_node->get_node_index() << endl;
	}

	outfile << get_min_side_of_box(current_max_node);

	outfile.close();
	infile.close();

	return 0;
}
