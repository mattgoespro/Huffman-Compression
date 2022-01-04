// Huffman Node class definitions

#include <iostream>
#include <memory>
#include <string>
#include "huffmannode.h"

using namespace std;

namespace YNGMAT005 {

	HuffmanNode::HuffmanNode(string l, int freq) {
		letter = l;
		frequency = freq;
	}

	HuffmanNode::~HuffmanNode() {
	}

	shared_ptr<HuffmanNode> & HuffmanNode::get_left() {
		return left;
	}

	shared_ptr<HuffmanNode> & HuffmanNode::get_right() {
		return right;
	}

	void HuffmanNode::set_left(shared_ptr<HuffmanNode> & l) {
		left = l;
	}

	void HuffmanNode::set_right(shared_ptr<HuffmanNode> & r) {
		right = r;
	}

	bool HuffmanNode::has_left() {
		return left != 0;
	}

	bool HuffmanNode::has_right() {
		return right != 0;
	}

	string HuffmanNode::get_letter() {
		return letter;
	}

	int HuffmanNode::get_frequency() {
		return frequency;
	}
}