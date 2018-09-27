LIBS=-lcppcms

all: example

example: example.cpp
	$(CXX) -O2 -Wall $(CXXFLAGS) -g example.cpp -o example ${LIBS}

clean:
	rm -fr example
