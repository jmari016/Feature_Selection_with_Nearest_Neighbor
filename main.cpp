#include "domain.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <thread>
using namespace std;

int main() {
	string filename;
	string fileInput;
	double curNum;
	int userInput = -1;
	Domain* domain;
	vector<double> features; //class label (at index 0) and features of a single instance
	vector<Instance*> instances; //contains all instances from txt file
	ifstream inFile;
	
	cout << "Welcome to Jorge Marin's Feature Selection Algorithm." << endl;
	restart:
	cout << "Type in the name of the file to test: ";
	cin >> filename;
	cout << endl;
	inFile.open(filename);
	if (inFile.is_open()) {
		while (!inFile.eof()) {
			getline(inFile, fileInput); //gets the info from the txt file one line at a time
			if (fileInput == "")
				break;
			istringstream is(fileInput); //treats the string from getline as a stream
			while (is >> curNum) //all of the nums will be stored in features (class label is index 0)
				features.push_back(curNum);
			instances.push_back(new Instance(features)); //store the instance info into vector
			features.clear(); //empty contents to fill with new features
		}
		inFile.close();
		domain = new Domain(instances);
	}
	else {
		cout << "Unable to open file " << filename << endl;
		goto restart;
	}
	while (userInput < 1 || userInput > 2) {
		cout << endl << "Type the number of the algorithm you want to run:" << endl
			<< "\t1. Forward Selection" << endl
			<< "\t2. Backward Elimination" << endl;
		cin >> userInput;
		if (userInput < 1 || userInput > 2)
			cout << "Invalid input." << endl;
	}
	cout << endl;
	domain->startSearch(userInput);
	return 0;
}
