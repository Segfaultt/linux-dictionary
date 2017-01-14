#include <iostream>
#include <fstream>
#include <string>

#define DICTIONARY "Oxford English Dictionary.txt"

#define PRINT_HELP \
	std::cout << "A simple commandline dictionary\n\n"\
	<< "USAGE:\n"\
	<< '\t' << argv[0] << " [-d <path to dictionary>] <word>\n"\
	<< "EXAMPLES:\n"\
	<< '\t' << argv[0] << " Zephyr\n"\
	<< '\t' << argv[0] << " -d ~/Downloads/dictionary.txt Yarmulke\n";

int main(int argc, char* argv[])
{
	//check arg count
	if (!(argc == 2 || argc == 4)) {
		PRINT_HELP
		return -1;
	}

	//open dictionary
	std::ifstream dictionary;
	char* dictionary_file_location = "Oxford English Dictionary.txt";
	if (argc == 4)
		dictionary_file_location = argv[2];
	#ifndef NDEBUG
		std::cout << "opening dictionary "
		          << dictionary_file_location
			  << std::endl;
	#endif
	dictionary.open(dictionary_file_location, std::ifstream::ate);
	dictionary.seekg(dictionary.tellg() / 2); //go to middle of file
	
	if (!dictionary.is_open()) {
		std::cerr << "ERROR: unable to open dictionary \""
		          << dictionary_file_location
			  << std::endl;
		return -1;
	}
	#ifndef NDEBUG
		std::cout << "opened dictionary. dictionary.tellg() = "
		          << dictionary.tellg()
			  << std::endl;
	#endif

	return 0;
}
