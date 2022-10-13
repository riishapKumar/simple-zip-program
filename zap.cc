#include <iostream>
#include <fstream>
#include <string>

#include "huffman.h"

int main(int argc, char *argv[]) {
// check for arguments
  if (argc != 3) {
    std::cerr << "Usage: ./zap <inputfile> <zapfile>" << std::endl;
    exit(1);
  }
// Open Input/ Output streams
  std::string input = argv[1];
  std::string output = argv[2];
  // Input Stream
  std::ifstream in;
  in.open(input);
  if (!in.good()) {
    std::cerr << "Could not open input file " << input << std::endl;
    exit(1);
  }
  // Output Stream
  std::ofstream out;
  out.open(output, std::ios::binary | std::ios::out | std::ios::trunc);
  if (!out.good()) {
    std::cerr << "Could not open output file " << output << std::endl;
    exit(1);
  }
// Compression
  Huffman File;
  File.Compress(in, out);
// Close Input/Output streams
  std::cout << "Compressed input file " << input <<
  " into zap file " << output << std::endl;
  in.close();
  out.close();
}
