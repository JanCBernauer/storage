all: libstorage.so storage_writer_test storage_reader_test

libstorage.so: storage.o
	c++ -shared -o $@ $< -lflatbuffers

storage.o:  storage.cc storage.h event_generated.h index_generated.h
	c++ -o $@ -c $< -g  -O2  -std=c++14

%_generated.h: ../schema/%.fbs
	flatc --cpp --gen-object-api $<

storage_writer_test: storage_writer_test.cc libstorage.so
	c++ -o $@ $< -L. -lstorage  -std=c++14 -g -O2

storage_reader_test: storage_reader_test.cc libstorage.so
	c++ -o $@ $< -L. -lstorage  -std=c++14 -g -O2

clean:
	rm *_generated.h storage.o libstorage.so storage_writer_test storage_reader_test

