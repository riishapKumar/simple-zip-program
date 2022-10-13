#include <iostream>
#include <fstream>
#include <string>

#include "huffman.h"

int main(int argc, char *argv[]) {
// check for arguments
  if (argc != 3) {
    std::cerr << "Usage: ./unzap <zapfile> <outputfile>" << std::endl;
    exit(1);
  }
// Open Input/ Output streams
  std::string input = argv[1];
  std::string output = argv[2];
  // Input Stream
  std::ifstream in;
  in.open(input, std::ios::binary | std::ios::in);
  if (!in.good()) {
    std::cerr << "Could not open input file " << input << std::endl;
    exit(1);
  }
  // Output Stream
  std::ofstream out;
  out.open(output, std::ios::out | std::ios::trunc);
  if (!out.good()) {
    std::cerr << "Could not open output file " << output << std::endl;
    exit(1);
  }
// Decompression
  Huffman File;
  File.Decompress(in, out);
// Close Input/Output streams
  std::cout << "Decompressed zap file " << input <<
  " into output file " << output << std::endl;
  in.close();
  out.close();
}


