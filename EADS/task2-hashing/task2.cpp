#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;


int main()
{
	std::ifstream infile("input.txt");

	//std::string line;
	//std::getline(infile, line);

	int num_of_boxes = 0;
	infile >> num_of_boxes;
//	cout << num_of_boxes << endl;

	int width  = 0, height = 0, length = 0;

	for (int i = 0; i < num_of_boxes; i++) {
		infile >> width >> height >> length;

//		cout << width << height << length << endl;
	}


	return 0;
}