#ifndef DOMAIN_H
#define DOMAIN_H
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <iterator>
#include <numeric>
#include <math.h>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct Instance { //when opening file, info is stored as an object of Instance 
	Instance(vector<double> features): features(features) {}
	vector<double> features;
};
struct MostAccurateInstance { //used to store the final answer of the search
	MostAccurateInstance(set<int> features, double percentage) : features{features}, percentage{percentage} {}
	set<int> features;
	double percentage;
	string printInstance();
};
class Domain {
public:
	Domain();
	Domain(vector<Instance*> instances);
	void startSearch(int choice);
private:
	vector<Instance*> instances; //where all of the instances are stored
	int numOfFeatures; //total number of features
	void normalizeData(); //normalizes all of the instances' features
	set<int> featureIDs; //Feature set used in the search
	/*Computes the nearest neighbor of a "new" instance and returns
	the classLabel of the nearest neighbor. InstanceID is the index
	of the instance being used to classify (i.e. not being included
	in the training data)*/
	double nearestNeighbor(int instanceID);
	double leaveOneOutValidator();
	void forwardSelection();
	void backwardElimination(double accuracyAllFeatures); //pass in accuracy of the full feature set for beginning of search
	string printFeatureIDs(set<int> features); //comma seperated features
	void printTimeDuration(int seconds); //used to print the duration of the search algorithm
};
#endif // !DOMAIN_H
