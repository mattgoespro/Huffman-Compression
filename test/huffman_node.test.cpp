// Test class to test all Huffman Node methods

#include "huffmantree.h"
#include "huffmannode.h"
#include <memory>
#include <string>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;
using namespace YNGMAT005;

SCENARIO("Huffman Node is built correctly", "[HuffmanNode]") {
	GIVEN("A node and a key-value pair") {
		HuffmanNode node("P", 1);

		WHEN("The node is constructed") {
			REQUIRE(node.get_left() == nullptr);
			REQUIRE(node.get_right() == nullptr);
			REQUIRE(node.get_letter() == "P");
			REQUIRE(node.get_frequency() == 1);
		}

		WHEN("A child node is set") {
			HuffmanNode node_ins("Q", 0);
			shared_ptr<HuffmanNode> nptr = make_shared<HuffmanNode>(node_ins);
			HuffmanNode node_ins2("D", 4);
			shared_ptr<HuffmanNode> nptr2 = make_shared<HuffmanNode>(node_ins);

			REQUIRE(node.has_left() == false);
			REQUIRE(node.has_right() == false);

			node.set_left(nptr);
			node.set_right(nptr2);

			THEN("The left or right node should be set") {
				REQUIRE(node.get_left() == nptr);
				REQUIRE(node.get_right() == nptr2);
				REQUIRE(node.has_left() == true);
				REQUIRE(node.has_right() == true);
			}
		}
	}
}



