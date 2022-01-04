// Driver class for Huffmap Compression algorithm

#include "huffmantree.h"
#include "huffmannode.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;
using namespace YNGMAT005;

// Handle user input and operations
void handleInput(int argc, char* argv[]) {
	switch(argc) {
		case 1:
			cout << "Please specify an input file." << endl;
			break;
		case 2:
			cout << "Please specify an output file." << endl;
			break;
		case 3:
			HuffmanTree tree;
			cout << "=============================================" << endl;
			cout << "Huffman Tree compression program running..." << endl;
			cout << "=============================================" << endl;
			cout << "Input file: " << argv[1] << endl;
			tree.set_input_file(argv[1]);
			cout << "Output file: " << argv[2] << endl;
			tree.set_output_file(argv[2]);
			cout << "Loading Huffman Tree data..." << endl;
			tree.load_data();

			if(tree.has_loaded()) {
				cout << "Data loaded successfully." << endl;
				cout << "Building Huffman Tree..." << endl;
				tree.build_tree();
				cout << "Exporting code table to \"" << string(argv[2]) << ".hdr\"..." << endl;
				tree.build_code_table(tree.get_root(), "");
				tree.export_code_table();
				cout << "Exporting 'compressed' data to \"" << string(argv[2]) << "\"..." << endl;
				tree.compress_data();
				cout << "Writing compressed bit stream to \"" << string(argv[2]) << ".bin\"..." << endl;
				tree.write_bits();
				cout << "Operations completed.\n" << endl;

				for(;;) {
					cout << "Perform an operation on the tree by choosing an option below:\n" << endl;
					cout << "1: Print Huffman Tree code table" << endl;
					cout << "2: Read bit stream from \"" << string(argv[2]) << ".bin\"" << endl;
					cout << "3: Print Huffman Tree representation" << endl;
					cout << "0: Quit" << endl;
					cout << "Enter an option: ";

					string input;
					getline(cin, input);

					if(input == "1") {
						system("clear");
						cout << "Huffman Tree code table:" << endl;
						tree.print_codes();
						cout << "\n";
					} else if(input == "2") {
						system("clear");
						cout << "Decompressed message: " << tree.read_bits() << endl;
						cout << "\n";
					} else if(input == "3") {
						system("clear");
						cout << "Huffman Tree representation:\n" << endl;
						cout << ":" << tree.get_root()->get_frequency() << endl;
						tree.print_tree(tree.get_root(), "");
						cout << "\n";
					} else if(input == "0") {
						break;
					}
				}
			} else {
				cout << "Error loading file: Either the file doesn't exist or it is empty." << endl;
			}
			break;
	}
	
}

// Main
int main(int argc, char* argv[]) {
	handleInput(argc, argv);
}