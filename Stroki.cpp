#include<iostream>
#include<fstream>

using namespace std;

struct LIST		// Создание структуры для односвязного списка
{
	char* ch;		// значение
	LIST* next;		// ссылка на след. элемент списка
};

// Создаем и заполняем список значениями
LIST* create_list()
{
	char* buf_ch;			// буфер
	LIST* start = NULL, * p;		// start - указатель на начало списка
								// p - вспомогательный указатель    
	ifstream f("Group.txt");
	if (f.is_open())
	{
		if (!f.eof())		// если не конец файла идет запись в список  
		{
			start = new LIST();
			p = start;				// устанавливаем p на начало списка
			buf_ch = new char[13];
			f >> buf_ch;
			p->ch = buf_ch;
			while (!f.eof())	// пока не конец файла создаются новые элементы списка
			{
				p->next = new LIST();		// создаем новый элемент списка
				p = p->next;				// p указывает на этот новый элемент
				buf_ch = new char[13];
				f >> buf_ch;
				p->ch = buf_ch;
			}
			p->next = NULL;					// весь файл списан в список и теперь обознпчим голову списка 
		}
		else cout << "Fille empty \n";		// файл мог оказаться пустым
		f.close();
	}
	else cout << "File is no found \n";		// файл мог быть не найден
	return start; 						//Возврат указателя на голову созданного списка
}

// проверяем, соответствуют ли данные в символьном массиве целому числу
bool is_int(LIST* item)
{
	if ((item->ch[0] == '-') || ((item->ch[0] >= '0') && (item->ch[0] <= '9')))
	{
		int i = 1;
		while (item->ch[i] != '\0')		//проверяем группу 
		{
			if ((item->ch[i] >= '0') && (item->ch[i] <= '9'))
			{
				i += 1;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

// проверяем, соответствуют ли данные в символьном массиве вещественному значению
bool is_float(LIST* item)
{
	if ((item->ch[0] == '-') || ((item->ch[0] >= '0') && (item->ch[0] <= '9')))
	{
		int i = 1;
		while (item->ch[i] != '.' && item->ch[i] != ',')
		{
			if ((item->ch[i] >= '0') && (item->ch[i] <= '9'))
			{
				i += 1;
			}
			else
			{
				return false;
			}
		}
		i += 1;		// переступаем через точку / запятую
		while (item->ch[i] != '\0')
		{
			if ((item->ch[i] >= '0') && (item->ch[i] <= '9'))
			{
				i += 1;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

// обработка int
void proc_int(LIST* item)
{
	char* buf = new char[25];
	int i = 0, zn = 0, i_buf = 0;		// i - для цикла, zn - флаг, сигнализирующий о наличии знака
										// i_buf - результат перевода символьного массива в число
	if (item->ch[0] == '-')
	{
		zn = -1;
		i = 1;
	}
	while (item->ch[i] != '\0')		// переводим строку в число
	{
		i_buf = i_buf * 10 + int(item->ch[i] - '0');
		i += 1;
	}
	i_buf *= 2;
	i = 0;								// обнуляем счетчик
	if (i_buf == 0)
	{
		buf[0] = '0';
		i += 1;
	}
	else
	{
		while (i_buf != 0)					// обратный перевод числа в строку (строка перевернута)
		{
			buf[i] = char('0' + i_buf % 10);
			i_buf /= 10;
			i += 1;
		}
	}
	if (zn == -1)
	{
		buf[i] = '-';
		i += 1;
	}
	buf[i] = '\0';							// отметим, что длина итоговой строки равна i
	for (int j = 0; j < (i / 2); j++)		// приводим к нормальному виду
	{
		char ch = buf[j];
		buf[j] = buf[(i - 1) - j];
		buf[(i - 1) - j] = ch;
	}
	delete[] item->ch;
	item->ch = buf;
}

// обработка float
void proc_float(LIST* item)
{
	int i = 0, j = 0;
	if (item->ch[0] == '-') 		// если первый символ - знак '-', то пропускаем его
	{
		i = j = 1;
	}
	while (item->ch[j] == '0')		// пропускаем все начальные нули
	{
		j += 1;						// j - позиция первого ненулевого символа
	}					
	if (item->ch[j] == '.' || item->ch[j] == ',') 		// для случая, если целая часть числа целиком состоит из нулей
	{
		i += 1; 
	}
	else 
	{
		while (item->ch[j] != '.' && item->ch[j] != ',')		// если нет - идем по обычному сценарию
		{
			item->ch[i] = item->ch[j];
			i += 1;
			j += 1;
		}
	}
	item->ch[i] = '\0';				// j - это индекс символа ('.') / (','), а i - индекс конца полученной строки ('\0')
	if (item->ch[j + 1] >= '5')		// если после точки стоит символ >= '5'
	{
		char* buf = new char[13];
		int k = 0, m = 1;							// k - для прохода по символам buf, m - флаг разряда (единичка в уме)
		while (i != 0)								// идем по сценарию инта - переворачиваем, добавляем, и опять переворачиваем
		{
			if (item->ch[i - 1] == '-') 
			{
				if (m == 1) 
				{
					buf[k] = '1'; 
					k += 1;
					m = 0;
				}
				buf[k] = item->ch[i - 1];
			}
			else 
			{
				if (m == 1) 
				{
					if (item->ch[i - 1] == '9')
					{
						buf[k] = '0';
					}
					else
					{
						buf[k] = item->ch[i - 1] + 1;
						m = 0;
					}
				}
				else buf[k] = item->ch[i - 1];
			}
			i -= 1; k += 1;
		}
		if (m == 1)				// на данный момент число перевернуто
		{						// вращаем его обратно
			buf[k] = '1';		// k установлен на конец строки buf, i = 0
			k += 1; 
		}					
		while (k != 0) 
		{
			k -= 1;
			item->ch[i] = buf[k];
			i += 1;
		}
		item->ch[i] = '\0';			// число приведено к нормальному виду
		delete[] buf;
	}
}

// запись в файл
void write_to_file(LIST* list)
{
	LIST* p = list;							// вспомогательный указатель
	ofstream f("Group_out.txt");
	if (f.is_open())
	{
		while (p != NULL)
		{
			f << p->ch << ' ';
			p = p->next;
		}
		f.close();
	}
	else cout << "File is no found \n";
}

int main()
{
	LIST* ls = create_list();
	LIST* p;							// вспомогательный указатель
	if (ls != NULL)
	{
		cout << "Kurnosova Tatiana PI-192\n";
		p = ls;		// устанавливаем p на начало списка
		while (p != NULL)
		{
			if (is_int(p))
			{
				proc_int(p);
			}
			else if (is_float(p))
			{
				proc_float(p);
			}
			p = p->next;
		}
		write_to_file(ls);
	}
}