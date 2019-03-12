#pragma once

// C++ header
#include<iostream>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <cstddef>
#include <ctime>
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


// ===================== Struct Definitions ==========================================

// This is the comparison function for the Priority Queue
struct compareProcessStartTime {
	bool operator()(Process const &p1, Process const &p2) {
		// return "true" if "p1" is ordered before "p2", for example:
		return p1.getStartTime() > p2.getStartTime();
	}
};

struct compareProcessDurationTime {
	bool operator()(Process const &p1, Process const &p2) {
		// return "true" if "p1" is ordered before "p2", for example:
		return p1.getDurationTime() > p2.getDurationTime();
	}
};


struct numberOfLinesAndFromTheLine {
	int numberOfLines;
	int fromTheLine;
};

// ===================== Struct Definitions ==========================================




// ======================= Prototype Test Functions =========================

void storeToPriorityQueue(std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> &priorityQueue);
void popFromPriorityQueue(std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> &priorityQueue);
void popFromPriorityQueue(std::priority_queue<Process, std::vector<Process>, compareProcessDurationTime> &priorityQueue);
void testingPriorityQueueFunc();
void testVector();
void pirntVectorOfLines(std::vector<std::string> lineVector);


// ======================= Prototype Test Functions =========================








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





void readFileToVectors(std::vector<std::queue<Process>> &vectorOfQueuesPtr,
	std::vector<std::string> &vectorOfLines,
	std::vector<numberOfLinesAndFromTheLine> &vectorOfLinesAndUserLinesInFile) {

	std::queue<Process> queuePtrTemp;

	// declare temporary variables
	std::string strTemp;

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
	for (std::size_t j = 0 + 1; j < vectorOfLines.size(); j++) {
		//        std::cout << ' ' << *it << std::endl;
		lineCounter++;
		strTemp = vectorOfLines[j];
		numberOfLinesAndFromTheLine numOfLinesAndFromTheLineStruct;
		bool userFound = false;
		bool processCountFinished = false;
		// Temporary string builder
		std::string strBuilder = "";


		for (std::size_t i = 0; i < strTemp.length(); i++) {

			if (isalpha(strTemp[0])) {
				if (i == (strTemp.length() - 1)) {
					processCountFinished = true;
				}
				// if user was found already don't create duplicate queue for a  user
				if (!userFound) {
					//queuePtrTemp = new std::queue<Process>;
					vectorOfQueuesPtr.push_back(queuePtrTemp);
					userFound = true;
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
			if (processCountFinished) {
				try {
					numOfLinesAndFromTheLineStruct.fromTheLine = lineCounter;
					//                    numOfLinesAndFromTheLineStruct.numberOfLines = std::stoi(strBuilder);
					numOfLinesAndFromTheLineStruct.numberOfLines = std::stoi(strBuilder);
					vectorOfLinesAndUserLinesInFile.push_back(numOfLinesAndFromTheLineStruct);
				}
				catch (int e) {
					std::cout << "Error: " << e << std::endl;
				}
			}
		}
	}
	return;
}

void populateUserQueuesFromVectors(std::vector<std::queue<Process>> &vectorOfQueuesPtr,
	std::vector<std::string> vectorOfLines,
	std::vector<numberOfLinesAndFromTheLine> VectorOfLinesAndFromLines) {

	for (std::size_t userObjects = 0; userObjects < VectorOfLinesAndFromLines.size(); userObjects++) {

		for (std::size_t lines = 0; lines < VectorOfLinesAndFromLines.at(userObjects).numberOfLines; lines++) {
			bool isDuration = false;
			bool saveDuration = false;
			std::string strBuilder = "";
			Process userProcess = Process();

			for (int lineElements = 0;
				lineElements < vectorOfLines.at(VectorOfLinesAndFromLines.at(userObjects).fromTheLine + 1 + lines).size();
				lineElements++) {

				std::string lineStrTemp = vectorOfLines.at(VectorOfLinesAndFromLines.at(userObjects).fromTheLine + 1 + lines);

				// checking the start time and building the Start time string to be saved in the process obj
				if (isdigit(lineStrTemp[lineElements]) && !isDuration) {
					strBuilder.push_back(lineStrTemp[lineElements]);
				}
				// saving the start time to the process obj and settin the start of duration string builder
				else if (lineStrTemp[lineElements] == ' ') {
					userProcess.setStartTime(std::stoi(strBuilder));
					isDuration = true;
					strBuilder = "";
				}
				// building the duration string to be saved in the process obj
				else if (isDuration && isdigit(lineStrTemp[lineElements])) {
					strBuilder.push_back(lineStrTemp[lineElements]);
				}

				// Checking when to save the duration time to the process obj - saves the duration time after the string reaches its end
				// sets the durationTime of the temporary userProcess object 
				// saved it to the respective user queue
				if (lineElements == (lineStrTemp.length() - 1)) {
					userProcess.setDurationTime(std::stoi(strBuilder));
					// adding the user id to the user process 
					userProcess.setUser((int)userObjects);
					// adding process id to the user process
					userProcess.setProcessID((int)lines);
					vectorOfQueuesPtr.at(userObjects).push(userProcess);
				}
			}
		}
	}
}

void populatePriorityQueueFromUserQueues(std::vector<std::queue<Process>> VecOfQueues,
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> &priorityQueueStartTime,
	std::priority_queue<Process, std::vector<Process>, compareProcessDurationTime> &priorityQueueDurationTime) {

	for (int i = 0; i < VecOfQueues.size(); i++) {
		Process tempProcess;
		Process referenceFromQueue;
		while (!VecOfQueues.at(i).empty()) {

			referenceFromQueue = VecOfQueues.at(i).front();

			tempProcess = Process(referenceFromQueue);

			VecOfQueues.at(i).pop();

			priorityQueueStartTime.push(tempProcess);
			priorityQueueDurationTime.push(tempProcess);

		}

	}
	int k = 0;
}

int numberOfUsers(const std::vector<std::queue<Process>> &queues) {
	return queues.size();
}

int numberOfUsers(const std::vector<std::queue<Process>> *queues) {
	return queues->size();
}

int  numberOfProcesses(const std::queue<Process> &queue) {
	return queue.size();
}

int  numberOfProcesses(const std::queue<Process> *queue) {
	return queue->size();
}

void timeCalculation(double &quantumTime, double &userTime,
	const std::vector<std::queue<Process> > &vectorOfUserQueues) {

	quantumTime = determineQuantum();
	// calculate usertime 
	userTime = quantumTime / numberOfUsers(vectorOfUserQueues);

}

void calcProcesTime(std::vector<double> &processTime, const double &userTime,
	const std::vector<std::queue<Process> > *vectorOfUserQueues)
{
	// calculate userProcessTime
	for (int i = 0; i < numberOfUsers(vectorOfUserQueues); i++) {
		double numberOfProcessesInQueue = numberOfProcesses(vectorOfUserQueues->at(i));
		if (numberOfProcessesInQueue <= 0) {
			processTime.push_back(0);
		}
		else {
			processTime.push_back(userTime / numberOfProcessesInQueue);
		}
	}
}

double secondsToMilli(int seconds) {
	return seconds * 1000;
}
















// ============================================ Test functions ===========================================================================
// ============================================ Test functions ===========================================================================














// TEST FUNCTION
void storeToPriorityQueue(std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> &priorityQueue) {

	// std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> priorityQueue;

	//Process Process1 = Process(100, 2);
	//Process Process2 = Process(50, 2);
	//Process Process3 = Process(300, 2);
	//Process Process4 = Process(2, 2);
	//Process Process5 = Process(900, 2);
	//Process Process6 = Process(450, 2);
	//Process Process7 = Process(700, 2);
	//priorityQueue.push(Process1);
	//priorityQueue.push(Process2);
	//priorityQueue.push(Process3);
	//priorityQueue.push(Process4);
	//priorityQueue.push(Process5);
	//priorityQueue.push(Process6);
	//priorityQueue.push(Process7);

}

// TEST FUNCTION
void popFromPriorityQueue(std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> &priorityQueue) {
	while (!priorityQueue.empty()) {
		Process process = priorityQueue.top();
		priorityQueue.pop();
		std::cout << process.getStartTime() << std::endl;
	}
}

// TEST FUNCTION
void popFromPriorityQueue(std::priority_queue<Process, std::vector<Process>, compareProcessDurationTime> &priorityQueue) {
	while (!priorityQueue.empty()) {
		Process process = priorityQueue.top();
		priorityQueue.pop();
		std::cout << process.getDurationTime() << std::endl;
	}
}

// TEST FUNCTION
void testingPriorityQueueFunc() {
	// Code to testPriorityQueues
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> priorityQueue;
	storeToPriorityQueue(priorityQueue);
	popFromPriorityQueue(priorityQueue);
}

// TEST FUNCTION
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

// TEST FUNCTION
void pirntVectorOfLines(std::vector<std::string> lineVector) {
	for (std::vector<std::string>::iterator it = lineVector.begin(); it != lineVector.end(); it++) {
		std::cout << ' ' << *it << std::endl;
	}
}