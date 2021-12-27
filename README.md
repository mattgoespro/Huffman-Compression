-------------------------------------------
CSC3022H Assignment 3 - YNGMAT005

README File
-------------------------------------------

Please note: The properly compressed binary files are
larger than expected, since writing the number of bits
as a header in the binary file drastically increases the compressed
file's size.


File Descriptions:

	huffmannode.h:
		- Header file for the "huffmannode.cpp" class file.
		- Defines method stubs.
		- Defines some helper methods for the cpp file.
	huffmannode.cpp:
		- Main .cpp file that defines the method bodies of the 
		  HuffmanNode class functions.
		- Defines use of nodes within the HuffmanTree class.
		- Contains helper methods to use during unit testing.
	huffmantree.h:
		- Header file for the "huffmantree.cpp" class file.
		- Defines method stubs for the main functions of the
		  program.
		- Defines assignment operators.
	huffmantree.cpp:
		- Main .cpp file that defines the method bodies of the 
		  HuffmanTree class functions.
		- Defines the HuffmanTree constructors/destructor method(s).
		- Contains helper methods to use during unit testing.
	huffmandriver.cpp:
		- The driver class containing command line parsing
		  through the main() method.
		- Used to load and build the Huffman Tree.
		- Handles user operations through the terminal.
	huffmannodetests.cpp:
		- Unit tests for the HuffmanNode class operations.
	huffmantreetests.cpp:
		- Unit tests for the HuffmanTree class operations.
	makefile:
		- Used to compile the program. Type "make" in console to compile the files.
		- Type "make clean" to clean the binary, 'hdr', and text files
		  from the project directory.
		- Type "make test" to run the unit tests for the HuffmanNode and HuffmanTree
		  classes. Please note: Tests may take a while to complete.
 
Program Usage:
	
	- Any incorrect usage of the program will give the user feedback
	on what went wrong during execution.

	- The program is run using the following command-line arguments:
		> ./huffencode <input_file> <output_file>

	The path to the folder that contains the input text file has to be specified.
	The program will automatically add extensions to your file names.
		- Example input: ./huffencode Test\ Files/test5 test5_output