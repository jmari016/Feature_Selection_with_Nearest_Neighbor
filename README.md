# Feature Selection with Nearest Neighbor
The program uses the nearest neighbor classifier inside a "wrapper" which searches for the best subset of features.

University of California, Riverside  
CS170: Introduction to Artificial Intelligence  
Professor Niloofar Montazeri

### *Introduction*
The goal for Project #2 for CS170 was to utilize the nearest neighbor classifier to search 
through a dataset and find the best combination of features for classification. We were 
given 4 different datasets to work with: A small and large dataset given to the entire 
class and a small and large dataset that was unique for every student. In the assignment, 
we had to implement the nearest neighbor classifier, the leave-one-out validator, and the two 
search algorithms: forward selection and backward elimination. The project was coded in C++ 
and was organized using object-oriented programming. The class Domain would be the object 
initialized and used in main which holds the nearest neighbor algorithm, validator, and the two 
searches. The Domain would consist of a vector of pointers of type Instance, which is a struct 
that holds all the features and class label of a single instance. To make code slightly simpler, I 
also implemented a MostAccurateInstance struct which would hold all the necessary information 
for the final answer displayed after the search. One of the main challenges I faced during this 
project was runtime of the code. I tried optimizing my code as much as I was personally able to 
come up with but for large datasets, my search algorithm would take around 3+ hours. One of 
these optimizations that I utilized was to contain the feature subsets contained in a set to 
eliminate the need to manually search through the container if the feature being added already 
exists. Although the set still does this check, it is completed much faster as a set is implemented 
using a binary search tree. However, a tradeoff is that its insertion is not as fast as a vector 
(push_back).

Another important thing to point out is that I had two different cases for when the search 
algorithm ends: One where the search continues until no more features can be added/removed 
from the set and the other where it ends after the first initial decrease of accuracy. For the second 
type of search stop, the search would conduct one more level of the search to see if there is a 
local maxima. If a local maxima is found, then the highest accuracy would be updated and the 
search would continue normally until the next decrease in accuracy. In the graphs below, I make 
a distinction between how the search algorithm stops.

### *Results*
[![45.png](https://i.postimg.cc/C14mdvx2/45.png)](https://postimg.cc/2bV4K7vd)
[![4844.png](https://i.postimg.cc/Qdp5XFt4/4844.png)](https://postimg.cc/Jsztx4jZ)

When I was running my code, I noticed that the time differences between the forward 
selection and backward elimination search algorithms were very minimal for the small datasets.

[![7812225.png](https://i.postimg.cc/2ysbYYfN/7812225.png)](https://postimg.cc/N2xf8WMd)

[![124656.png](https://i.postimg.cc/263VBykf/124656.png)](https://postimg.cc/Q9GX2XK6)

However, the runtime differences between the two search algorithms were much more 
noticeable for the large datasets.

[![78725445.png](https://i.postimg.cc/BvbDf48t/78725445.png)](https://postimg.cc/7Cvbgvny)

[![35.png](https://i.postimg.cc/3JhvJgys/35.png)](https://postimg.cc/JtTnpH5x)


As shown, the runtime when the search algorithm ends after the initial decrease in 
percentage is much faster than the same dataset, with the same algorithm but when the search 
continues for all features. Although stopping the search algorithm on the first initial decrease is 
much faster than continuing for all features, the trade off is that it might not produce the best 
accuracy. One example of this is the given large dataset when ran with backward elimination: 
Although when running until the first decrease was much faster at 1 hr, 45 mins, and 18 secs, the 
resulting percentage was only 72.2%. However, when the same dataset was ran with the same 
search algorithm but through all the features, the highest accuracy found was 84.7% after a 
runtime of 5 hrs, 8 mins, and 45 secs. Another thing to note from the table is that forward 
selection tends to find the highest accuracy faster, and with the least number of features. 
Therefore, I believe forward selection produces better results overall. 
	  
Changing the stopping condition of the loop is simple and is shown accordingly below 
(code shown from domain.cpp and applies for both search algorithms).

Loop ends after initial decrease:

[![4218.png](https://i.postimg.cc/DwZm857M/4218.png)](https://postimg.cc/crpdzRgR)

Loop continues for all features (loop condition is in the while loop i.e while featureSet size != 
num of features for forward selection and while featureSet size != 1 for backward elimination):

[![684321.png](https://i.postimg.cc/SxvR88sN/684321.png)](https://postimg.cc/rD5q2Dxv)

### *Trace of personal small dataset (loop continues for all features) on Forward Selection:*
[![7842421896.png](https://i.postimg.cc/tCR4MDn4/7842421896.png)](https://postimg.cc/nM5xs41y)
[![652324.png](https://i.postimg.cc/cLr4sHmq/652324.png)](https://postimg.cc/0z1qn9FZ)
[![484635.png](https://i.postimg.cc/63wXG5dp/484635.png)](https://postimg.cc/qtmYPrbP)

### *Trace of personal small dataset (loop continues for all features) on Backward Elimination:*
[![36442.png](https://i.postimg.cc/L58FNCQd/36442.png)](https://postimg.cc/wyZrBVdF)
[![4520.png](https://i.postimg.cc/Y0DKV6Tv/4520.png)](https://postimg.cc/GTD5TyLR)
[![752323.png](https://i.postimg.cc/Y2dHh7jt/752323.png)](https://postimg.cc/NL2VzW1n)

### *Resources:*  
I consulted the following sources during the creation of this project:
* [Standardization Vs Normalization- Feature Scaling](https://www.youtube.com/watch?v=mnKm3YP56PY)
* [Data Normalization in Data Mining](https://www.geeksforgeeks.org/data-normalization-in-data-mining/)
* [Splitting int from a string](https://stackoverflow.com/questions/3421817/splitting-int-from-a-string)
* [Read integers from a text file with C++ ifstream](https://www.tutorialspoint.com/read-integers-from-a-text-file-with-cplusplus-ifstream)
* [Find average of input to vector C++](https://stackoverflow.com/questions/28574346/find-average-of-input-to-vector-c/28574413)
* [Calculate mean and standard deviation from a vector of samples in C++ using Boost](https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos)
* [Three Dimensional Distance Calculator](https://www.calculatorsoup.com/calculators/geometry-solids/distance-two-points.php)  

Borrowed code:  
* [C++ || Convert Time From Seconds Into Hours, Min, Sec Format](http://www.programmingnotes.org/?p=2062)
* [Measure execution time of a function in C++](https://www.geeksforgeeks.org/measure-execution-time-function-cpp/)



