all: serialize_root_writer_test serialize_root_reader_test
serialize_root.o: serialize_root.cc serialize_root.h
	c++ -c -o $@ $< -O3 -std=c++14 `root-config --cflags`

%_test: %_test.cc serialize_root.o
	c++ -o $@ $^  -I ../src -L ../src -lstorage `root-config --libs` -std=c++14

