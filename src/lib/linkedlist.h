#ifndef __LIST_H_INCLUDED__
#define __LIST_H_INCLUDED__

#include "body.h"

class List {
	int length;
	Body * head;
	int nextId;

	public:
		List();

		Body * GetHead() { return head; };
		int GetLength() { return length; };
		void Append(Body * node);
		void Remove(int id);

		Body * GetAt(int id);
		bool Contains(int id);
};

#endif
