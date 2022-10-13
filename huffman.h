#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <map>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq,
                       HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) { }


  bool IsLeaf() {
    // Node is a leaf if it doesn't have any children
    return left_ == nullptr && right_ == nullptr;
  }

  bool operator < (const HuffmanNode &n) const {
    // In case of equality, make it deterministic based on character
    if (freq_ == n.freq_)
      return ch_ < n.ch_;
    // Otherwise compare frequencies
    return freq_ < n.freq_;
  }

  size_t freq() { return freq_; }
  size_t data() { return ch_; }
  HuffmanNode* left() { return left_; }
  HuffmanNode* right() { return right_; }

 private:
  char ch_;
  size_t freq_;
  HuffmanNode *left_, *right_;
};

class Huffman {
 public:
  static void Compress(std::ifstream &ifs, std::ofstream &ofs);

  static void Decompress(std::ifstream &ifs, std::ofstream &ofs);

 private:
  // Helper methods...
  // Creates Tree
  static HuffmanNode* HTree(const std::string& contents);
  // Encodes Tree
  static void encoder(std::map<char, std::string> &HMap,
      const std::string& str, HuffmanNode *root,
      BinaryOutputStream &out);
  // Decodes Tree
  static void decoder(HuffmanNode *&root, BinaryInputStream &in);
};
// To be completed below

// Compression Algorithm
void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  // open, read file to string
  std::string contents;
  // Read to string
  // Used code from link provided by Prof.
  if (ifs) {
    ifs.seekg(0, std::ios::end);
    contents.resize(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    ifs.read(&contents[0], contents.size());
    ifs.close();
  }
  // Open Binary Stream Out
  BinaryOutputStream out(ofs);
  // Create the tree
  HuffmanNode *root = HTree(contents);
  // Code_Table and Tree Encoding
  std::map<char, std::string> code_table;
  std::string path;
  encoder(code_table, path, root, out);
  // Table to String
  std::string Code_Table_str;
  for (char ch : contents)
    Code_Table_str.append(code_table[ch]);
  // Output
  out.PutInt(root->freq());
  for (char ch : Code_Table_str)
    out.PutBit(ch != '0');
  // Close Binary Stream
  out.Close();
}

// Custom Comparator
template <class T> struct PtrComp {
  bool operator()(T *ptr, T *ptr_n){
    return *ptr < *ptr_n;
  }
};

// Recursively Encodes the Tree / Creates Code_Table
void Huffman::encoder(std::map<char, std::string> &HMap,
                      const std::string& str, HuffmanNode *root,
                      BinaryOutputStream &out) {
  // Traverse the tree
  if (root != nullptr) {
    if (root->IsLeaf()) {
      // Output 1 bit
      out.PutBit(true);
      // Output ASCII byte for data
      out.PutChar(root->data());
      HMap[root->data()] = str;
    } else {
      // Output 0 bit
      out.PutBit(false);
    }
    // add 0 to string and go left
    encoder(HMap, str + '0', root->left(), out);
    // add 1 to string and go right
    encoder(HMap, str + '1', root->right(), out);
  }
}

// Creates and returns a Huffman Tree
HuffmanNode* Huffman::HTree(const std::string& contents) {
  // Create PQueue for Huffman Nodes
  PQueue<HuffmanNode*, PtrComp<HuffmanNode>> PQ;
  // Create Map
  std::map<char, int> freq;
  for (char ch : contents)
    freq[ch]++;
  // Push to PQ
  for (auto it : freq) {
    auto *nnode = new HuffmanNode(it.first, it.second);
    PQ.Push(nnode);
  }
  // Huffman algorithm
  while (PQ.Size() != 1) {
    auto *left = PQ.Top();
    PQ.Pop();
    auto *right = PQ.Top();
    PQ.Pop();
    int sum = right->freq() + left->freq();
    auto *inter = new HuffmanNode('\0', sum, left, right);
    PQ.Push(inter);
  }
  return PQ.Top();
}

// Decompression Algorithm
void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {
  // Open Binary Input Stream
  BinaryInputStream in(ifs);
  // Reconstruct the HTree from bits
  HuffmanNode *Tree;
  decoder(Tree, in);
  // Get the total number of chars
  int Total = in.GetInt();
  // Open Binary output Stream
  BinaryOutputStream out(ofs);
  // Start reading chars
  for (int i = 0; i < Total; i++) {
    HuffmanNode *node = Tree;
    // Follow Tree using Code_Table
    while (!node->IsLeaf()) {
      if (in.GetBit())
        node = node->right();
      else
        node = node->left();
    }
    // Output Chars
    out.PutChar(node->data());
  }
  // Close Binary Output Stream
  out.Close();
}

// Recursive algorithm to decode Huffman table
void Huffman::decoder(HuffmanNode *&root, BinaryInputStream &in) {
  if (in.GetBit()) {
    // Found Leaf Node
    root = new HuffmanNode(in.GetChar(), 0);
    return;
  } else {
    HuffmanNode *left = nullptr;
    HuffmanNode *right = nullptr;
    decoder(left, in);
    decoder(right, in);
    root = new HuffmanNode('\0', 0, left, right);
  }
}

#endif  // HUFFMAN_H_
