// Huffman Node class header

#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

#include <memory>
#include <string>

namespace YNGMAT005 {

	class HuffmanNode {
		private:
			std::shared_ptr<HuffmanNode> left;
			std::shared_ptr<HuffmanNode> right;
			std::string letter;
			int frequency;

		public:
			HuffmanNode(std::string l, int freq);
			~HuffmanNode(void);
			std::shared_ptr<HuffmanNode> & get_left(void);
			std::shared_ptr<HuffmanNode> & get_right(void);
			void set_left(std::shared_ptr<HuffmanNode> & l);
			void set_right(std::shared_ptr<HuffmanNode> & r);
			bool has_left(void);
			bool has_right(void);
			std::string get_letter(void);
			int get_frequency(void);
	};

}

#endif
