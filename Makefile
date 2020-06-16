CXX=g++-9
FLAGS=-Wall -std=c++11

main: example.cpp
	$(CXX) $(FLAGS) -o example $^
