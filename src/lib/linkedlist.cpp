#include <iostream>
#include "linkedlist.h"

using namespace std;

Node::Node(int d) {
	data = d;
	next = NULL;
}

List::List() {
	head = NULL;
	length = 0;
	nextId = 0;
}

int List::GetLength() {
	return length;
}

void List::Append(Node * node) {
	// Update the List's length
	length++;

	// Give the new node an id
	node->id = nextId;
	nextId++;

	// Follow pointers through list until you reach a NULL pointer
	Node * pointer = head;

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

	Node * pointer = head;
	Node * previous = NULL;

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

void List::Display() {
	// For DEBUG purposes only
	Node * pointer = head;

	while (pointer != NULL) {
		cout << pointer->data << " [" << pointer->id << "]"<< ", ";
		pointer = pointer->next;
	}

	cout << endl;
}

Node * List::GetAt(int id) {
	Node * pointer = head;

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
	Node * pointer = head;

	while (pointer != NULL) {
		if (pointer->id == id) {
			return true;
		}

		pointer = pointer->next;
	}

	// If this code is reached, id doesn't exist
	return false;
}
