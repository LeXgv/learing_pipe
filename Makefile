DB = 0

all: pipe.cpp
	g++ -std=c++11 pipe.cpp -o pipe.prog

	
debug: pipe.cpp
	g++ -std=c++11 -D DEBUG=${DB} -g  pipe.cpp -o pipe_debug_${DB}
