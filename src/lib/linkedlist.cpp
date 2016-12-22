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
	if (Contains(id) && head != NULL) {
		length--;

		if (head->id == id) {
			head = head->next;
		}
		else {
			Body * pointer = head->next;
			Body * previous = head;

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
	}

	/*
	length--;
	cout << "1" << endl;

	Body * pointer = head;
	cout << "2" << endl;
	Body * previous = NULL;
	cout << "3" << endl;

	while (pointer != NULL) {
		cout << "4" << endl;
		if (pointer->id == id) {
			cout << "5" << endl;
			previous->next = pointer->next;
			cout << "6" << endl;
			pointer->next = NULL;
			cout << "7" << endl;
			break;
		}

		cout << "8" << endl;
		previous = pointer;
		cout << "9" << endl;
		pointer = pointer->next;
		cout << "0" << endl;
	}
	*/
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
