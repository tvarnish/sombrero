#include <iostream>
#include "linkedlist.h"

using namespace std;

List::List() {
	head = NULL;
	length = 0;
	nextId = 0;
}

void List::Append(Body * node) {
	// Update the List's length
	length++;

	// Give the new node an id
	node->id = nextId;
	nextId++;

	// Follow pointers through list until you reach a NULL pointer
	Body * pointer = head;

	if (head == NULL) {
		head = node;
	}
	else {
		while (pointer->next != NULL) {
			pointer = pointer->next;
		}

		pointer->next = node;
	}
}

void List::Remove(int id) {
	length--;

	Body * pointer = head;
	Body * previous = NULL;

	while (pointer != NULL) {
		if (pointer->id == id) {
			previous->next = pointer->next;
			pointer->next = NULL;
			break;
		}

		previous = pointer;
		pointer = pointer->next;
	}
}

Body * List::GetAt(int id) {
	Body * pointer = head;

	while (pointer != NULL) {
		if (pointer->id == id) {
			return pointer;
		}

		pointer = pointer->next;
	}

	// If this code is reached, id doesn't exist
	return NULL;
}

bool List::Contains(int id) {
	Body * pointer = head;

	while (pointer != NULL) {
		if (pointer->id == id) {
			return true;
		}

		pointer = pointer->next;
	}

	// If this code is reached, id doesn't exist
	return false;
}
