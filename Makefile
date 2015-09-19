CXX=clang++
CXXFLAGS=-O3 -Wall -Werror -pedantic -std=c++14

units: units.cc

clean:
	rm -f units
