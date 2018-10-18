LIBS=-lcppcms

all: example

example: example.cpp access_file_data.cpp Record.cpp
	$(CXX) -std=c++11 -O2 -Wall $(CXXFLAGS) -g example.cpp access_file_data.cpp Record.cpp -o example ${LIBS}

clean:
	rm -fr example
