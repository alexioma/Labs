#include "stdafx.h"
#include <iostream>
#include <stdexcept>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <math.h>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

template<class T>
class BST
{
private: 
	struct node					//узел
	{
		node *left, *right, *parent;   //левый и правый потомки, родитель
		int key;
		T data;          //ключ и данные
		node(int k, T d)
		{
			key = k;
			data = d;
			left = NULL;
			right = NULL;
		}
	};
	node * root;				//указатель на корень

private:
	void insert_(node ** rt, int k, T d)  //вставка
	{
		if (*rt == NULL) //дерево пустое или найдено место вставки
		{
			node *nd = new node(k, d);  //создали новый узел с ключом и данными
			*rt = nd;					//добавили его в дерево
			return;
		}
		if (k < (*rt)->key)		//если ключ меньше ключа корня, то идем влево
		{
			insert_(&((*rt)->left), k, d); //записываем слева
		}
		else if (k > (*rt)->key)            //если ключ больше ключа корня, то идем вправо
		{
			insert_(&((*rt)->right), k, d); //записываем справа
		}
		
	}
	bool search(node *root, int key, T *pdata)  //ищем (корень, ключ, данные) 
	{
		if (root == NULL)				 //если дерево пусто - false
			return false;
		if (root->key == key)				 //если ключ корня равен ключу
		{
			*pdata = root->data;		//в данные записываем данные корня
			cout << "Data: " << root->data << '\n';
			return true;
			
		}
		if (root->key > key)				//если ключ корня больше ключа
		{
		return(search(root->left, key, pdata)); //повторяем поиск по левому потомку
		}
		else
		{
		return(search(root->right, key, pdata)); //поторяем поиск по правому потомку
		}
	}

	void min(node *root)			// Минимальный элемент дерева
	{
		while (root->left != NULL)
			root = root->left;
		cout << "Min: " << root->key << " , " << root->data << '\n';
	}

	void max(node *root)			// Максимальный элемент дерева
	{
		while (root->right != NULL)
			root = root->right;
		cout << "Max: " << root->key << " , " << root->data << '\n';
	}

	void TLR(node *root)
	{
		if (root == NULL)
			return;
		cout << root->key << " data " << root->data << '\n';
		TLR(root->left);
		TLR(root->right);
	}
	void LTR(node *root)				//обход LTR
	{
		if (root != NULL){			//проверка на пустоту дерева

		LTR(root->left);	//Рекурсивная функция для левого поддерева
		cout << root->key << " data " << root->data << '\n';//Отображаем корень дерева
		LTR(root->right);	//Рекурсивная функция для правого поддерева
		}
	}
	void LRT(node *root)
	{
		if (root == NULL)
			return;

		LRT(root->left);
		LRT(root->right);
		cout << root->key << " data " << root->data << '\n';
	}

	void print_Tree(node * ptr, int level)
	{
		if (ptr)
		{
			print_Tree(ptr->right, level + 1);
			for (int i = 0; i < level; i++) cout << " ";
			cout << ptr->key << endl;
			print_Tree(ptr->left, level + 1);
		}
	}

	int countTree(node *t)
	{
		if (t == NULL)
			return 0;
		int returnValue;
		returnValue = 1 + countTree(t->left) + countTree(t->right);
		return returnValue;
	}

	node * Del(node *t, node *par_t)
	{
		if (t->left != NULL)
		{
			t->left = Del(t->left, par_t);
			return t;
		}
		par_t->key = t->key;
		par_t->data = t->data;
		node *x = t->right;
		delete t;
		return x;
	}
	node* BST_Delete(node *t, int key, bool &deleted)
	{
		bool del;
		if (t == nullptr)
		{
			deleted = false;
			return t;
		}
		if (key < t->key)	//если ключ меньше ключа корня, смотрим левую ветку
		{
			t->left = BST_Delete(t->left, key, del);
			deleted = del;
			return t;
		}
		if (key > t->key)      //если ключ больше ключа корня, смотрим правую ветку
		{
			t->right = BST_Delete(t->right, key, del);
			deleted = del;
			return t;
		}
		//если у узла нет подузлов (справа и слева)
		if (t->left == NULL && t->right == NULL)
		{
			delete t;
			return NULL;
		}
		//если есть 1 подузел справа
		if (t->left == NULL)
		{
			node *x = t->right;
			delete t;
			return x;
		}
		//если есть 1 подузел слева
		if (t->right == NULL)
		{
			node *x = t->left;
			delete t;
			return x;
		}
		t->right = Del(t->right, t);
		return t;
	}
	
public:
	BST()
	{
		root = NULL;
	}
	void insert(int k, T d)			
	{
		insert_(&root, k, d);
	}
	bool Search(int key, T *pData)	
	{
		return(search(root, key, pData));
	}
	void tlr()
	{
		TLR(root);
	}
	void ltr()
	{
		LTR(root);							
	}
	void lrt()
	{
		LRT(root);
	}
	void Print()
	{
		print_Tree(root, 0);
	}
	void Size()
	{
		int ret = countTree(root);
		cout << "Size:" << ret << endl;
	}
	void MIN()
	{
		min(root);
	}
	void MAX()
	{
		max(root);
	}
	char Delete(int key)
	{
		char ret = false;
		bool deleted = false;
		BST_Delete(root, key, deleted);
		return ret;
	}
};

void print_menu()
{
	cout << "\nWhat do you want to do?" << endl;
	cout << "1. Search. TLR." << endl;
	cout << "2. Search. LTR." << endl;
	cout << "3. Search. LRT." << endl;
	cout << "4. Print the size." << endl;
	cout << "5. Insert the node." << endl;
	cout << "6. Checking for the existence of the node. Receiving data node." << endl;
	cout << "7. Max node and min node." << endl;
	cout << "8. Delete the node." << endl;
	cout << "9. Exit" << endl;
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

	BST <int> bst;					  //создали дерево
	bst.insert(10, 2);
	bst.insert(3, 3);
	bst.insert(1, 4);
	bst.insert(5, 5);
	bst.insert(25, 6);
	bst.insert(20, 6);
	bst.insert(50, 6);
	bst.Print();					   //вывели на экран

	do {
		print_menu(); 				// выводим меню на экран
		variant = get_variant(9);                // получаем номер выбранного пункта меню
		switch (variant) {
		case 1:
			bst.tlr();
			break;

		case 2:
			bst.ltr();
			break;

		case 3:
			bst.lrt();
			break;

		case 4:
			bst.Size();
			break;

		case 5:
			int k5, d5;
			cout << "Enter key:";
			cin >> k5;
			cout << "Enter date:";
			cin >> d5;
			bst.insert(k5, d5);
			bst.Print();
			break;

		case 6:
			int p, k6;
			cout << "Enter key:";
			cin >> k6;
			bool s;
			s = bst.Search(k6, &p);
			cout << s << endl;
			break;

		case 7:
			bst.MIN();
			bst.MAX();
			break;

		case 8:
			int k8;
			cout << "Enter key:";
			cin >> k8;
			bst.Delete(k8);
			bst.Print();
			break;
		}

		if (variant != 9)
			system("pause"); 	
	} while (variant != 9);
	return 0;
}
