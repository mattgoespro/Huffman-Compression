// Huffman Tree class definitions

#include "huffmantree.h"
#include "huffmannode.h"
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <queue>
#include <vector>
#include <iostream>
#include <sstream>
#include <utility>
#include <math.h>

using namespace std;

namespace YNGMAT005 {

  // Default Constructor
  HuffmanTree::HuffmanTree() {
    loaded = false;
    root = nullptr;
  }

  // Testing Constructor
  HuffmanTree::HuffmanTree(string input_file, string output_file) {
    this->input_file = input_file;
    this->output_file = output_file;
    this->run();
  }

  // Copy Constructor
  HuffmanTree::HuffmanTree(HuffmanTree & tree) {
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
  }

  // Move Constructor
  HuffmanTree::HuffmanTree(HuffmanTree && tree) {
    if(root != nullptr) {
      root = nullptr;
    }

    nodes = move(tree.nodes);
    frequencies = move(tree.frequencies);
    code_table = move(tree.code_table);
    root = move(tree.root);
    input_file = move(tree.input_file);
    output_file = move(tree.output_file);
    loaded = move(tree.loaded);
  }

  // Destructor
  HuffmanTree::~HuffmanTree() {
    root = nullptr;
  }

  void HuffmanTree::run() {
    // load data from text file
    this->load_data();

    // if loading was successful, proceed with
    // the rest of the building operations
    if(loaded) {
      this->build_tree();
      this->build_code_table(root, "");
      this->export_code_table();
      this->compress_data();
    }
  }

  shared_ptr<HuffmanNode> & HuffmanTree::get_root() {
    return root;
  }

  void HuffmanTree::build_tree() {
    // push nodes into the priority queue
    for(auto x: frequencies) {
        HuffmanNode n(x.first, x.second);
        nodes.push(n);
    }

    // build tree
    while(nodes.size() != 1) {
      // create pointers for the lowest frequency letters 
      shared_ptr<HuffmanNode> left = make_shared<HuffmanNode>(nodes.top());
      nodes.pop();
      shared_ptr<HuffmanNode> right = make_shared<HuffmanNode>(nodes.top());
      nodes.pop();

      // create parent node and set attributes of parent and siblings, then push parent
      HuffmanNode p("", left->get_frequency() + right->get_frequency());
      shared_ptr<HuffmanNode> parent = make_shared<HuffmanNode>(p);
      parent->set_left(left);
      parent->set_right(right);
      nodes.push(*parent);
    }

    // last node is the root, so pop it out
    root = std::make_shared<HuffmanNode>(nodes.top());
    nodes.pop();
  }

  void HuffmanTree::load_data() {
    ifstream data(input_file + ".txt");

    // if file not found
    if(!data) {
      loaded = false;
      return;
    }

    // if file is empty
    if(data.peek() == ifstream::traits_type::eof()) {
      loaded = false;
      return;
    }

    loaded = true;
    string line;

    // read file
    while(!data.eof()) {
      getline(data, line);
      original_data.push_back(line);

      // update frequency table
      for(int i = 0; i < line.length(); i++) {
        string let(1, line[i]);
        ++frequencies[let];
      }
    }
  }

  void HuffmanTree::build_code_table(shared_ptr<HuffmanNode> root, string code) {
    // traverse left subtree if child found, and add 0 to code
    if(root->has_left()) {
      this->build_code_table(root->get_left(), code + "0");
    }

    // traverse right subtree if child found, and add 1 to code
    if(root->has_right()) {
      this->build_code_table(root->get_right(), code + "1");
    }

    // if no left or right child, insert current node's letter
    // and code to the code table
    code_table.insert(pair<string, string>(root->get_letter(), code));
  }

  void HuffmanTree::export_code_table() {
    // write code table to output file
    ofstream codeStream(output_file + ".hdr");
    codeStream << code_table.size() << endl;

    for(auto x: code_table) {
        codeStream << x.first << ":" << x.second << endl;
    }
    codeStream.close();
  }

  void HuffmanTree::compress_data() {
    ofstream compressed(output_file + ".txt");
    string buffer;
    string line;

    for(string line : original_data) {
      // write letter codes to a string buffer
      for(int i = 0; i < line.size(); i++) {
        char a = line[i];
        string let(1, line[i]);
        buffer += code_table[let];
      }
    }
    compressed << buffer.c_str();
    compressed.close();
  }

  // helper method to test tree
  void HuffmanTree::print_tree(shared_ptr<HuffmanNode> root, string prefix) {
      if(loaded) {
        // if right node at current root, traverse subtree and print
        if(root->has_right()) {
          cout << prefix << "|" << endl;
          cout << prefix << "|__" << root->get_right()->get_letter() << ":" << root->get_right()->get_frequency() << endl;
          this->print_tree(root->get_right(), prefix + "|  ");
        } else {
          cout << prefix << "|__" << endl;
        }
        
        // if left node at current root, traverse subtree and print
        if(root->has_left()) {
          cout << prefix << "|"<<endl;
          cout << prefix << "|__" << root->get_left()->get_letter() << ":" << root->get_left()->get_frequency() << endl;
          this->print_tree(root->get_left(), prefix + "   ");
        } else {
          cout << prefix << "|__" << endl;
        }
      }
  }

  void HuffmanTree::write_bits() {
    ofstream bit_file(output_file + ".bin", ios::binary);
    int size = 0;

    // find and write number of bits in the file data
    for(auto& line : original_data) {
      for(auto& x : line) {
        string ch(1, x);
        size += code_table[ch].size();
      }
    }
    bit_file << size << endl;

    // byte buffer - minimum bytes needed to compress the file
    int c_size = (size%8 ? floor(int(size/8)) + 1 : size/8);
    unsigned char* bytes = new unsigned char[c_size];

    // pack the bits into the byte buffer and write out to 
    // the binary file
    this->pack(bytes, c_size, original_data);
    for(int x = 0; x < c_size; x++) {
      bit_file << bytes[x];
    }
    bit_file.close();
    delete [] bytes;  
  }

  void HuffmanTree::pack(unsigned char* bytes, int BUFFER_SIZE, vector<string> & data) {
    // set all byte values to 0 - not doing this caused strange behaviour
    // when writing
    for(int index = 0; index < BUFFER_SIZE; index++) {
      bytes[index] = 0;
    }

    int c_index = 0;        // byte index
    int bit_pointer = 7;    // bit position in current byte
    int num_bits = 0;       // bit counter

    for(auto& line : data) {
      // for all bytes in each line
      for(int c = 0; c < line.size(); c++) {
        string letter(1, line[c]);
        string code = code_table[letter];
        int code_size = code.size();
        int bit_index = 0;

        // while there are bits to write
        while(bit_index < code_size) {
          num_bits++;
          string nib(1, code[bit_index]);
          int nib_val = stoi(nib);
          // write bit to current working byte at bit position 'bit_pointer'
          bytes[c_index] |= (nib_val << bit_pointer);
          bit_pointer--;
          bit_index++;

          // if not done writing all bits of current code,
          // prevent overflow and start writing in the next
          // byte
          if(bit_pointer < 0) {
            c_index++;
            bit_pointer = 7;
          }
        }
      }
    }

    // pad the last byte with 0s if it contains less than 8 bits
    int padding = BUFFER_SIZE*8-num_bits;
    for(int pads = 0; pads < padding; pads++) {
      bytes[BUFFER_SIZE] |= (0 << (bit_pointer-pads));
      bit_pointer--;
    }
  }

  string HuffmanTree::unpack(unsigned char* bytes, int BUFFER_SIZE, int shift_offset) {
    string unpacked = "";
    for(int i = 0; i < BUFFER_SIZE; i++) {
      unsigned char byte = bytes[i];
      // extract bits from byte and add to buffer
      for(int bit = 0; bit < 8; bit++) {
        unpacked += byte & (1 << 7-bit) ? "1" : "0";
      }
    }
    // if there were padded bits, remove them
    return unpacked.substr(0, unpacked.size()-shift_offset);
  }

  string HuffmanTree::read_bits() {
    ifstream bit_file(output_file + ".bin", ios::binary);

    // get number of bits in the file
    string size;
    getline(bit_file, size);
    int s = stoi(size);

    // get number of bytes from number of bits
    int num_bytes = ceil(int(s/8)) + 1;
    int shift_offset = num_bytes*8 - s;

    // start reading unsigned chars from second line
    bit_file.seekg(size.size() + 1, ios::beg);

    // read and unpack data
    unsigned char* bytes = new unsigned char[num_bytes];
    bit_file.read((char*)bytes, num_bytes);
    bit_file.close();
    string unpacked = this->unpack(bytes, num_bytes, shift_offset);
    cout << "Unpacked bits: " << unpacked << endl;

    // decode unpacked data
    string decoded;
    string dec_buffer;

    // do a longest prefix search to decode message
    for(int i = 0; i < unpacked.size(); i++) {
      string bit(1, unpacked[i]);
      dec_buffer = dec_buffer + bit;
      // search tree
      string letter = this->search_tree(this->get_root(), dec_buffer);

      if(letter != "") {
        decoded += letter;
        dec_buffer = "";
      }
    }

    // clean up memory
    delete [] bytes;
    return decoded;
  }

  string HuffmanTree::search_tree(shared_ptr<HuffmanNode> root, string code) {
    string bit(1, code[0]);

    // if bit is 0, branch left ---- below is same thing for branching right
    if(bit == "0") {
      // if root has left, proceed, else return leaf's letter
      if(root->has_left()) {
        // if root has left and code length > 0, then traverse left subtree
        // else, root has letter ? return letter : return "".
        if(code.size() > 0) {
          return this->search_tree(root->get_left(), code.substr(1, code.size()-1));
        } else {
          if(root->get_letter() != "") {
            return root->get_letter();
          } else {
            return "";
          }
        }
      } else {
        return root->get_letter();
      }
    } else if(bit == "1") {
      if(root->has_right()) {
        if(code.size() > 0) {
          return this->search_tree(root->get_right(), code.substr(1, code.size()-1));
        } else {
          if(root->get_letter() != "") {
            return root->get_letter();
          } else {
            return "";
          }
        }
      } else {
        return root->get_letter();
      }
    } else {
      if(root->get_letter() != "") {
        return root->get_letter();
      } else {
        return "";
      }
    }
    return "";
  } 

  bool Comparator::operator()(HuffmanNode & h1, HuffmanNode & h2) {
    return h1.get_frequency() > h2.get_frequency();
  }

// ==================== Methods for Testing ====================

  void HuffmanTree::print_codes() {
    if(loaded) {
      for(auto x: code_table) {
        cout << x.first << ":" << x.second << endl;
      } 
    }
  }

  void HuffmanTree::set_output_file(string output_file) {
    this->output_file = output_file;
  }

  void HuffmanTree::set_input_file(string input_file) {
    this->input_file = input_file;
  }

  bool HuffmanTree::has_loaded() {
    return loaded;
  }

  unordered_map<string, int> HuffmanTree::get_frequency_table() {
    return frequencies;
  }

  unordered_map<string, string> HuffmanTree::get_code_table() {
    return code_table;
  }

  string HuffmanTree::get_encoded_data() {
    return encoded_data;
  }
}