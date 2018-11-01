#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

using namespace std;

class Node {
	private:
		int width;
		int height;
		int length;
		int index;

		int first_parent;
		int second_parent;

		Node* next;


	public:
		Node(int w = 0, int h = 0, int l = 0, int i = 0);
		~Node();		

		int get_width();
		int get_height();
		int get_length();

		int is_glued();

		void set_parents(int first, int second);

		int get_big_parent();
		int get_small_parent(); 
		int get_node_index();
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
std::list<Node*> new_glued_nodes;

int get_mix_side_of_box(Node* node)
{
	int w = node->get_width();
	int h = node->get_height();
	int l = node->get_length();

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

//void check_max_node(Node** cur_node, Node** new_node)
int check_max_node(Node** cur_node, Node** new_node)
{
	Node* c_node = *cur_node;
	Node* n_node = *new_node;

	int cur_min = get_mix_side_of_box(c_node);
	int new_min = get_mix_side_of_box(n_node);

	if (cur_min < new_min) {
 		*cur_node = *new_node;
		return 1;
	}

	return 0;
}

#define HTABLE_SIZE		409

class HashTable {
	private:
		int num_of_buckets;
		list<Node*> *hash_table;

	public:
		HashTable(int sz);
		~HashTable();

		int hash_func(int key);
		void add_node(Node* node, int key);
		//void delete_node();
		int get_num_of_nodes_in_chain(int key);
		Node* get_node_from_chain_by_index(int key, int index);
};

HashTable::HashTable(int sz)
{
	num_of_buckets = sz;
	hash_table = new list<Node*>[num_of_buckets];
}

int HashTable::hash_func(int key)
{
	return key % HTABLE_SIZE;
}

void HashTable::add_node(Node* node, int key) 
{
	int idx = hash_func(key); 
    hash_table[idx].push_back(node);
}


int HashTable::get_num_of_nodes_in_chain(int key)
{
	return hash_table[hash_func(key)].size();
}

Node* HashTable::get_node_from_chain_by_index(int key, int index)
{
	list <Node*> :: iterator i;
	int count = 0;

	for (i = hash_table[hash_func(key)].begin(); count != index; i++) {
		count ++;
	}

	return *i;
}

int glue_two_nodes_and_get_min_side(Node* node0, Node* node1)
{
	int w0 = node0->get_width();
	int h0 = node0->get_height();
	int l0 = node0->get_length();
	int w1 = node1->get_width();
	int h1 = node1->get_height();
	int l1 = node1->get_length();	
	int w2 = 0, h2 = 0, l2 = 0;

	if ((w0 == w1) || (w0 == l1) || (w0 == h1)) {
		if (w0 == w1) {
			if (l0 == l1)
				return (h0 + h1);
			if (h0 == h1)
				return (l0 + l1);
			if (h0 == l1)
				return (h1 + l0);
			else
				return (h0 + l1);
		}

		if (w0 == l1) {
			if (l0 == w1)
				return (h0 + h1);
			if (l0 == h1)
				return (h0 + w1);
			if (h0 == w1)
				return (h1 + l0);
			if (h0 == h1)
				return (w1 + l0);
		}

		if (w0 == h1) {
			if (l0 == w1)
				return (h0 + l1);
			if (l0 == l1)
				return (h0 + w1);
			if (h0 == w1)
				return (l0 + l1);
			if (h0 == l1)
				return (l0 + w1);
		}
	} else {
		if ((l0 == l1) || (l0 == h1)) {
			return (w0 + w1);
		} else {
			return 0;
		}
	}
	return 0;
}

void glue_two_nodes(Node* node0, Node* node1)
{
	int w0 = node0->get_width();
	int h0 = node0->get_height();
	int l0 = node0->get_length();
	int w1 = node1->get_width();
	int h1 = node1->get_height();
	int l1 = node1->get_length();	
	int w2 = 0, h2 = 0, l2 = 0;

	// FIXME: Need to rewrite as if - else

	if ((w0 == w1) || (w0 == l1) || (w0 == h1)) {
		if (w0 == w1) {
			w2 = w0;
			if (l0 == l1) {
				l2 = l0;
				h2 = h0 + h1;
			}
			if (h0 == h1) {
				h2 = h0;
				l2 = (l0 + l1);
			}
			if (h0 == l1) {
				l2 = h0;
				h2 = h1 + l0;
			}
			if (h1 == l0) {
				l2 = l0;
				h2  = h0 + l1;
			}
			//cout << "here 0" << endl;
		}

		if (w0 == l1) {
			w2 = w0;
			if (l0 == w1) {
				l2 = l0;
				h2 = h0 + h1;
			}
			if (l0 == h1) {
				l2 = l0;
				h2 = h0 + w1;
			}
			if (h0 == w1) {
				l2 = h0;
				h2 =  h1 + l0;
			}
			if (h0 == h1) {
				l2 = h0;
				h2 =  w1 + l0;
			}
			//cout << "here 1" << endl;
		}

		if (w0 == h1) {
			w2 = w0;
			if (l0 == w1) {
				l2 = l0;
				h2 = h0 + l1;
			}
			if (l0 == l1) {
				l2 = l0;
				h2 =  h0 + w1;
			}
			if (h0 == w1) {
				l2 = h0;
				h2 = l0 + l1;
			}
			if (h0 == l1) {
				l2 = h0;
				h2 = l0 + w1;
			}
			//cout << "here 2" << endl;
		}
	} else {
		//cout << "here 3" << endl;
		if ((l0 == l1) || (l0 == h1)) {
			
			w2 = w0 + w1;
			l2 = l0;
			h2 = h0;
		}
		else {
			return;
		}
	}

	Node* dual_node = new Node(w2, h2, l2, 0);
	dual_node->set_parents(node0->get_node_index(), node1->get_node_index());

	if (check_max_node(&current_max_node, &dual_node))
		new_glued_nodes.push_back(dual_node);
	else
		delete dual_node;
}

void check_two_sides(HashTable* hash_table, Node* new_node, int side_0, int side_1)
{
	Node* node;
	int num_of_nodes = hash_table->get_num_of_nodes_in_chain(side_0 + side_1);
	int max_side = 0;	

	for (int i = 0; i < num_of_nodes; i++) {
		node = hash_table->get_node_from_chain_by_index(side_0 + side_1, i);		
		glue_two_nodes(node, new_node);
	}

}

void check_dual_nodes(HashTable* hash_table, Node* new_node)
{
	int w = new_node->get_width();
	int h = new_node->get_height();
	int l = new_node->get_length();

	check_two_sides(hash_table, new_node, w, h);
	check_two_sides(hash_table, new_node, w, l);
	check_two_sides(hash_table, new_node, l, h);		
}

int main()
{
	HashTable* hash_table = new HashTable(HTABLE_SIZE);

	current_max_node = new Node(0, 0, 0, 0);	// TODO: May be need to use just NULL pointer here, because memory may leaks
	
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
		hash_table->add_node(new_node, width + height);
		hash_table->add_node(new_node, width + length);
		hash_table->add_node(new_node, length + height);
	}

	int num_of_glued_boxes = current_max_node->is_glued() + 1;

	cout << num_of_glued_boxes << endl;

	if (current_max_node->is_glued()) {
		cout << current_max_node->get_small_parent() << " " << current_max_node->get_big_parent() << endl;

	} else {
		cout << current_max_node->get_node_index() << endl;
	}

	cout << current_max_node->get_width() << " " << current_max_node->get_height() << " " << current_max_node->get_length() << endl;

	return 0;
}
