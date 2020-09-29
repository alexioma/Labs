#include "stdafx.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <math.h>
#include "m.h"
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
	cout << "7. Exit" << endl;
	cout << ">";
}

int get_variant(int count)
{
	int variant;
	string s; 		// строка для считывания введённых данных
	getline(cin, s); 	// считываем строку

	// пока ввод некорректен, сообщаем об этом и просим повторить его
	while (sscanf(s.c_str(), "%d", &variant) != 1 || variant < 1 || variant > count) {
		cout << "Incorrect input. Try again: "; // выводим сообщение об ошибке
		getline(cin, s); 			   // считываем строку повторно
	}
	return variant;
}

int main() {
	int variant; 					  // выбранный пункт меню
	
	Vector<int> vec;
	Vector<int> vec_it;
	vec.push_back(111);
	vec.push_front(222);
	vec.push_back(333);
	vec.push_front(444);
	vec.push_front(555);
	cout << "Vector:" << '\n';
	for (auto const& val : vec)
		cout << val << '\n';
	Vector<int>::Iterator it = vec.begin();
	Vector<int>::Iterator end = vec.end();
	int value = *it;
	do {
		print_menu(); 				// выводим меню на экран
		variant = get_variant(7); // получаем номер выбранного пункта меню
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
		}

		if (variant != 7)
			system("pause"); 	// задерживаем выполнение, чтобы пользователь мог увидеть результат выполнения выбранного пункта
	} while (variant != 7);
	return 0;
}
