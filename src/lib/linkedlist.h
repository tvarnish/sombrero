// linkedlist.h
// Linked List object - stores body object in simulation

#ifndef __LIST_H_INCLUDED__
#define __LIST_H_INCLUDED__

#include "body.h"

class List {
	int length;
	Body * head;
	int next_id;

	public:
		List();

		Body * GetHead() { return head; };
		int GetLength() { return length; };
		void Append(Body * node);
		void Remove(int id);
		bool Contains(int id);
};

#endif
