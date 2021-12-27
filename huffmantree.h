// Huffman Tree class header

#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "huffmannode.h"
#include <string>
#include <queue>
#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>
#include <map>

namespace YNGMAT005 {

	// Comparator class used to compare two nodes
	class Comparator {
		public:
			bool operator()(HuffmanNode & h1, HuffmanNode & h2);
	};

	// HuffmanTree class representation
	class HuffmanTree {

		private:
			std::priority_queue<HuffmanNode, std::vector<HuffmanNode>, Comparator> nodes;
			std::unordered_map<std::string, int> frequencies;
			std::unordered_map<std::string, std::string> code_table;
			std::shared_ptr<HuffmanNode> root;
			std::string input_file, output_file, encoded_data;
			std::vector<std::string> original_data;
			std::vector<std::shared_ptr<HuffmanNode>> all_nodes;
			bool loaded;

		public:
			// Special member functions

			// Default constructor
			HuffmanTree(void);

			// Destructor
			~HuffmanTree(void);

			// Copy constructor
			HuffmanTree(HuffmanTree & tree);

			// Move constructor
			HuffmanTree(HuffmanTree && tree);

			// Copy assignment operator
			HuffmanTree & operator=(const HuffmanTree & tree) {
				if(root != nullptr) {
      				root = nullptr;
    			}
				nodes = tree.nodes;
				frequencies = tree.frequencies;
				code_table = tree.code_table;
				root = tree.root;
				input_file = tree.input_file;
				output_file = tree.output_file;
				loaded = tree.loaded;
				return *this;
			}

			// Move assignment operator
			HuffmanTree & operator=(const HuffmanTree && tree) {
				if(root != nullptr) {
      				root = nullptr;
    			}
				nodes = std::move(tree.nodes);
				frequencies = std::move(tree.frequencies);
				code_table = std::move(tree.code_table);
				root = std::move(tree.root);
				input_file = std::move(tree.input_file);
				output_file = std::move(tree.output_file);
				loaded = std::move(tree.loaded);
				return *this;
			}

			std::shared_ptr<HuffmanNode> & get_root(void);
			// convenience method to create and build a Huffman Tree
			void run(void);
			// build tree
			void build_tree(void);
			// load data from text file
			void load_data(void);
			// build code table of characters
			void build_code_table(std::shared_ptr<HuffmanNode> root, std::string code);
			// write the code table to a test file
			void export_code_table(void);
			// write the 'compressed' bit stream to the output file
			void compress_data(void);

			// Extra credit

			// write the bitstream to a binary file
			void write_bits(void);
			// pack text file data bits into an array of unsigned chars
			void pack(unsigned char* bytes, int BUFFER_SIZE, std::vector<std::string> & data);
			// unpack text file data from an array of unsigned chars
			std::string unpack(unsigned char* bytes, int BUFFER_SIZE, int shift_offset);
			// read the bitstream from a binary file
			std::string read_bits(void);
			// search the Huffman Tree to decode the read bits
			std::string search_tree(std::shared_ptr<HuffmanNode> root, std::string code);		

// ==================== Methods for Testing ====================
			// Convenience constructor
			HuffmanTree(std::string input_file, std::string output_file);
			void set_output_file(std::string output_file);
			void set_input_file(std::string input_file);
			bool has_loaded(void);
			void print_tree(std::shared_ptr<HuffmanNode> root, std::string prefix);
			void print_codes(void);
			std::unordered_map<std::string, int> get_frequency_table();
			std::unordered_map<std::string, std::string> get_code_table();
			std::string get_encoded_data();
	};

}

#endif