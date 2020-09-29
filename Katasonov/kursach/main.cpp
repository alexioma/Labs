#include <stdio.h>
#include <string.h>
#include <string>
#include <math.h>
#include "Sort.h"
# define NULL 0
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

void print_menu()
{
	cout << "\nWhat do you want to do?" << endl;
	cout << "1. Print the size and capacity" << endl;
	cout << "2. Insert at the beginning" << endl;
	cout << "3. Insert at the end" << endl;
	cout << "4. Get index data" << endl;
	cout << "5. Clear" << endl;
	cout << "6. Check for empty space" << endl;
	cout << "7. LSD sort. Increasing order" << endl;
	cout << "8. LSD sort. Reverse order" << endl;
	cout << "9. MSD sort. Increasing order" << endl;
	cout << "10. MSD sort. Reverse order" << endl;
	cout << "11. Exit" << endl;
	cout << ">";
}

int get_variant(int count)
{
	int variant;
	string s; // строка для считывания введённых данных
	getline(cin, s); // считываем строку

	// пока ввод некорректен, сообщаем об этом и просим повторить его
	while (sscanf(s.c_str(), "%d", &variant) != 1 || variant < 1 || variant > count) {
		cout << "Incorrect input. Try again: "; // выводим сообщение об ошибке
		getline(cin, s); // считываем строку повторно
	}
	return variant;
}

int main() {
	int variant; // выбранный пункт меню

	Vector<int> vec;
	Vector<int> vec_1[10], vec_2[10], vec_3[10];

	vec.push_back(5);
	vec.push_back(10);
	vec.push_back(8);
	vec.push_back(25);
	vec.push_back(46);
	vec.push_back(3);
	vec.push_back(9);
	vec.push_back(81);
	vec.push_back(34);
	vec.push_back(23);

	cout << "Vector:" << '\n';
	for (auto const& val : vec)
		cout << val << '\n';
	Vector<int>::Iterator it = vec.begin();
	Vector<int>::Iterator it_1 = vec.begin();
	Vector<int>::Iterator end = vec.end();
	int value = *it, value_1 = *it;
	int valued = *end;
	int t = 2;			//  t - разрядность (максимальная длина числа) 
	int i, j, d, d_1, m = 1, n=10;

	do {
		print_menu(); // выводим меню на экран

		variant = get_variant(11); // получаем номер выбранного пункта меню

		switch (variant) {
		case 1:
			cout << "size: " << vec.size() << "\ncapacity: " << vec.capacity() << "\n\n";
			break;

		case 2:
			int n;
			cout << "Enter a number:";
			cin >> n;
			vec.push_front(n);
			for (auto const& val : vec)
				cout << val << '\n';
			break;

		case 3:
			int d;
			cout << "Enter a number:";
			cin >> d;
			vec.push_back(d);
			for (auto const& val : vec)
				cout << val << '\n';
			break;

		case 4:
			int a;
			cout << "Enter index:";
			cin >> a;
			it = vec.begin();
			end = vec.end();
			value = *it;
			valued = *end;
			for (int i = 0; i < a - 1; i++){
				it++;
			}
			value = *it;
			cout << value << '\n';
			//вывод вектора на экран		
			/*for (it; it != end; it++) {
			value = *it;
			cout << value << '\n';
			}*/

			break;

		case 5:
			vec.~Vector();
			break;

		case 6:
			if (vec.begin() == vec.end()){
				cout << "Empty" << endl;
			}
			else {
				cout << "Not empty" << endl;
			}
			break;

		case 7:
			for (j = 1; j <= t; j++) {	// от 1 до 2

				for (i = 0; i <= 9; i++)	//карманы от 0 до 9
					vec_1[i].~Vector();		//заполняем нулями

				it = vec.begin();
				end = vec.end();
				for (it; it != end; it++) {	
					value = *it;			
					d = ((int)(value / m)) % (int)10;	
					vec_1[d].push_back(value);
				}
				vec.~Vector();
				for (i = 0; i <= 9; i++){
					it = vec_1[i].begin();
					end = vec_1[i].end();
					for (it; it != end; it++) {
						value = *it;
						vec.push_back(value);
					}
				}
				for (auto const& val : vec)
					cout << val << '\n';
				m *= 10;
			}
			cout << "Sort:" << '\n';
			for (auto const& val : vec)
				cout << val << '\n';

			break;

		case 8:

			for (j = 1; j <= t; j++) {	// от 1 до 2

			for (i = 0; i <= 9; i++)	//карманы от 0 до 9
			vec_1[i].~Vector();	//заполняем нулями

			it = vec.begin();
			end = vec.end();
			for (it; it != end; it++) {	//проход по всем числам вектора
			value = *it;			//получили значение
			d = ((int)(value / m)) % (int)10;
			vec_1[d].push_front(value);
			}
			vec.~Vector();
			for (i = 0; i <= 9; i++){
			it = vec_1[i].begin();
			end = vec_1[i].end();
			for (it; it != end; it++) {
				value = *it;
			vec.push_front(value);
			}
			}
			for (auto const& val : vec)
			cout << val << '\n';
			m *= 10;
			}
			cout << "Sort:" << '\n';
			for (auto const& val : vec)
			cout << val << '\n';
			
			break;

		case 9:
			it = vec.begin();
			end = vec.end();

			for (i = 0; i <= 9; i++)	//карманы от 0 до 9
				vec_1[i].~Vector();		//заполняем нулями

			for (it; it != end; it++) {	//проход по всем числам вектора
				value = *it;			//получили значение
				d = (int)(value / 10);	
				vec_1[d].push_back(value);
			}
			//вывод вектора на экран
			for (i = 0; i <= 9; i++){
				it = vec_1[i].begin();
				end = vec_1[i].end();
				for (it; it != end; it++) {
					value = *it;
					cout << value << '\n';
				}
			}

			for (i = 0; i <= 9; i++)
				vec_2[i].~Vector();

			for (i = 0; i <= 9; i++){
				if (vec_1[i].begin() != vec_1[i].end()){
					it = vec_1[i].begin();
					end = vec_1[i].end();
					for (j = 0; j <= 9; j++)
						vec_2[j].~Vector();
					for (it; it != end; it++) {	
						value = *it;			
						d = ((int)(value / 1)) % (int)10;	
						vec_2[d].push_back(value);
					}

					for (j = 0; j <= 9; j++){
						it = vec_2[j].begin();
						end = vec_2[j].end();
						for (it; it != end; it++) {
							value_1 = *it;
							vec_3[i].push_back(value_1);
						}
					}
				}
			}

			vec.~Vector();
			for (i = 0; i <= 9; i++){
				it = vec_3[i].begin();
				end = vec_3[i].end();
				for (it; it != end; it++) {
					value = *it;
					vec.push_back(value);
				}
			}
			cout << "Sort:" << '\n';
			for (auto const& val : vec)
				cout << val << '\n';

			break;

		case 10:
			it = vec.begin();
			end = vec.end();

			for (i = 0; i <= 9; i++)	//карманы от 0 до 9
				vec_1[i].~Vector();		//заполняем нулями

			for (it; it != end; it++) {	//проход по всем числам вектора
				value = *it;			//получили значение
				d = (int)(value / 10);	//отдеделяем разряды для дальнейшего сранения с каждым заходом
				vec_1[d].push_back(value);
			}
			//вывод вектора на экран
			for (i = 0; i <= 9; i++){
				it = vec_1[i].begin();
				end = vec_1[i].end();
				for (it; it != end; it++) {
					value = *it;
					cout << value << '\n';
				}
			}

			for (i = 0; i <= 9; i++)
				vec_2[i].~Vector();

			for (i = 0; i <= 9; i++){
				if (vec_1[i].begin() != vec_1[i].end()){
					it = vec_1[i].begin();
					end = vec_1[i].end();
					for (j = 0; j <= 9; j++)
						vec_2[j].~Vector();
					for (it; it != end; it++) {	
						value = *it;			
						d = ((int)(value / 1)) % (int)10;	
						vec_2[d].push_back(value);
					}

					for (j = 0; j <= 9; j++){
						it = vec_2[j].begin();
						end = vec_2[j].end();
						for (it; it != end; it++) {
							value_1 = *it;
							vec_3[i].push_back(value_1);
						}
					}
				}
			}

			vec.~Vector();
			for (i = 0; i <= 9; i++){
				it = vec_3[i].begin();
				end = vec_3[i].end();
				for (it; it != end; it++) {
					value = *it;
					vec.push_front(value);
				}
			}
			cout << "Sort:" << '\n';
			for (auto const& val : vec)
				cout << val << '\n';

			break;
		}

		if (variant != 11)
			system("pause"); 
	} while (variant != 11);

	return 0;
}
