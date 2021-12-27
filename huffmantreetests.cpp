// Test class to test all Huffman Node methods

#include "huffmantree.h"
#include "huffmannode.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <cstdlib>
#include "catch.hpp"

using namespace std;
using namespace YNGMAT005;

SCENARIO("Huffman Tree can load data successfully", "[HuffmanTree]") {
	GIVEN("A default tree") {
		HuffmanTree test;

		WHEN("The input and output file are set") {
			test.set_output_file("Test Files/test1_out");

			THEN("Files not found result in unloaded data") {
				// this file directory does not exist
				test.set_input_file("Test Files/test");
				test.load_data();
				REQUIRE(test.has_loaded() == false);
			}
			
			THEN("The file found should load successfully") {
				// this file directory does exist
				test.set_input_file("Test Files/test1");
				test.load_data();
				REQUIRE(test.has_loaded() == true);
			}
		}
	}
}

SCENARIO("The code table is populated correctly") {
	GIVEN("A loaded and built Huffman Tree") {
		HuffmanTree test("Test Files/test3", "test3_out");
		
		/*
		|__:8
		|  |
		|  |__a:4
		|  |  |__
		|  |  |__
		|  |
		|  |__c:4
		|     |__
		|     |__
		|
		|__:8
		   |
		   |__b:4
		   |  |__
		   |  |__
		   |
		   |__d:4
		      |__
		      |__
		      */

		THEN("From the tree above and the rules for building each letter code") {
			unordered_map<string, string> code_table = test.get_code_table();
			REQUIRE(code_table["a"] == "11");
			REQUIRE(code_table["b"] == "00");
			REQUIRE(code_table["c"] == "01");
			REQUIRE(code_table["d"] == "10");
		}
	}

	GIVEN("A loaded and built Huffman Tree") {
		HuffmanTree test("Test Files/test4", "test4_out");

		THEN("From the rules for building each letter code") {
			unordered_map<string, string> code_table = test.get_code_table();
			REQUIRE(code_table["r"] == "100");
			REQUIRE(code_table["v"] == "1110");
			REQUIRE(code_table["l"] == "00");
			REQUIRE(code_table["d"] == "101");
			REQUIRE(code_table["i"] == "010");
			REQUIRE(code_table["n"] == "1111");
			REQUIRE(code_table["s"] == "110");
			REQUIRE(code_table["b"] == "011");
		}
	}
}

SCENARIO("The frequency table is populated correctly") {
	GIVEN("An empty Huffman Tree") {
		HuffmanTree test;

		// test3.txt data: "aaaabbbbccccdddd"
		test.set_input_file("Test Files/test3");
		test.set_output_file("test3_out");

		WHEN("The tree is loaded") {
			test.load_data();

			THEN("The frequency table should be populated") {
				unordered_map<string, int> table = test.get_frequency_table();
				REQUIRE(table["a"] == 4);
				REQUIRE(table["b"] == 4);
				REQUIRE(table["c"] == 4);
				REQUIRE(table["d"] == 4);
				REQUIRE(table["e"] == 0);
			}
		}
	}
}

SCENARIO("The nodes in the priority queue are ordered properly") {
	GIVEN("Huffman Nodes with letters, frequencies, and a priority queue with a comparator") {
		HuffmanNode n1("A", 1), n2("B", 6), n3("C", 10), n4("D", 3);
		priority_queue<HuffmanNode, vector<HuffmanNode>, Comparator> nodes;

		WHEN("A node is pushed into the queue") {
			nodes.push(n3);

			THEN("A node with a lower frequency is put at the front of the queue") {
				nodes.push(n2);
				nodes.push(n4);
				nodes.push(n1);
				HuffmanNode n = nodes.top();
				REQUIRE(n.get_letter() == "A");
				nodes.pop();
				n = nodes.top();
				REQUIRE(n.get_letter() == "D");
				nodes.pop();
				n = nodes.top();
				REQUIRE(n.get_letter() == "B");
				nodes.pop();
				n = nodes.top();
				REQUIRE(n.get_letter() == "C");
				nodes.pop();
			}
		}
	}
}

SCENARIO("Huffman Tree can be built properly", "[HuffmanTree]") {
	GIVEN("A successfully loaded Huffman Tree") {
		HuffmanTree tree;
		tree.set_input_file("Test Files/test2");
		tree.set_output_file("test2_out");
		tree.load_data();

		WHEN("The tree is built") {
			tree.build_tree();

			THEN("There has to be a root") {
				REQUIRE(tree.get_root() != nullptr);
			}

			shared_ptr<HuffmanNode> root = tree.get_root();

			THEN("The root must have a left and right node") {
				REQUIRE(root->has_left() == true);
				REQUIRE(root->has_right() == true);
			}

			THEN("The frequency of any node = sum of the frequencies of its children") {
				shared_ptr<HuffmanNode> left = root->get_left();
				shared_ptr<HuffmanNode> right = root->get_right();
				REQUIRE(root->get_frequency() == left->get_frequency() + right->get_frequency());
				root = root->get_right();
				left = root->get_left();
				right = root->get_right();
				REQUIRE(root->get_frequency() == left->get_frequency() + right->get_frequency());
			}

			THEN("All leaf nodes have letters and frequencies") {
				while(true) {
					if(root->has_right()) {
						root = root->get_right();
					} else {
						REQUIRE(root->get_letter() != "");
						break;
					}
				}

				while(true) {
					if(root->has_left()) {
						root = root->get_left();
					} else {
						REQUIRE(root->get_letter() != "");
						break;
					}
				}
			}
		}
	}
}

SCENARIO("Bit stream reading and writing works correctly") {
	GIVEN("A loaded and built Huffman Tree") {
		HuffmanTree test("Test Files/test5", "test5_out");

		WHEN("The binary file is created") {
			test.write_bits();

			THEN("The original data should be received when reading the binary file") {
				ifstream in("Test Files/test5.txt");
				string data;
				getline(in, data);
				string decoded_msg = test.read_bits();
				REQUIRE(decoded_msg == data);
			}
		}

		WHEN("Random characters are written to a file") {
			ofstream random("random_chars.txt");
			vector<unsigned char> characters;
			// write 40 characters in a random order
			int num_chars = rand()%40;

			for(int i = 0; i < num_chars; i++) {
				int random_int = rand()%2;
				if(random_int == 0) {
					random << 'a';
					characters.push_back('a');
				} else {
					random << 'b';
					characters.push_back('b');
				}  
			}
			random.close();
			HuffmanTree test2("random_chars", "random_chars");
			test2.write_bits();

			THEN("The input should be equal to the output") {
				string decoded_msg = test2.read_bits();

				for(int j = 0; j < num_chars; j++) {
					REQUIRE(characters[j] == decoded_msg[j]);
				}
			}
		}
	}
}

SCENARIO("The Huffman Tree can decode a binary message to an ASCII message") {
	GIVEN("A correctly loaded and built HuffmanTree and its code table") {
		HuffmanTree tree("Test Files/test3", "test3_out");
		unordered_map<string, string> code_table = tree.get_code_table();

		THEN("Searching for a letter's binary code should return that letter") {
			for(auto& code : code_table) {
				REQUIRE(tree.search_tree(tree.get_root(), code.second) == code.first);
			}
		}

		THEN("Searching for a code that doesn't exist should return nothing") {
			REQUIRE(tree.search_tree(tree.get_root(), "0") == "");
		}
	}
}

SCENARIO("Assignment operators and constructors/destructor works properly") {
	GIVEN("A correctly loaded and built HuffmanTree") {
		HuffmanTree tree("Test Files/test3", "test3_out");

		WHEN("An empty tree is created and assigned to the one given") {
			HuffmanTree tree2 = tree;

			THEN("The second tree should share the same values as the first") {
				REQUIRE(tree2.get_root() == tree.get_root());
				REQUIRE(tree2.get_frequency_table() == tree.get_frequency_table());
				REQUIRE(tree2.get_code_table() == tree.get_code_table());
			}
		}

		WHEN("A copy of the first tree is made") {
			HuffmanTree tree2(tree);

			THEN("The second tree should share the same values as the first") {
				REQUIRE(tree2.get_root() == tree.get_root());
				REQUIRE(tree2.get_frequency_table() == tree.get_frequency_table());
				REQUIRE(tree2.get_code_table() == tree.get_code_table());
			}
		}

		WHEN("The first tree is moved by constructor to the second tree") {
			shared_ptr<HuffmanNode> tree_root = tree.get_root();
			unordered_map<string, string> codes = tree.get_code_table();
			unordered_map<string, int> frequencies = tree.get_frequency_table();
			HuffmanTree tree2(move(tree));

			THEN("The first tree's resources should be in the second") {
				REQUIRE(tree2.get_root() == tree_root);
				REQUIRE(tree2.get_code_table() == codes);
				REQUIRE(tree2.get_frequency_table() == frequencies);
				REQUIRE(tree.get_root() == nullptr);
			}
		}

		WHEN("The first tree is moved by assignment to the second tree") {
			HuffmanTree tree2("Test Files/test3", "test3_out");
			shared_ptr<HuffmanNode> tree_root = tree2.get_root();
			unordered_map<string, string> codes = tree2.get_code_table();
			unordered_map<string, int> frequencies = tree2.get_frequency_table();
			HuffmanTree tree3 = move(tree2);

			THEN("The first tree's resources should be in the second") {
				REQUIRE(tree3.get_root() == tree_root);
				REQUIRE(tree3.get_code_table() == codes);
				REQUIRE(tree3.get_frequency_table() == frequencies);
				REQUIRE(tree2.get_root() == nullptr);
			}
		}
	}
}