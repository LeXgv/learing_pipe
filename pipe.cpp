#include <iostream>
#include <string>
#include <vector>
#define CHR_SEP '|'

class  Proc_Pipe 
{
private:
	char ***result; // массив процессов с аргументами data[i][0][] - процесс data[i][...][0] - аргументы
	unsigned int cproc = 0; //количество процессов
	unsigned int *carg = nullptr; // количесвто аргументов каждого процесса + сам процесс
	unsigned int *length_arg = nullptr; // длина каждого аргумента
public:

void split(const std::string &buffer,const char sep_arg,const char sep_proc)
{
	//char ***result = nullptr;
	//unsigned int cproc = 0; //количество процессов
	//unsigned int *carg = nullptr;// количество аргументов каждого процесса + сам процесс
	//unsigned int *length_arg = nullptr;//длина каждого аргумента
	unsigned int sum_carg = 0;//общее количество аргументов
	//подсчет количество процессов
         for(auto iter = buffer.begin(); iter != buffer.end(); iter++)
	{
		if(*iter == '|') cproc++;
	}
	cproc++;
		#if DEBUG==split
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
		if(*iter == '|')
		{
			proc++;
			iter++;
			continue;
		}
		if(*iter == ' ') 
		{
			carg[proc]++;
			sum_carg++;
		} 
	}
	 if(*(buffer.end()-1) != ' ') 
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
		result[i] = new char*[carg[i]+1];
		result[carg[i]] = nullptr;
	}

	//подсчет длины каждого аргумента
	length_arg = new unsigned int[sum_carg];
	for(int i = 0; i < sum_carg; i++)
	{
		length_arg[i] = 1;	
	}
	proc = 0;
	for(auto iter = buffer.begin(); iter != buffer.end(); iter++)
	{
		if(*iter == '|')
		{
			iter++;
			continue;
		}
		if(*iter == ' ')
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
			result[i][arg] = new char[length_arg[arguments_index]];
			arguments_index++;
		} 

	}
	//заполнение созданного массива данными
	auto iter = buffer.begin();
	for(int i = 0; i < cproc; i++)
	{
		for(int j = 0; j < carg[i]; j++)
		{
			if(*iter == '|') iter = iter + 2; 
			for(int y = 0; y < length_arg[j]-1; y++)
			{
				result[i][j][y]	= *iter;
				iter++;
			}
			result[i][j][length_arg[j]-1] = '\0';	
			iter++;
		}	
	}
}

	void print()
	{
		for(int i = 0; i < cproc; i++)
		{	
			std::cout << "--------" << std::endl;
			if(result[i] != nullptr)
			for(int j = 0; j < carg[i]; j++)
			{ 	if(result[i][j] != nullptr)
				{
					if(j == 0) std::cout << "Процесс: ";
					else std::cout << "Аргумент: ";
					for(int y = 0; y < length_arg[j]; y++)
					{
							
							std::cout << result[i][j][y];
						
					}	
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

	 char* get_proc(int id)
	{
		if(id < 0 || id >= cproc) return nullptr;
		else
		{
			return result[id][0];
		}	
	}

};
int main(int argumentc, char **argumentv)
{
	setlocale(LC_ALL, "");
	std::string argv;
	#ifdef DEBUG
	#warning "DEBUG_MODE"

	int argc = 1;
	#else
	int argc = 0;
	#endif
	 std::getline(std::cin, argv, '\n');
	Proc_Pipe p;
	p.split(argv, ' ', '|'); 	
	for(int i = 0; i < p.numproc(); i++)
	{
		std::cout << p.get_proc(i) << std::endl;	
	} 
	return 0;
}
