import random
import numpy


num_of_lines = numpy.random.randint(low = 8, high = 20)
test_file = open("input.txt", "w+")

test_file.write("%d\n" % num_of_lines)

for i in range (num_of_lines):
	test_file.write("%d " % numpy.random.randint(low = 4, high = 15))	
	test_file.write("%d " % numpy.random.randint(low = 4, high = 15))
	test_file.write("%d\n" % numpy.random.randint(low = 4, high = 15))