// linkedlist.cpp
// Linked List object - stores body object in simulation

#include <iostream>
#include "linkedlist.h"

using namespace std;

List::List() {
	// Constructor - set default parameter values (for empty list)
	head = NULL;
	length = 0;
	nextId = 0;
}

void List::Append(Body * node) {
	// Add a new body object (node) to the end of the list
	// This is the only function that adds a body to the list as order is irrelevant

	// Give the new node an id
	node->SetID(nextId);
	nextId++;

	// Follow pointers through list until you reach a NULL pointer
	Body * pointer = head;

	if (head == NULL) {
		head = node;
	}
	else {
		while (pointer->GetNext() != NULL) {
			pointer = pointer->GetNext();
		}

		// Point the last node to the new node
		pointer->SetNext(node);
	}

	// Update the List's length
	length++;
}

void List::Remove(int id) {
	// Remove a node (body object) from the list
	if (Contains(id) && head != NULL) {
		if (head->GetID() == id) {
			head = head->GetNext();
		}
		else {
			Body * pointer = head->GetNext();
			Body * previous = head;

			while (pointer != NULL) {
				if (pointer->GetID() == id) {
					previous->SetNext(pointer->GetNext());
					pointer->SetNext(NULL);
					break;
				}

				previous = pointer;
				pointer = pointer->GetNext();
			}
		}

		// Update the List's length
		length--;
	}
}

bool List::Contains(int id) {
	// Check if the body exists in the list
	Body * pointer = head;

	while (pointer != NULL) {
		if (pointer->GetID() == id) {
			return true;
		}

		pointer = pointer->GetNext();
	}

	// If this code is reached, id doesn't exist
	return false;
}
