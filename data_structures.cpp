/*
Copyright (C) 2017  Luca Pengelly

This file is part of linux-dictionary.

linux-dictionary is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

linux-dictionary is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with linux-dictionary.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <climits>
//#include "data_structures.h"

//========================Linked_list========================
linked_list::linked_list() {
	head = NULL;
	length = 0;
}

//append a node to the list
bool linked_list::add_node(std::string new_word) {
	if (length >= UINT_MAX)
		return false;
	
	node* new_node = new node(new_word);//create new node

	if (head) {
		node  *current_node;
		node *next_node;
		current_node = head;
		next_node = current_node->next;

		while (next_node) {//traverse until the next pointer is null (end of list)
			current_node = next_node;
			next_node = current_node->next;
		}
		#ifndef NDEBUG
			std::cout<<"node \""<<current_node->word<<"\" set to point to node \""<<new_node->word<<'\"'<<std::endl;
		#endif
		current_node->next = new_node;//set the next pointer to the new node
	} 
	else {
		#ifndef NDEBUG
			std::cout<<"head set to point to node \""<<new_node->word<<'\"'<<std::endl;
		#endif
		head = new_node;
	}
	
	length++;

	return true;
}

unsigned int linked_list::get_length() {
	return length;
}

node linked_list::get_node(unsigned int count) { 
	if (count > length || count < 1) {//check if in domain
		std::cerr<<"error: trying to get a node that doesn't exist\n";
		return *head;
	}

	node *current_node = head, *next_node = head;
	for (int i = 0; i < count; i++) {
		current_node = next_node;
		next_node = current_node->next;
	}

	return *current_node;
}

//transverse through nodes while deleting them
linked_list::~linked_list() {
	node *current_node;
	node *next_node;
	next_node = head;
	while (next_node) {
		current_node = next_node;
		next_node = current_node->next;
		if (current_node) {
			#ifndef NDEBUG
				std::cout<<"deleting node \""<<current_node->word<<'\"'<<std::endl;
			#endif

			delete current_node;
		}
	}
}

node::node() {
	#ifndef NDEBUG
		std::cout<<"construcing a default node\n";
	#endif
	word = "[EMPTY]";
	next = NULL;
}

node::node(std::string a) {
	#ifndef NDEBUG
		std::cout<<"construcing node \""<<a<<"\"\n";
	#endif

	word = a;
	next = NULL;
}

//========================Binary trees========================
leaf::leaf() {
	#ifndef NDEBUG
		std::cout<<"constructing a default leaf\n";
	#endif
	next_greater = NULL;
	next_lesser = NULL;
	word = "[EMPTY]";
}

leaf::leaf(std::string input_word) {
	#ifndef NDEBUG
		std::cout<<"constructing leaf \""<<input_word<<"\"\n";
	#endif
	next_greater = NULL;
	next_lesser = NULL;
	word = input_word;
}

binary_tree::binary_tree() {
	head = NULL;
	length = 0;
}

binary_tree::~binary_tree() {
	delete_leaf(head);
}

bool binary_tree::add_leaf(std::string new_word) {
	if (length >= UINT_MAX)
		return false;

	leaf *new_leaf = new leaf(new_word);

	if (head) {//if there are any leaves
		leaf *current_leaf, *next_leaf;
		next_leaf = head;

		//traverse until an empty place or duplicate is found
		do {
			current_leaf = next_leaf;
			if (new_word.compare(current_leaf->word) > 0) 
				next_leaf = current_leaf->next_greater;
			else if (new_word.compare(current_leaf->word) < 0)
				next_leaf = current_leaf->next_lesser;
			else {
				#ifndef NDEBUG
					std::cout<<current_leaf->word<<" already exists in the tree"<<std::endl;
				#endif
				delete new_leaf;//delete the now needless leaf
				return true;
			}
		} while (next_leaf != NULL);

		//set the lesser or greater pointer to the new leaf
		if (new_word.compare(current_leaf->word) > 0) {
			current_leaf->next_greater = new_leaf;
		#ifndef NDEBUG
			std::cout<<current_leaf->word<<" greater pointer set to leaf \""<<new_leaf->word<<'\"'<<std::endl;
		#endif
		} else {
			current_leaf->next_lesser = new_leaf;
		#ifndef NDEBUG
			std::cout<<current_leaf->word<<" lesser pointer set to leaf \""<<new_leaf->word<<'\"'<<std::endl;
		#endif
		}
	} else {
		#ifndef NDEBUG
			std::cout<<"head set to point to leaf \""<<new_leaf->word<<'\"'<<std::endl;
		#endif
		head = new_leaf;
	}

	length++;
	return true;
}

bool binary_tree::search_tree(std::string target_word) {
	if (length >= UINT_MAX)
		return false;

	leaf *new_leaf = new leaf(target_word);

	if (head) {//if there are any leaves
		leaf *current_leaf, *next_leaf;
		next_leaf = head;

		//traverse until an empty place or duplicate is found
		do {
			current_leaf = next_leaf;
			if (target_word.compare(current_leaf->word) > 0) 
				next_leaf = current_leaf->next_greater;
			else if (target_word.compare(current_leaf->word) < 0)
				next_leaf = current_leaf->next_lesser;
			else {
				#ifndef NDEBUG
					std::cout<<current_leaf->word<<" exists in the tree"<<std::endl;
				#endif
				return true;
			}
		} while (next_leaf != NULL);

	} else {
		#ifndef NDEBUG
			std::cout<<"tree is empty\n";
		#endif
	}

	return false;
}

void binary_tree::delete_leaf(leaf *leaf) {
	if (leaf) {
		delete_leaf(leaf->next_greater);
		delete_leaf(leaf->next_lesser);
		delete leaf;
	}
}
