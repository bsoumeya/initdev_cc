#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

struct stat sb;

int main(int argc, char* argv[]) {

	// Define strings
	string current_exec_name = argv[0]; // Name of the current exec program
	string first_arg;
	if(argv[1] != NULL) {
		string first_arg = argv[1];
	}
	string home_dir = "~/";
	// If project name is not given than throw an exception
	if(argv[1] == NULL) {
		cout << "Expected arguments, please check the help : " + current_exec_name + " –help" << '\n';
		return 1;
	} else if (string(argv[1]) == "-help") { // If the user invoked -help
		cout << "Name: " + current_exec_name << endl;
		cout << "Description: Create a new preconfigured project initialized with templates to get started faster.\n"
		<< "Syntax: "
		<< current_exec_name
		<< " name_of_project"
		<< " -license"
		<< " -type"
		<< " -git\n"
		<< "Arguments: \n"
		<< "name_of_project: (Any valid alpha_dash string literal is accepted)" << " the name of the directory and project to be created.\n"
		<< "-help: Shows this help menu and instructions and then exit.\n"
		<< "-type can be: \n"
		<< "-C++ or -CPP: Creates a project with preconfigurations tailored for C++ projects. \n"
		<< "-C: Creates a project with preconfigurations tailored for C projects.\n"
		<< "-Py: Creates a project with preconfigurations tailored for Python projects.\n"
		<< "-Latex: Creates a project with preconfigurations tailored for Latex documents projects.\n"
		<< "-BEAMER: Creates a project with preconfigurations tailored for Beamer projects.\n"
		"Author: Benguergoura Soumeya (bsoumeya) benguergoura.soumeya2@gmail.com" << endl;
		return 0;
} else { // A project name is given
		string sPath = argv[1];
		if (!stat(argv[1], &sb)) {
			cout << "Could not make project directory, already exists\n"
			<< "Remove that project first rm -rf "
			<< argv[1]
			<< " and retry"
			<< endl;
			return 1;
		}
		mode_t nMode = 0733; // UNIX style permissions
		int nError = 0;
		#if defined(_WIN32)
			nError = _mkdir(sPath.c_str()); // can be used on Windows
		#else 
			nError = mkdir(sPath.c_str(), nMode); // can be used on non-Windows
			if ((argv[2] == NULL) || (string(argv[2]) == "")) {
				// Create an empty project
				fstream fs;
				// Copy empty main file to the project directory
				fs.open(sPath + "/main", ios::out);
				fs.close();
				fs.open(sPath + "/.gitignore", ios::out);
				fs.close();
			} else if ((string(string(argv[2])) == "-C++") || (string(string(argv[2])) == "-CPP")) {
				// Create a C++ project
				ifstream main_code_source("../sources/main.cpp", ios::binary);
				ofstream main_code_destination(sPath + "/main.cpp", ios::binary);
				main_code_destination << main_code_source.rdbuf();
				ifstream git_ignore_source("../gitignores/cpp", ios::binary);
				ofstream git_ignore_destination(sPath + "/.gitignore", ios::binary);
				git_ignore_destination << git_ignore_source.rdbuf();
			} else if (string(argv[2]) == "-C") {
				// Create a C project
				ifstream main_code_source("../sources/main.c", ios::binary);
				ofstream main_code_destination(sPath + "/main.c", ios::binary);
				main_code_destination << main_code_source.rdbuf();
				ifstream git_ignore_source("../gitignores/c", ios::binary);
				ofstream git_ignore_destination(sPath + "/.gitignore", ios::binary);
				git_ignore_destination << git_ignore_source.rdbuf();
			} else if (string(argv[2]) == "-Py") {
				// Create a Python project
				ifstream main_code_source("../sources/main.py", ios::binary);
				ofstream main_code_destination(sPath + "/main.py", ios::binary);
				main_code_destination << main_code_source.rdbuf();
				ifstream git_ignore_source("../gitignores/python", ios::binary);
				ofstream git_ignore_destination(sPath + "/.gitignore", ios::binary);
				git_ignore_destination << git_ignore_source.rdbuf();
			} else if (string(argv[2]) == "-Latex") {
				// Create a Latex project
				ifstream main_code_source("../sources/main.tex", ios::binary);
				ofstream main_code_destination(sPath + "/main.tex", ios::binary);
				main_code_destination << main_code_source.rdbuf();
				ifstream git_ignore_source("../gitignores/tex", ios::binary);
				ofstream git_ignore_destination(sPath + "/.gitignore", ios::binary);
				git_ignore_destination << git_ignore_source.rdbuf();
			} else if (string(argv[2]) == "-BEAMER") {
				// Create a Beamer project
				ifstream main_code_source("../sources/beamer.tex", ios::binary);
				ofstream main_code_destination(sPath + "/main.tex", ios::binary);
				main_code_destination << main_code_source.rdbuf();
			} else if (string(argv[2]) == "-git") {
				// Prompt the user
				cout << "You must set project type, please check the help : initdev –help" << endl;
				return 1;
			} else {
				cout << "Uknown arguments, please check the help : initdev –help"
				<< endl;
				return 1;
			}
			
		/*
		* Licensing
		*/
		if ((argv[3] == NULL) || (string(argv[3]) == "")) {
				fstream fs;
				// Create an empty license file in the root directory
				fs.open(sPath + "/LICENSE", ios::out);
				fs.close();
			} else if (string(string(argv[3])) == "-GPL") {
				// Copy the GNU Gneral Public License v3
				ifstream license_source("../licenses/GPL", ios::binary);
				ofstream license_destination(sPath + "/LICENSE", ios::binary);
				license_destination << license_source.rdbuf();
			} else if (string(argv[3]) == "-MIT") {
				// Copy the Massachusetts Institute of Technology License
				ifstream license_source("../licenses/MIT", ios::binary);
				ofstream license_destination(sPath + "/LICENSE", ios::binary);
				license_destination << license_source.rdbuf();
			} else {
				cout << "Uknown arguments, please check the help : initdev –help"
				<< endl;
				return 1;
			}
		/*
		* Initialize git repo
		*/
		if ((argv[4] == NULL) || (string(argv[4]) == "")) {
		// Do nothing
		} else if (string(string(argv[4])) == "-git") {
			// Initialize git repo
			chdir(string(sPath).c_str());
			system("git init");
		} else {
			cout << "Uknown arguments, please check the help : initdev –help"
			<< endl;
			return 1;
		}
		// Create empty MakeFile
		ofstream makefile(sPath + "/MakeFile", ios::binary);
		#endif
		if (nError != 0) {
			cout << "Could not make project directory, make sure the name is correct" << endl;
			return 1;
		}
	}
	// Suppress warnings
	(void)argc;
	(void)argv;
	return 0;
}
