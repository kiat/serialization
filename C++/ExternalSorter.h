#ifndef EXTERNALSORTER_H
#define EXTERNALSORTER_H

#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <cassert>
#include <queue>
#include <map>
#include <unordered_map>
#include "FileHandler.h"
#include "ItemPart.h"
#include "PartIP.h"
#include "LineItemIP.h"
#include "CustomerIP.h"
using namespace std;


#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


//If you want to compare two objects, 
//but you only have handles to them, use these comparators:

//Ascending Sorter:
template<class T>
struct UniversalPointerComparatorAscending {
	bool operator()(T* a, T* b) { 
		return *a < *b; 
	} 
};

//Descending Sorter:
template<class T>
struct UniversalPointerComparatorDescending {
	bool operator()(T* a, T* b) { 
		if (*a < *b) 
			return false; 
		return true;
	}
};


template<class T>
class ExternalSorter {
	private: 
		char * inPlaceFileStart;
		FileHandler<T> * fh;
		int fileCount;
		vector<FileHandler<T> *> allSortedFileHandlers;
		FileHandler<T> * sortedFileHandler;

		//Split the big file in 10 smaller sorted file.
		//It is ensured that number of objects is multiple of 10.
		int fileSplit = 0;
		
		//Total time spent on IO:
		double totalIOTime = 0.0;

		//Total time required for the entire External Sort step:
		double totalTimeExternalSort = 0.0;


		//Required for Inplace:
		int objectType;



		//Split the large file into parts and sort:
		void sortByParts(int countObject) {

			//Switch to InPlace code:
			if (fh->getSerializationType() == 2) {
				switch(objectType) {
					case 0:
						return sortByPartsInPlace<PartIP>(countObject);
						break;
					case 1:
						return sortByPartsInPlace<LineItemIP>(countObject);
						break;
					case 2:
						return sortByPartsInPlace<CustomerIP>(countObject);
						break;
					default: 
						break;
				}
			}


			// Auxiliary variables:
			int objectsReadSoFar = 0;
			int objectsInABatch = (countObject/fileSplit);	
			fileCount = 0;


			//DELETE: Change the APIs: getObjectsFromFile
			float oneLoopTime = 0.0;
			//vector<T*> curSet;
			while (objectsReadSoFar < countObject) {
				cout << "Loop start!" << endl;
				
				//DELETE
				auto loopTimer = chrono::steady_clock::now();


				//DELETE:
				auto beginTime = chrono::steady_clock::now();

				vector<T*> curSet;

				if ((countObject - objectsReadSoFar) > objectsInABatch)	{
					//curSet = fh->getObjectsFromFile(objectsReadSoFar, objectsInABatch);

					//DELETE:
					curSet = fh->getObjectsFromOpenFile(objectsReadSoFar, objectsInABatch);

					objectsReadSoFar += objectsInABatch;
				}
				else {
					int currentBatch = countObject - objectsReadSoFar;
					//curSet = fh->getObjectsFromFile(objectsReadSoFar, currentBatch);

					//DELETE:
					curSet = fh->getObjectsFromOpenFile(objectsReadSoFar, currentBatch);

					objectsReadSoFar += currentBatch;
				}


				//Timers:
				totalIOTime += fh->getPreviousIOTime();

				//DELETE:
				cout << "Read: " << fh->getPreviousIOTime() << endl;

				//DELETE:
				float totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
				cout << "(Read + Deserialize): " << totalTime << endl;



				//DELETE:
				beginTime = chrono::steady_clock::now();

				//Sort:
				sort(curSet.begin(), curSet.end(), UniversalPointerComparatorAscending<T>());

				//DELETE:
 				totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
				cout << "Sort: " << totalTime << endl;				


				//DELETE:
				beginTime = chrono::steady_clock::now();

				string curFileName = fh->getFileName() + "." + SSTR(fileCount) + ".sorted";
				FileHandler<T> * nfh = new FileHandler<T> (curFileName, fh->getSerializationType(), "InPlace" + curFileName);
				nfh->appendObjectsToFile(curSet);
				totalIOTime += nfh->getPreviousIOTime();  //IO Time.


				//DELETE:
				//cout << "NFH: " << nfh->getPreviousIOTime() << endl;

				//DELETE:
 				totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
				cout << "Write: " << totalTime << endl;	


				//DELETE:
				beginTime = chrono::steady_clock::now();


				nfh->flush();
				totalIOTime += nfh->getPreviousIOTime();  //IO Time. 

				//DELETE:
 				totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
 				cout << "Flush: " << totalTime << endl;


				//DELETE:
				beginTime = chrono::steady_clock::now();


				fileCount++;
				allSortedFileHandlers.push_back(nfh);

				//Memory Leak:
				for (int i = 0; i < curSet.size(); i++) {
					curSet.at(i)->freePointedResources();
					delete curSet.at(i);
				}
				curSet.clear();


				//DELETE:
 				totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
				cout << "Memory Release: " << totalTime << endl;	



				//DELETE:
 				oneLoopTime = chrono::duration<float>(chrono::steady_clock::now() - loopTimer).count();
				cout << "Sorted current batch: " << oneLoopTime << endl;
				totalTimeExternalSort += oneLoopTime;
			}


			//Print total time for first step:
			cout << "Total time in 1st step: " << totalTimeExternalSort << endl;
		}


		// Merge the smaller files into a large file.
		void MergeByPartsPaged() {
		
			//Switch to InPlace code:
			if (fh->getSerializationType() == 2) {
				switch(objectType) {
					case 0:
						return MergeByPartsPagedInPlace<PartIP>();
						break;
					case 1:
						return MergeByPartsPagedInPlace<LineItemIP>();
						break;
					case 2:
						return MergeByPartsPagedInPlace<CustomerIP>();
						break;
					default: 
						break;
				}
			}

			//Timer:
			auto mergeTimer = chrono::steady_clock::now();

			priority_queue<T*, vector<T*>, UniversalPointerComparatorDescending<T> > pq;	
			int * objectsRemainingInPage = new int[fileCount];
			int * pageIndexReadSoFar = new int[fileCount];
			unordered_map <T* , int> metadataPQ;
		

			// Set counters as 0 for all files..
			for (int i = 0; i < allSortedFileHandlers.size(); i++) {
				pageIndexReadSoFar[i] = 0;	
				objectsRemainingInPage[i] = 0;
			}

			//Read first pages from all files:
			for (int i = 0; i < allSortedFileHandlers.size(); i++) {

				//DELETE:
				//auto beginTime = chrono::steady_clock::now();


				if (pageIndexReadSoFar[i] < allSortedFileHandlers.at(i)->getPageCount()) {
					vector<T*> curSet = allSortedFileHandlers.at(i)->getAllObjectsInPage(pageIndexReadSoFar[i]);

					totalIOTime += allSortedFileHandlers.at(i)->getPreviousIOTime(); //IO time.

					//DELETE:
					// cout << "OT Page Read: " << allSortedFileHandlers.at(i)->getPreviousIOTime() << endl;	


					for (int j = 0; j < curSet.size(); j++) {
						pq.push(curSet.at(j));
						metadataPQ[curSet.at(j)] = i;
					}
					pageIndexReadSoFar[i] += 1;
					objectsRemainingInPage[i] = curSet.size();
				}	



				//DELETE:
				// float totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
				// cout << "PageReadTotal: " << totalTime << endl;


			}
	

			//Global Sorted File:
			string curFileName = fh->getFileName() + ".sorted.all";
			sortedFileHandler = new FileHandler<T> (curFileName, fh->getSerializationType(), "InPlace" + curFileName);
			vector<T*> temp;
			while(!pq.empty()) {

				//DELETE:
				//auto beginTime = chrono::steady_clock::now();

				T * curObject = pq.top();
				temp.push_back(curObject);
				pq.pop();

				int fileIndex = metadataPQ[curObject];
				metadataPQ.erase(curObject);
				objectsRemainingInPage[fileIndex]--;

				//DELETE:
				//float totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
				//cout << "Loop 1: " << totalTime << endl;



				//DELETE:
				//beginTime = chrono::steady_clock::now();


				if (objectsRemainingInPage[fileIndex] == 0) {
					
					if(pageIndexReadSoFar[fileIndex] < allSortedFileHandlers.at(fileIndex)->getPageCount()) {


					//DELETE:
					//auto bTime = chrono::steady_clock::now();

					vector<T*> curSet = allSortedFileHandlers.at(fileIndex)->getAllObjectsInPage(pageIndexReadSoFar[fileIndex]);
					totalIOTime += allSortedFileHandlers.at(fileIndex)->getPreviousIOTime(); //IO time.

					//DELETE:
					// cout << "Page Read IO: " << allSortedFileHandlers.at(fileIndex)->getPreviousIOTime() << endl;
					// cout << "Page Read OH: "<< allSortedFileHandlers.at(fileIndex)->getOverheadTime() << endl; 
					// totalTime = chrono::duration<float>(chrono::steady_clock::now() - bTime).count();
					// cout << "Part 1: " << totalTime << endl;


					//DELETE:
					// bTime = chrono::steady_clock::now();


					for (int j = 0; j < curSet.size(); j++) {
						pq.push(curSet.at(j));
						metadataPQ[curSet.at(j)] = fileIndex;
					}
					pageIndexReadSoFar[fileIndex] += 1;
					objectsRemainingInPage[fileIndex] = curSet.size();

					//DELETE:
					// totalTime = chrono::duration<float>(chrono::steady_clock::now() - bTime).count();
					// cout << "Part 2: " << totalTime << endl;



					//DELETE:
					// bTime = chrono::steady_clock::now();

					//The sorted vector temp is kept in memory. We don't want it to be exceptionally large.
					sortedFileHandler->appendObjectsToFile(temp); 
					totalIOTime += sortedFileHandler->getPreviousIOTime(); //IO time.

					//DELETE:
					// totalTime = chrono::duration<float>(chrono::steady_clock::now() - bTime).count();
					// cout << "Part 3: " << totalTime << endl;


					//DELETE:
					// cout << "Append: " << sortedFileHandler->getPreviousIOTime() << endl;	



					//DELETE:
					// bTime = chrono::steady_clock::now();
					
					//Memory Leak:				
					for (int i = 0; i < temp.size(); i++) {
						temp.at(i)->freePointedResources();
						delete temp.at(i);
					}
					//Clear the vector for subsequent usage:
					temp.clear();

					//DELETE:
					// totalTime = chrono::duration<float>(chrono::steady_clock::now() - bTime).count();
					// cout << "Part 4: " << totalTime << endl;

				}
			}	
				//If ends


				// totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
				// cout << " loop 2: " << totalTime << endl;			
			}

			//DELETE:
			// cout << " Outside while loop: " << endl;

			sortedFileHandler->appendObjectsToFile(temp); 
			totalIOTime += sortedFileHandler->getPreviousIOTime(); //IO time.

			//DELETE:
			// cout << "Append: " << sortedFileHandler->getPreviousIOTime() << endl;	
			
			//Memory Leak:		
			for (int i = 0; i < temp.size(); i++) {
				temp.at(i)->freePointedResources();
				delete temp.at(i);
			}
			temp.clear();			

			
			//Flush the sorted file.
			sortedFileHandler->flush();
			totalIOTime += sortedFileHandler->getPreviousIOTime(); //IO time.	

			//DELETE:
			// cout << "Flush: " << sortedFileHandler->getPreviousIOTime() << endl;				

			//Memory Leak:
			delete[] objectsRemainingInPage;
			delete[] pageIndexReadSoFar;


			float mergeTotalTime = chrono::duration<float>(chrono::steady_clock::now() - mergeTimer).count();
			cout << "Total time in 2nd step: " << mergeTotalTime << endl;
			totalTimeExternalSort += mergeTotalTime;

		}





	public:

		//Default Destructor:
		~ExternalSorter() {
			delete sortedFileHandler;
		}

		ExternalSorter(FileHandler<T> * fh) {
			this->fh = fh;
			//this->inPlaceFileStart = fileStart;
		}
		
		FileHandler<T> * sortFile(int countObject, int objectType, int totalFileSplits) {			
			//This is required for InPlace:
			this->objectType = objectType;

			fileSplit = totalFileSplits;

			totalIOTime = 0.0;
			overheadTime = 0.0;
			totalTimeExternalSort = 0.0;

			//Sanity Check:
			if (fh->getObjectCount() < countObject) {
				throw invalid_argument("Not enough objects to sort.");
			}
			
			//split file into smaller parts and sort them:
			//auto beginTime = chrono::steady_clock::now();
			sortByParts(countObject);
			//float totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
			//cout << "SortByParts: " << totalTime << endl;



			//Merge the split files:
			//beginTime = chrono::steady_clock::now();
			MergeByPartsPaged();
			//totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
			//cout << "MergeByPartsPaged: " << totalTime << endl;


			//Memory Leak: Split file handlers are supposed to be deleted.
			for (int i = 0; i < allSortedFileHandlers.size(); i++) {
				delete allSortedFileHandlers.at(i);
			}

			return sortedFileHandler;
		}
	

		double getTotalIOTime() {
			return totalIOTime;
		} 

		double getOverheadTime() {
			return overheadTime;
		}
		

		double getTotalTimeExternalSort() {
			return totalTimeExternalSort;
		}
};

#endif