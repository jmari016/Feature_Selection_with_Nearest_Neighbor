# Feature Selection with Nearest Neighbor
The program uses the nearest neighbor classifier inside a "wrapper" which searches for the best subset of features.

### Introduction
The goal for Project #2 for CS170 was to utilize the nearest neighbor classifier to search 
through a dataset and find the best combination of features for classification. In the assignment, 
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

### Results
![Image of results table when search continues for all features](https://www.picturepaste.ca/images/2020/06/10/45.png)
![Image of results table when search ends after intitial decrease](https://www.picturepaste.ca/images/2020/06/10/4844.png)

When I was running my code, I noticed that the time differences between the forward 
selection and backward elimination search algorithms were very minimal for the small datasets.

![Image of runtime of small datasets. Search continues for all features](https://www.picturepaste.ca/images/2020/06/10/7812225.png)

![Image of runtime of small datasets. Search ends after initial decrease.](https://www.picturepaste.ca/images/2020/06/10/124656.png)

However, the runtime differences between the two search algorithms were much more 
noticeable for the large datasets.

![Image of runtime of large datasets. Search continues for all features](https://www.picturepaste.ca/images/2020/06/10/78725445.png)

![Image of runtime of large datasets. Search ends after intitial decrease](https://www.picturepaste.ca/images/2020/06/10/35657f57808e44c955.png)


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

![Image of code ending after initial decrease](https://www.picturepaste.ca/images/2020/06/10/4218240b517d7726b6b9.png)

Loop continues for all features (loop condition is in the while loop i.e while featureSet size != 
num of features for forward selection and while featureSet size != 1 for backward elimination):

![Image of code continuing for all features](https://www.picturepaste.ca/images/2020/06/10/684321.png)

#### Trace of personal small dataset (loop continues for all features) on Forward Selection:
![Trace of personal small dataset on FS 1](https://www.picturepaste.ca/images/2020/06/10/7842421896.png)
![Trace of personal small dataset on FS 2](https://www.picturepaste.ca/images/2020/06/10/652324.png)
![Trace of personal small dataset on FS 3](https://www.picturepaste.ca/images/2020/06/10/484635.png)

#### Trace of personal small dataset (loop continues for all features) on Backward Elimination:
![Trace of personal small dataset on BE 1](https://www.picturepaste.ca/images/2020/06/10/36442.png)
![Trace of personal small dataset on BE 2](https://www.picturepaste.ca/images/2020/06/10/4520.png)
![Trace of personal small dataset on BE 3](https://www.picturepaste.ca/images/2020/06/10/752323.png)





