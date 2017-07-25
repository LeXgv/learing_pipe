DB = 
DEBUG = 
ifdef DB
	echo Режим дебага функции ${DB}
	DEBUG = -D DEBUG=${DB}
endif	
ded: pipe.cpp
	g++ -std=c++11 ${DEBUG} pipe.cpp -o pipe_debug_${DB} -g
