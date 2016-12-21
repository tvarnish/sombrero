#ifndef __LIST_H_INCLUDED__
#define __LIST_H_INCLUDED__

#include "body.h"

class Node {
	public:
		int data;
		int index;
		Node * next;

		Node();
};

class List {
	int length;
	Node * head;

	public:
		List();
		int GetLength();
		void Append(Node * node);
		void Display();
};

#endif
