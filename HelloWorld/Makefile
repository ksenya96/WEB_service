LIBS=-lcppcms

all: example

example: example.cpp access_db_data.cpp Record.cpp
	c++ --std=c++11 example.cpp access_db_data.cpp Record.cpp -lcppcms $(pkg-config --cflags --libs libmongocxx) -Wl,-rpath,/usr/local/lib

clean:
	rm -fr example
