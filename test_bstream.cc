#include <cstdio>
#include <fstream>

#include <gtest/gtest.h>

#include "bstream.h"

TEST(BStream, input) {
  std::string filename{"test_bstream_input"};

  const unsigned char val[] = {
    0x58, 0x90, 0xab, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0101100010010000101010110000100000000000010011101101101101000000
  // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we reading the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 0x62); // 01100010
  EXPECT_EQ(bis.GetChar(), 0x42); // 01000010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276); // 01011000010000000000001001110110

  ifs.close();

  std::remove(filename.c_str());
}

TEST(BStream, output) {
  std::string filename{"test_bstream_output"};
  // make an out-put file
  std::ofstream ofs(filename);
  BinaryOutputStream bos(ofs);

  // write char and ints in file
  bos.PutChar('S');
  bos.PutChar('u');
  bos.PutChar('b');
  bos.PutInt(0x58400276);
  ofs.close();
  // read in binary
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  //check what has been read in
  EXPECT_EQ(bis.GetChar(),0x53);
  EXPECT_EQ(bis.GetChar(),0x75);
  EXPECT_EQ(bis.GetChar(), 0x62);
  EXPECT_EQ(bis.GetInt(), 0x58400276);

  ifs.close();

  std::remove(filename.c_str());

}



TEST(BStream, char_output) {
  std::string filename{"test_bstream_output"};
  // make an out-put file
  std::ofstream ofs(filename);
  BinaryOutputStream bos(ofs);

  // write char in file
  bos.PutChar('S');
  ofs.close();
  // read in binary
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  //check what has been read in
  EXPECT_EQ(bis.GetChar(),0x53);
  ifs.close();

  std::remove(filename.c_str());

}


TEST(BStream, int_output) {
  std::string filename{"test_bstream_output"};
  // make an out-put file
  std::ofstream ofs(filename);
  BinaryOutputStream bos(ofs);

  // write  ints in file
  bos.PutInt(0x58400276);
  ofs.close();
  // read in binary
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  //check what has been read in
  EXPECT_EQ(bis.GetInt(),0x58400276);


  ifs.close();

  std::remove(filename.c_str());

}
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
