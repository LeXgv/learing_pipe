DB = 0
	
ded: pipe.cpp
	g++ -std=c++11 -D DEBUG=${DB} pipe.cpp -o pipe_debug_${DB}
