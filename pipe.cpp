#include <iostream>
#include <string>
#include <vector>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#define CHR_SEP '|'
#include <unistd.h>
#define DEBUG 'hello'
static int NUM_CHILDREN_PROCESSES = 0;

class  Proc_Pipe 
{
private:
	char ***result; // массив процессов с аргументами data[i][0][] - процесс data[i][...][0] - аргументы
	unsigned int cproc = 0; //количество процессов
	unsigned int *carg = nullptr; // количесвто аргументов каждого процесса + сам процесс
	unsigned int *length_arg = nullptr; // длина каждого аргумента
public:

void split(std::string &buffer,const char sep_arg,const char sep_proc)
{
	//добавляем в конец пробел, чтобы не было выхода за пределы буфера при разбиении
	buffer.push_back(' ');
	//char ***result = nullptr;
	//unsigned int cproc = 0; //количество процессов
	//unsigned int *carg = nullptr;// количество аргументов каждого процесса + сам процесс
	//unsigned int *length_arg = nullptr;//длина каждого аргумента
	unsigned int sum_carg = 0;//общее количество аргументов
	//подсчет количество процессов
         for(auto iter = buffer.begin(); iter != buffer.end(); iter++)
	{
		if(*iter == sep_proc) cproc++;
	}
	cproc++;
		#if DEBUG == split
		std::cout << "Количество переданных процессов: " <<  cproc << std::endl;
		#endif 
	result = new char**[cproc];
	carg = new unsigned int[cproc];
	
	for(int i = 0; i < cproc; i++)
	{
		carg[i] = 0;
	}

	//подсчет количество аргументов каждого процесса(+ сам процесс)
	int proc = 0;
	 for(auto iter = buffer.begin(); iter != buffer.end(); iter++)
	{
		if(*iter == sep_proc)
		{
			proc++;
			iter++;
			continue;
		}
		if(*iter == sep_arg) 
		{
			carg[proc]++;
			sum_carg++;
		} 
	}
	 if(*(buffer.end()-1) != sep_arg) 
	{
		carg[proc]++;
		sum_carg++;
	}
		#if DEBUG==split
			std::cout << "Количество аргументов для каждого процесса: ";
			for(int i = 0; i <cproc; i++)
			{
			 	std::cout << carg[i] << ' ';	
			}
			std::cout << std::endl;
			std::cout << "Общее количество аргументво: " << sum_carg << std::endl;
		#endif
	for(int i = 0; i < cproc; i++)
	{
		result[i] = new char*[carg[i]];
	}
	//подсчет длины каждого аргумента + нулевой символ
	length_arg = new unsigned int[sum_carg];
	for(int i = 0; i < sum_carg; i++)
	{
		length_arg[i] = 1;	
	}
	proc = 0;
	for(auto iter = buffer.begin(); iter != buffer.end(); iter++)
	{
		if(*iter == sep_proc)
		{
			iter++;
			continue;
		}
		if(*iter == sep_arg)
		{
			proc++;
			continue;
		}		
		length_arg[proc]++;
		
	}
		#if DEBUG==split
			std::cout << "Длины каждого аргумента: ";
			for(int i = 0; i < sum_carg; i++)
			{
				std::cout << length_arg[i] << ' ';	
			}
			std::cout << std::endl;
		#endif
	//выделение места под каждый аргумент
	unsigned int arguments_index = 0; 
	for(int i = 0; i < cproc; i++)
	{
		for(int arg = 0; arg < carg[i]; arg++)
		{
			#if DEBUG==split
				std::cout << "выделено памяти под аргумент: " <<
						 length_arg[arguments_index] << std::endl;
			#endif
			result[i][arg] = new char[length_arg[arguments_index]+1];
			arguments_index++;
		} 

	}
	//заполнение созданного массива данными
	auto iter = buffer.begin();
	arguments_index = 0;
	for(int i = 0; i < cproc; i++)
	{
		for (int k = 0; k < carg[i]; k++)
		{

			if(*iter == '|') iter = iter + 2; 
			for(int y = 0; y < length_arg[arguments_index]-1; y++)//length_arg[arguments_index] - 1  : длина аргумената в строке доп байт для нуль-символа
			{
				result[i][k][y]	= *iter;
				iter++;
			}	
			result[i][k][length_arg[arguments_index] - 1] = '\0';
			arguments_index++;
			iter++;
		}	
	}
}

	void print()
	{
		unsigned int argument_index = 0;
		for(int i = 0; i < cproc; i++)
		{	
			std::cout << "--------" << std::endl;
			if(result[i] != nullptr)
			for(int j = 0; j < carg[i]; j++)
			{ 	if(result[i][j] != nullptr)
				{
					if(j == 0) std::cout << "Процесс: ";
					else std::cout << "Аргумент: ";
					for(int y = 0; y < length_arg[argument_index]; y++)
					{
							
							std::cout << result[i][j][y];
						
					}	
					argument_index++;
					std::cout << std::endl;
				}
			}	
		}	
	}	

	~Proc_Pipe()
	{
		for(int i = 0; i < cproc; i++)
		{
			if(result[i] != nullptr)
			for(int j = 0; j < carg[i]; j++)
			{ 	if(result[i][j] != nullptr)			
					delete[] result[i][j];
					
			}	
		}	
		if(carg != nullptr) delete[] carg;
		if(length_arg != nullptr) delete[] length_arg;
	}


	int numproc()
	{
		return cproc;
	}

	char** get_args(int id)
	{
		if (id < 0 || id >= cproc) return nullptr;
		else
		{
			return result[id];
		}
	}
	
	
};
//обработчик сигнала смерти потомка
void sigchld_handler(int signal)
{
	pid_t pidDead;
	int resultWorking = 0;
	pidDead = waitpid(-1, &resultWorking, 0);
	if(-1 == pidDead) return;
	else NUM_CHILDREN_PROCESSES--;

	std::cout << "Потомок закнончил свою работу\n";
}

int main(int argumentc, char **argumentv)
{
	setlocale(LC_ALL, "");
	/*std::string argv;
	std::getline(std::cin, argv, '\n');
	Proc_Pipe p;
	std::cout << DEBUG << std::endl;
	p.split(argv, ' ', '|');
	// execvp(p.get_args(0)[0], p.get_args(0));
	//установка обработчика сигнала смерти потомка (SIGCHLD)
	sigset_t onemask = {0};
	sigfillset(&onemask);
 	struct sigaction signal_handler = {0};
	bzero(&signal_handler, sizeof(signal_handler));
	signal_handler.sa_handler = sigchld_handler;
	signal_handler.sa_mask = onemask;
	sigaction(SIGCHLD, &signal_handler, nullptr);
	int nump = p.numproc();*/
	//подмена дескриптора консоли на дескриптор файла
	int fd = open("file.f", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	std::cout << "создан файл\n";
	char bf[] = "Text";
	/*if(-1 == write(fd, &bf, sizeof(bf)))
	{
		std::cout << "Ошибка записи в файл \n";	
	}*/
	int err = dup2(fd, STDOUT_FILENO);
	if(err == -1)
	{
		std::cout << "Ошибка\n";
		return 0;
	}
	std::cout << "Используется std::cout\n";		
	/*for(int i = 0; i < nump; i++)
	{	//запуск процессов с аргументами
		if(!fork())
		{
			execvp(p.get_args(i)[0], p.get_args(0));
		}	
	}*/
	
	//pause();
close(fd);
	return 0;
}

