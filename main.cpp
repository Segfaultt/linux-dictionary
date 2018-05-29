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
#include <climits>
#include <cstdlib>
#include <time.h>

#define DICTIONARY "/etc/linux-dictionary/oxford.txt"

#define TOLERANCE 3

#define PRINT_HELP \
	std::cout << "A simple commandline dictionary\n\n"\
	<< "USAGE:\n"\
	<< '\t' << argv[0] << " <-h | --help>\tprint this\n" \
	<< '\t' << argv[0] << " [-d <path to dictionary>] <word>\n"\
	<< '\t' << argv[0] << " (interactive mode)\n"\
	<< '\t' << argv[0] << " --random\n"\
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

void print_similar(std::ifstream&, std::string);

//evaluate similarity
int how_similar(std::string, std::string);

void linux_dictionary(std::ifstream&, std::string);

int main(int argc, char* argv[])
{
	//check arg count
	if (argc > 4 || argc == 2 && (argv[1] == "--help" || argv[1] == "-h")) {
		PRINT_HELP
		return -1;
	}

	//check help option
	if (argc > 1 && ((std::string)argv[1] == "--help" || (std::string)argv[1] == "-h")) {
		PRINT_HELP
		return 0;
	}

	//open dictionary
	std::ifstream dictionary;
	if (argc == 4 || argc == 3)
		dictionary.open(argv[2], std::ifstream::ate);
	else
		dictionary.open(DICTIONARY, std::ifstream::ate);

	//check if open
	if (!dictionary.is_open()) {
		std::cerr << "Error: unable to open dictionary"
			  << std::endl;
		return -2;
	}

	if (argc == 2 && (std::string)argv[1] == "--random") {//random word
		srand(time(NULL));
		dictionary.seekg(0, std::ios_base::end);
		dictionary.seekg(rand()%dictionary.tellg());
		dictionary.ignore(1024, '\n');

		std::string word, line;
		std::getline(dictionary, word, ' ');
		std::getline(dictionary, line, '\n');

		std::cout << '\t' << word << std::endl
			  << line << std::endl;
	} else if (argc == 1 || argc == 3) {//interactive mode
		//print message
		std::cout << "linux-dictionary Copyright (C) 2017 Luca Pengelly\n"
			  << "This program comes with ABSOLUTELY NO WARRANTY.\n"
			  << "This is free software, and you are welcome to redistribute it\n"
			  << "under certain conditions. linux-dictionary is released under the GNU GPL.\n"
			  << "\nWelcome to linux-dictionary interactive mode\n"
			  << "type in any word you wish to see the definition of\n"
			  << "type '\\exit' to exit\n"
			  << "\n>";
		std::string entry;
		std::getline(std::cin, entry);
		while (entry != "\\exit") {
			linux_dictionary(dictionary, entry);
			std::cout << "\n>";
			std::getline(std::cin, entry);
		}
	} else {//non interactive
		std::string target(argv[argc - 1]);
		linux_dictionary(dictionary, target);
	}

	return 0;
}

void linux_dictionary(std::ifstream& dictionary, std::string target)
{
	//search for word
	dictionary.clear();
	dictionary.seekg(0, std::ifstream::beg);
	if (linear_search(dictionary, target) != 0) {
		std::cout << "Unable to find word\n"
		          << "Did you mean:\n";
		print_similar (dictionary, target);
	}
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

struct potential {
	std::string word;
	int similarity;

	potential();
};

potential::potential() {
	word = "";
	similarity = INT_MAX;
}

void print_similar(std::ifstream& dictionary, std::string target)
{
	potential list[10];//create array of top 10 potential words
	potential current;
	bool put_in;

	for (int i = 0; i < 10; i < i++)
		list[i].word = "";
	
	dictionary.clear();
	dictionary.seekg(0, dictionary.beg);
	while (dictionary.peek() != EOF) {
		put_in = false;
		dictionary.ignore(1024, '\n');
		std::getline(dictionary, current.word, ' ');
		current.similarity = how_similar(current.word, target);
		
		//put in top 10?
		for (int i = 0; i < 10 && !put_in; i++) {
			if (current.word == list[i].word)
				put_in = true;
			if (current.similarity < list[i].similarity && current.word.length() > 1) {
				list[i].word = current.word;
				list[i].similarity = current.similarity;
				put_in = true;
			}
		}
	}

	//print list
	for (int i = 0; i < 10; i++) {
		std::cout << list[i].word << std::endl;
	}
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
