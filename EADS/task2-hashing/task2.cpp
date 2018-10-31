#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Node {
	private:
		int width;
		int height;
		int length;
		Node* next;

	public:
		Node(int w = 0, int h = 0, int l = 0);
		~Node();		

		int get_width();
		int get_height();
		int get_length();
};

Node::Node(int w, int h, int l)
{
	width = w;
	height = h;
	length = l;
}

int Node::get_width() { return width; }
int Node::get_height() { return height; }
int Node::get_length() { return length; }

Node* current_max_node;

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


void check_max_node(Node** cur_node, Node** new_node)
{
	Node* c_node = *cur_node;
	Node* n_node = *new_node;

	int cur_min = get_mix_side_of_box(c_node);
	int new_min = get_mix_side_of_box(n_node);

	if (cur_min < new_min)
		*cur_node = *new_node;
}

int main()
{
	current_max_node = new Node(0, 0, 0);	// TODO: May be need to use just NULL pointer here, because memory may leaks


	std::ifstream infile("input.txt");

	int num_of_boxes = 0;
	infile >> num_of_boxes;

	
	int width  = 0, height = 0, length = 0;
	Node* new_node;

	for (int i = 0; i < num_of_boxes; i++) {
		infile >> width >> height >> length;
		new_node = new Node(width, height, length);

		check_max_node(&current_max_node, &new_node);
	}

	cout << current_max_node->get_width() << current_max_node->get_height() << current_max_node->get_length() << endl;

	return 0;
}
