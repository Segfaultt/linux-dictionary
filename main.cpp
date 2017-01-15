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
#define NDEBUG

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "data_structures.h"

#define DICTIONARY "oxford.txt"

#define TOLERANCE 3

#define PRINT_HELP \
	std::cout << "A simple commandline dictionary\n\n"\
	<< "USAGE:\n"\
	<< '\t' << argv[0] << " [-s] [-d <path to dictionary>] <word>\n"\
	<< "EXAMPLES:\n"\
	<< '\t' << argv[0] << " Zephyr\n"\
	<< '\t' << argv[0] << " -d ~/Downloads/dictionary.txt Yarmulke\n";

//case insensitive equality check
//returns 0 if a = b
//returns 1 if a > b
//returns -1 if a < b
int compare(std::string, std::string);

//find word in dictionary
int linear_search(std::ifstream&, std::string);

//did you mean ____?
bool unique_entry(std::string word, linked_list& word_list);
void guess(std::ifstream&, std::string, linked_list&);

//is the word not already in word_list

//evaluate similarity
int how_similar(std::string, std::string);

int main(int argc, char* argv[])
{
	//check arg count
	if (!(argc == 2 || argc == 4)) {
		PRINT_HELP
		return -1;
	}

	//open dictionary
	std::ifstream dictionary;
	if (argc == 4)
		dictionary.open(argv[2], std::ifstream::ate);
	else
		dictionary.open(DICTIONARY, std::ifstream::ate);

	//check if open
	if (!dictionary.is_open()) {
		std::cerr << "Error: unable to open dictionary \""
			  << std::endl;
		return -2;
	}

	//search for word
	std::string target(argv[argc - 1]);
	if (linear_search(dictionary, target) != 0) {
		std::cout << "Unable to find word, did you mean one of the following?\n";
		linked_list potentials;
		guess(dictionary, target, potentials);
		for (int i = 1; i <= potentials.get_length(); i++) {
			std::cout << potentials.get_node(i).word << std::endl;
		}
		return -3;
	}

	return 0;
}

//find word in dictionary
int linear_search(std::ifstream& dictionary, std::string target)
{
	std::string word;
	dictionary.seekg(0, std::ifstream::beg);
	while (compare(word, target) != 0) {
		dictionary.ignore(1024, '\n');
		std::getline(dictionary, word, ' ');
		if (dictionary.eof())
			return -1;
	}

	std::string definition;
	std::getline(dictionary, definition);
	std::cout << '\t' << word << std::endl;
	std::cout << definition << std::endl;

	return 0;
}

//case insensitive equality check
//returns 0 if a = b
//returns 1 if a > b
//returns -1 if a < b
int compare(std::string a, std::string b)
{
	int longest_length;

	if (a.length() < b.length())
		longest_length = b.length();
	else
		longest_length = a.length();
	
	for (int i = 0; i < longest_length; i++) {
		if (a[i] > 94)
			a[i] -= 32;
		if (b[i] > 94)
			b[i] -= 32;
		if (a[i] > b[i]) {
			return 1;
		}
		if (b[i] > a[i]) {
			return -1;
		}
	}

	return 0;
}

//did you mean ____?
void guess(std::ifstream& dictionary, std::string entry, linked_list& potentials)
{
	std::string word;

	dictionary.clear();
	dictionary.seekg(0);
	while (dictionary.peek() != EOF) {
		std::getline(dictionary, word, ' ');
		dictionary.ignore(1024, '\n');
		if (how_similar(word, entry) < TOLERANCE && unique_entry(word, potentials))
			potentials.add_node(word);
	}
}

bool unique_entry(std::string word, linked_list& word_list)
{
	for (int i = 1; i <= word_list.get_length(); i++) {
		if (word_list.get_node(i).word == word)
			return false;
	}
	return true;
}
//evaluate similarity
int how_similar(std::string a, std::string b)
{
	int similarity = 0, longest_length;

	similarity += abs(a.length() - b.length());

	if (a.length() < b.length())
		longest_length = b.length();
	else
		longest_length = a.length();
	
	for (int i = 0; i < longest_length; i++) {
		if (a[i] > 94)
			a[i] -= 32;
		if (b[i] > 94)
			b[i] -= 32;
		if (a[i] != b[i])
			similarity += (longest_length-i);
	}

	return similarity;
}
