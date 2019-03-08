// C++ header
#include<iostream>
#include <string>
#include <stdio.h>
#include <ctype.h>
// Thread headers
//#include <processthreadsapi.h>
#include <windows.h>
#include <handleapi.h>
// File Access header
#include <fstream>
// Created class header
#include "Process.h"

// Algorithm headers
#include<queue> //https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
#include <vector>

// This is the comparison function for the Priority Queue
struct compareProcessStartTime {
	bool operator()(Process const &p1, Process const &p2) {
		// return "true" if "p1" is ordered before "p2", for example:
		return p1.getStartTime() > p2.getStartTime();
	}
};

struct numberOfLinesAndFromTheLine {
	std::string numberOfLines;
	int fromTheLine;
};


void storeToPriorityQueue(std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> &priorityQueue) {

	// std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> priorityQueue;

	Process Process1 = Process(100, 2);
	Process Process2 = Process(50, 2);
	Process Process3 = Process(300, 2);
	Process Process4 = Process(2, 2);
	Process Process5 = Process(900, 2);
	Process Process6 = Process(450, 2);
	Process Process7 = Process(700, 2);
	priorityQueue.push(Process1);
	priorityQueue.push(Process2);
	priorityQueue.push(Process3);
	priorityQueue.push(Process4);
	priorityQueue.push(Process5);
	priorityQueue.push(Process6);
	priorityQueue.push(Process7);

}

void popFromPriorityQueue(std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> &priorityQueue) {
	while (!priorityQueue.empty()) {
		Process process = priorityQueue.top();
		priorityQueue.pop();
		std::cout << process.getStartTime() << std::endl;
	}
}

void testingPriorityQueueFunc() {
	// Code to testPriorityQueues
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> priorityQueue;
	storeToPriorityQueue(priorityQueue);
	popFromPriorityQueue(priorityQueue);
}

// the randomFileGenerator function creates and writes some predefined integers to the input.txt file
void randomNumberFileGenerator() {
	std::ofstream outfile("input.txt");
	outfile << "4" << std::endl;
	outfile << "A 2" << std::endl;
	outfile << "4 3" << std::endl;
	outfile << "1 5" << std::endl;
	outfile << "B 1" << std::endl;
	outfile << "5 6" << std::endl;
	outfile.close();
}

// TODO Fix file reader
int determineQuantum() {

	std::ifstream fileVar("./input.txt");
	char character;
	std::string integerValuesStr;
	bool firstNewLine = false;
	//int size;

	while (fileVar.get(character)) {
		if (character != '\n') {
			integerValuesStr.push_back(character);
			//integerValuesStr = integerValuesStr + character;
			//std::cout << integerValuesStr << std::endl;
		}
		else {
			if (firstNewLine == false) {
				firstNewLine = true;
				fileVar.close();
				//std::cout << "This is a test" << std::endl;
				return std::stoi(integerValuesStr);
			}
			else {
				break;
			}
		}
	}
	return -1;
}

// TODO Fix output file generator
void outputFileGenerator(int *arr, int size) {
	std::ofstream outfile("./output.txt");
	for (int i = 0; i < size; i++) {
		outfile << arr[i] << std::endl;
	}
	outfile.close();
}

// TODO Fix print file contents
void printTextFiles() {
	std::ifstream fileVar("./input.txt");
	char character;
	while (fileVar.get(character)) {
		std::cout << character;
	}
	fileVar.close();
}


void testVector() {
	std::vector<int> intVector;
	intVector.push_back(1);
	intVector.push_back(2);
	intVector.push_back(3);
	intVector.push_back(4);

	for (std::vector<int>::iterator it = intVector.begin(); it != intVector.end(); it++) {
		std::cout << ' ' << *it << std::endl;
	}
}

void pirntVectorOfLines(std::vector<std::string> lineVector) {
	for (std::vector<std::string>::iterator it = lineVector.begin(); it != lineVector.end(); it++) {
		std::cout << ' ' << *it << std::endl;
	}
}

void testGetline() {

}


// TODO Fix file reader
void readProcessesToQueues(std::vector<std::queue<Process> *> VectorOfQueuesPtr) {

	std::vector<numberOfLinesAndFromTheLine> linesAndFromLineVec;

	std::queue<Process> *queuePtrTemp;


	// declare temporary variables
	std::string strTemp;
	std::vector<std::string> vectorOfLines;

	// readfile to ifstream object
	std::ifstream fileObj("./input.txt");

	// Check if object is valid
	if (!fileObj) {
		std::cerr << "Unable to open file " << std::endl;
		return;
	}

	// Read the next line from File untill it reaches the end.
	while (std::getline(fileObj, strTemp)) {
		// Line contains string of length > 0 then save it in vector
		if (strTemp.size() > 0)
			vectorOfLines.push_back(strTemp);
	}
	//Close The File
	fileObj.close();


	// line counter is initialized to 1 to ignore the first line in the input file, which is quantum time
	int lineCounter = 0;

	// Reading the vector containing lines from position 1 and skipping position 0. Position 0 is ignored because its Quantum time
	for (int j = 0 + 1; j < vectorOfLines.size(); j++) {
		//        std::cout << ' ' << *it << std::endl;
		lineCounter++;
		strTemp = vectorOfLines[j];
		numberOfLinesAndFromTheLine numOfLinesAndFromTheLineStruct;
		bool userFound = false;
		// Temporary string builder
		std::string strBuilder = "";

		//        std::cout << strTemp.length() << std::endl;


		for (int i = 0; i < strTemp.length(); i++) {

			if (isalpha(strTemp[0])) {
				// checking for new line
				if (strTemp[i] != '\n') {
					// checking for user

//                    if (isalpha(strTemp[i])) {
					// condition to check for if user is more than 1 letter
					if (!userFound) {
						queuePtrTemp = new std::queue<Process>;
						VectorOfQueuesPtr.push_back(queuePtrTemp);
						userFound = true;
						//                        }
					}
					// check for space and ignore it
					else if (strTemp[i] == ' ') {
						// continue
					}
					// check for the number of processes
					else if (isdigit(strTemp[i])) {
						strBuilder.push_back(strTemp[i]);
					}
				}
				try {
					numOfLinesAndFromTheLineStruct.fromTheLine = lineCounter;
					//                    numOfLinesAndFromTheLineStruct.numberOfLines = std::stoi(strBuilder);
					numOfLinesAndFromTheLineStruct.numberOfLines = strBuilder;
					linesAndFromLineVec.push_back(numOfLinesAndFromTheLineStruct);
				}
				catch (int e) {
					std::cout << "Error: " << e << std::endl;
				}
			}
		}


	}






	// testing if the line by line strings are properly stored in the vector
	// pirntVectorOfLines(vectorOfLines);



	//    userQueueArr = new std::queue[]

	//
	//
	//    bool firstNewLine = false;
	//    bool userFound = false;
	//    bool addProcess = false;
	//    int counter = 0;
	//
	//    while (fileObj.get(character)) {
	//
	//
	//        if (character != '\n') {
	//            if (character != ' ') {
	//                user.push_back(character);
	//            }
	//            user.push_back(character);
	//            //values = values + character;
	//            //std::cout << values << std::endl;
	//        } else {
	//            if (firstNewLine == false) {
	//                firstNewLine = true;
	//                user.clear();
	//            } else {
	//                arr[counter] = std::stoi(user);
	//                counter++;
	//                user.clear();
	//                //std::cout << "This is a test" << std::endl;
	//            }
	//        }
	//    }


}


int main() {

	//    testingPriorityQueueFunc();

	    randomNumberFileGenerator();
	//    std::cout << determineQuantum() << std::endl;

	// ===================================================

	std::vector<std::queue<Process> *> Queues;


	//readProcessesToQueues(Queues);

	//    std::cout << isalpha('Z') << std::endl;


	//    testVector();


	return 0;
}



