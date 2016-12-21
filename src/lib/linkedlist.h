#ifndef __LIST_H_INCLUDED__
#define __LIST_H_INCLUDED__

#include "body.h"

class Node {
	public:
		int data;
		int id;
		Node * next;

		Node(int d);
};

class List {
	int length;
	Node * head;
	int nextId;

	public:
		List();
		int GetLength();
		void Append(Node * node);
		void Remove(int id);
		void Display();

		Node * GetAt(int id);
		bool Contains(int id);
};

#endif
