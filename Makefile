CXX=g++
CXXFLAGS=-g -Wall -Werror -std=c++11

all: zap unzap test_bstream test_pqueue

zap: zap.cc huffman.h bstream.h pqueue.h
	$(CXX) $(CXXFLAGS) zap.cc -o zap

unzap: unzap.cc huffman.h bstream.h pqueue.h
	$(CXX) $(CXXFLAGS) unzap.cc -o unzap

test_bstream: test_bstream.cc bstream.h
	$(CXX) $(CXXFLAGS) test_bstream.cc -o test_bstream -pthread -lgtest
	
test_pqueue: test_pqueue.cc pqueue.h
	$(CXX) $(CXXFLAGS) test_pqueue.cc -o test_pqueue -pthread -lgtest

clean: 
	rm zap unzap test_bstream test_pqueue