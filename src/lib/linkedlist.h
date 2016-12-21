#ifndef __LIST_H_INCLUDED__
#define __LIST_H_INCLUDED__

#include "body.h"

class Node {
	public:
		int data;
		Node * next;

		Node();
};

class List {
	public:
		Node * head;
		int length;

		List();
		void Append(Node * node);
		void Display();
};

#endif
