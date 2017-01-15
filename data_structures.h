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

#include <string>

//========================Linked_list========================
struct node {
	node();
	node(std::string);//string contstructor
	node *next;
	std::string word;
};

class linked_list {
	public:
	linked_list();
	virtual ~linked_list();

	bool add_node(std::string);//append a node to the list
	unsigned int get_length();
	node get_node(unsigned int);

	private:
	node *head;
	unsigned int length;
};

//========================Binary trees========================
struct leaf {
	leaf();
	leaf(std::string);

	leaf *next_greater;
	leaf *next_lesser;

	std::string word;
};

class binary_tree {
	public:
	binary_tree();
	virtual ~binary_tree();

	bool add_leaf(std::string); //return true if it is successful
	bool search_tree(std::string);//return true if it is fount

	private:
	leaf *head;
	unsigned int length;
	void delete_leaf(leaf*);
};

#include "data_structures.cpp"
