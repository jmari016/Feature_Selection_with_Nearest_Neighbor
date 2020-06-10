#include "domain.h"

string MostAccurateInstance::printInstance() {
	/*returns string of comma seperated features*/
	string str = "";
	for (auto it = this->features.begin(); it != this->features.end(); ++it) {
		str += to_string(*it);
		if (it != --this->features.end())
			str += ",";
	}
	return str;
}

Domain::Domain() {
	this->numOfFeatures = 0;
}

Domain::Domain(vector<Instance*> instances) {
	this->instances = instances; //copy vector of Instances from main
	this->numOfFeatures = this->instances.at(0)->features.size() - 1; //-1 because first item in vector of instances is the class label
	cout << "This dataset has " << this->numOfFeatures
		<< " features (not including the class attribute), with "
		<< this->instances.size() << " instances." << endl;
	cout << endl << "Please wait while I normalize the data....";
	this->normalizeData();
	cout << "Done!" << endl;
}

void Domain::normalizeData() {
	//Uses z-score to normalize data
	vector<double> columnFeature; //stores all of the features of a single column
	double mean, sqSum, stdDev; 
	for (unsigned int i = 1; i <= this->numOfFeatures; ++i) { //for all the features
		for (unsigned int j = 0; j < this->instances.size(); ++j) //push back the column of features in the vector
			columnFeature.push_back(this->instances.at(j)->features.at(i));
		mean = accumulate(columnFeature.begin(), columnFeature.end(), 0.0) / (columnFeature.size() * 1.0); //calculates mean
		sqSum = inner_product(columnFeature.begin(), columnFeature.end(), columnFeature.begin(), 0.0);
		stdDev = sqrt((sqSum / columnFeature.size()) - (mean * mean)); //calculates stdDev
		columnFeature.clear(); //clear the vector to be used again for the next column of features
		for (unsigned int j = 0; j < this->instances.size(); ++j) //for all instances again
			this->instances.at(j)->features.at(i) = (this->instances.at(j)->features.at(i) - mean) / stdDev; //update the column we just worked with new normalized data
	}
}

double Domain::nearestNeighbor(int instanceID) {
	if (this->featureIDs.size() == 0)
		return -1;
	/*feature1 is the features of the left out instance (at index instanceID)*/
	double sum, feature1, feature2;
	double nearestNeighborClass = 0; //saves the class label of the nearest neighbor
	/*will be used to save and keep track of the shortest distance to another neighbor
	initialized to DBL_MAX for first instance*/
	double nearestNeighborDistance = DBL_MAX; 
	for (unsigned int i = 0; i < this->instances.size(); ++i) { //for every instance in the domain
		if (i != instanceID) { //compute each distance between left out and training instances
			sum = 0; //initialize to 0 every time we calculate a new distance
			for (auto it = this->featureIDs.begin(); it != this->featureIDs.end(); ++it) { //accesses the featureIDs in the feature set to find nearest neighbor (featureIDs are essentially indexs)
				feature1 = this->instances.at(instanceID)->features.at(*it);
				feature2 = this->instances.at(i)->features.at(*it);
				sum += pow(feature2 - feature1, 2.0); //gather all the sums of (feature1 - feature2)^2
			}
			if (sum < nearestNeighborDistance) { //if the calculated distance is smaller than our saved distance
				nearestNeighborClass = this->instances.at(i)->features.at(0); //save the class label of that nearest neighbor
				nearestNeighborDistance = sum; //update our saved distance to the new shorter distance
			}
		}
	}
	/*at this point, we iterated through all the instances and found the nearest neighbor*/
	return nearestNeighborClass; //return the class label of that nearest neighbor 
}

double Domain::leaveOneOutValidator() {
	double correctCounter = 0;
	for (unsigned int i = 0; i < this->instances.size(); ++i) { //for all the instances
		/*compare returned value from validator with the actual class label
		to see if it predicted the label correctly*/
		if (this->nearestNeighbor(i) == this->instances.at(i)->features.at(0)) //class label of an instance is at index 0
			++correctCounter; //if it did, increment the counter of correct predictions
	}
	return ((correctCounter * 1.0) / (instances.size() * 1.0)) * 100.0; //returns the percentage
}

void Domain::startSearch(int choice) {
	/*Fill in the feature set with all features for initial
	"Running nearest neighbor with all features"*/
	for (unsigned int i = 1; i <= this->numOfFeatures; ++i) 
		this->featureIDs.insert(i);
	double accuracyAllFeatures;
	cout << "Running nearest neighbor with all " << this->numOfFeatures
		<< " features, using \"leaving-one-out\" evaluation, I get an accuracy of ";
	accuracyAllFeatures = this->leaveOneOutValidator();
	cout << accuracyAllFeatures << "%" << endl;
	cout << endl << "Beginning search." << endl << endl;
	/*Borrowed below code from 
	https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
	*/
	high_resolution_clock::time_point start, stop; //declare variables for keeping track of time
	if (choice == 1) {
		this->featureIDs.clear();
		start = high_resolution_clock::now();
		this->forwardSelection();
		stop = high_resolution_clock::now();
	}
	else {
		start = high_resolution_clock::now();
		this->backwardElimination(accuracyAllFeatures);
		stop = high_resolution_clock::now();
	}
	auto duration = duration_cast<seconds>(stop - start); //total time is time stamp after the search - time stamp before the search
	this->printTimeDuration(duration.count()); //function that takes in seconds and displays time in HH:MM:SS
}

void Domain::forwardSelection() {
	pair<set<int>::iterator, bool> insertReturnVal; //used to store the result of inserting a feature into the feature set
	pair<int, double> highestAccurateFeature; //stores the highest accuracy feature from the different feature subsets (int is the featureID and double is the percentage)
	MostAccurateInstance* finalAnswer = new MostAccurateInstance(this->featureIDs, 0.0); //stores the most accurate feature subset which will be the final answer when finished 
	double featureAccuracyPercentage; //stores the result of the validator on a subset of features
	bool beganDecrease = false; //used to see if the accuracy begins to drop. Used for formatting of the trace and updating mostAccurateInstance
	while (this->featureIDs.size() < this->numOfFeatures && finalAnswer->percentage != 100) { //loop runs until no more features can be added
		/*For each iteration of the search, we start with an empty set and continue
		adding features from 1 to however many features there are.*/
		highestAccurateFeature.first = 0; //initializes highestAccurateFeature contents to 0 for first iteration of search
		highestAccurateFeature.second = 0.0;
		for (unsigned int i = 1; i <= this->numOfFeatures; ++i) { //for all features
			insertReturnVal = this->featureIDs.insert(i); //attempt to add a feature into the subset of features
			if (insertReturnVal.second) {//if the inserted number was not already in the feature set
				cout << "\tUsing feature(s) {" << this->printFeatureIDs(this->featureIDs) << "} accuracy is ";
				featureAccuracyPercentage = this->leaveOneOutValidator(); //save accuracy of this subset of features
				cout << setprecision(3);
				cout << featureAccuracyPercentage << "%" << endl;
				/*Since we're adding features into the actual feature set one by one, 
				we delete them after because they might not be the most accurate feature addition*/
				this->featureIDs.erase(insertReturnVal.first); 
				/*If the added feature resulted in an accuracy percentage higher than 
				the one saved in highestAccurateFeature, then we update highestAccurateFeature
				to now save the new highest accurate feature addition along with the featureID*/
				if (featureAccuracyPercentage > highestAccurateFeature.second) { 
					highestAccurateFeature.first = i;
					highestAccurateFeature.second = featureAccuracyPercentage;
				}
			}
		}
		cout << endl;
		/*Once we've itterated and added all of the features into the existing feature subset,
		the feature that resulted in the highest accuracy will be saved in highestAccurateFeature.
		This is the point where we actually add it into our subset saved in the Domain class to iterate
		through the while loop again, now with the additional feature*/
		this->featureIDs.insert(highestAccurateFeature.first);
		/*If the resulting highest precentage calculated through this iteration of the loop is greater
		than the percentage saved in finalAnswer*/
		if (highestAccurateFeature.second > finalAnswer->percentage) {
			if (beganDecrease) { //Check to see if the percentages of the search began to drop
				cout << "New highest accuracy!" << endl; //then at this point, a new highest feature was found
				beganDecrease = false; //reset to false in case it begins to decrease again
			}
			finalAnswer->features = this->featureIDs; //update the feature set in finalAnswer
			finalAnswer->percentage = highestAccurateFeature.second; //update the percentage of finalAnswer
		}
		/*At this point, the highest precentage calculated through this iteration of the loop is less
		than the percentage saved in finalAnswer*/
		else if (beganDecrease) //goes in else-if if the search has began decreasing before
			break; //If it has began decreasing before, then break the loop and show final answer
		else /*if (!beganDecrease) */{ //goes in the else statement if it's the first time decreasing
			cout << "(Warning, accuracy has decreased! Continuing search in case of local maxima)" << endl;
			beganDecrease = true; //change to true
		}
		cout << setprecision(3);
		cout << "Feature set {" << this->printFeatureIDs(this->featureIDs) << "} was best, accuracy is " << highestAccurateFeature.second << "%" << endl << endl;
	}
	/*Once we finish the while loop and can't add anymore features (or if the loop was broken out of), 
	output and display the solution saved in finalAnswer.*/
	cout << setprecision(3);
	cout << "Finished search!! The best feature subset is {" << finalAnswer->printInstance()
		<< "} which has an accuracy of " << finalAnswer->percentage << "%" << endl;
}

void Domain::backwardElimination(double accuracyAllFeatures) {
	/*Eveything is almost exactly the same between forward selection
	and backward elimination. Only difference is that instead of starting on an empty set
	and adding features, we start on a full set and remove features*/
	MostAccurateInstance* finalAnswer = new MostAccurateInstance(this->featureIDs, accuracyAllFeatures);
	pair<int, double> highestAccurateFeature;
	int iteratorIndex; //used for iterator to return back to the correct index since we delete an item from the set
	double featureAccuracyPercentage;
	int savedFeatureID; //feature we temporarily remove every iteration is saved to be inserted again
	bool beganDecrease = false; 
	while (this->featureIDs.size() != 1 && finalAnswer->percentage != 100) {
		highestAccurateFeature.first = 0;
		highestAccurateFeature.second = 0.0;
		iteratorIndex = 0; //initialized to 0 since we start at the beginning of the feature set
		for (auto it = this->featureIDs.begin(); it != this->featureIDs.end(); ++it) {
			savedFeatureID = *it; //save the featureID
			this->featureIDs.erase(it); //remove the ID to calculate the accuracy
			cout << "\tUsing feature(s) {" << this->printFeatureIDs(this->featureIDs) << "} accuracy is ";
			featureAccuracyPercentage = this->leaveOneOutValidator(); 
			cout << setprecision(3);
			cout << featureAccuracyPercentage << "%" << endl;
			this->featureIDs.insert(savedFeatureID);
			if (featureAccuracyPercentage > highestAccurateFeature.second) {
				highestAccurateFeature.first = savedFeatureID;
				highestAccurateFeature.second = featureAccuracyPercentage;
			}
			it = this->featureIDs.begin(); //since we deleted the feature at the iterator, we reset it back to the beggining
			advance(it, iteratorIndex); //and move it back to where the feature was to continue the search
			++iteratorIndex; //increase by one for next item in featureSet
		}
		cout << endl;
		this->featureIDs.erase(highestAccurateFeature.first); //once we find the highest accuracy percentage, delete it from the set
		if (highestAccurateFeature.second > finalAnswer->percentage) {
			if (beganDecrease) {
				cout << "New highest accuracy!" << endl;
				beganDecrease = false;
			}
			finalAnswer->features = this->featureIDs;
			finalAnswer->percentage = highestAccurateFeature.second;
		}
		else if (beganDecrease)
			break;
		else /*if (!beganDecrease)*/ {
			cout << "(Warning, accuracy has decreased! Continuing search in case of local maxima)" << endl;
			beganDecrease = true;
		}
		cout << setprecision(3);
		cout << "Feature set {" << this->printFeatureIDs(this->featureIDs) << "} was best, accuracy is " << highestAccurateFeature.second << "%" << endl << endl;
	}
	cout << setprecision(3);
	cout << "Finished search!! The best feature subset is {" << finalAnswer->printInstance()
		<< "} which has an accuracy of " << finalAnswer->percentage << "%" << endl;
}

string Domain::printFeatureIDs(set<int> features) {
	/*returns string of comma seperated features*/
	string str = "";
	for (auto it = features.begin(); it != features.end(); ++it) {
		str += to_string(*it);
		if (it != --features.end())
			str += ",";
	}
	return str;
}

void Domain::printTimeDuration(int seconds) {
	/*Borrowed code from 
	http://www.programmingnotes.org/?p=2062 */
	// declare variables
	int hour = 0;
	int min = 0;
	int sec = 0;

	// using the time from ^ above, convert 
	// secs to HH:MM:SS format using division
	// and modulus
	hour = seconds / 3600;
	seconds = seconds % 3600;
	min = seconds / 60;
	seconds = seconds % 60;
	sec = seconds;

	// display data to user
	cout << "The search algorithm took: " << hour << " hours, "
	<< min << " minutes, and " << sec << " seconds!" << endl;
}
