
#include"fileParsing.h"
#include "Source.h"


// ========== Structure declarations ==============================================
struct schedulerPackage {
	std::vector<std::queue<Process>> * vecOfUserQueuesPtr;
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> * priorityQStartTimePtr;
	double * quantumTimePtr;
	std::vector<double> * processTimePtr;
	std::map<int, char>* userPosition_userCharMAPptr;
	std::queue<std::string>* outputFileLinesQueuePtr;

};

struct threadPackage {
	double * timer;
	Process * process;
	std::map<int, char>* userPosition_userCharMAPptr;
	std::queue<std::string>* outputFileLinesQueuePtr;

};

// ========== Structure declarations ==============================================




void threadCallBackFunc(double i) {
	std::cout << "Passed in variable: " << i << std::endl << std::endl;
	i++;
}


// https://www.quantstart.com/articles/Function-Objects-Functors-in-C-Part-1
// void createThreadFunc(void(*threadCallBackFunc)(int, int)) {
double exampleFuncWithFuncAsParameter(double i, void(*functionName)(double)) {
	(*functionName)(i);
	return 0;
}


DWORD WINAPI createProcess(LPVOID lpParam) {
	threadPackage * threadPackageObj = (threadPackage *)lpParam;
	std::string printOutString;
	char userID = threadPackageObj->userPosition_userCharMAPptr->at(threadPackageObj->process->getUser());

	if (!threadPackageObj->process->isStarted()) {
		threadPackageObj->process->setStarted(true);
		std::cout << *(threadPackageObj->timer) << " " << userID << " " << threadPackageObj->process->getProcessID() << " " << "started" << std::endl;
		
		printOutString = std::to_string(*(threadPackageObj->timer)) + " " + (userID) + " " + std::to_string(threadPackageObj->process->getProcessID()) +  " " + "started";
		threadPackageObj->outputFileLinesQueuePtr->push(printOutString);
	}

	std::cout << *(threadPackageObj->timer) << " " << userID << " " << threadPackageObj->process->getProcessID() << " " << "resumed" << std::endl;
	
	printOutString = std::to_string(*(threadPackageObj->timer)) + " " + (userID) + " " + std::to_string(threadPackageObj->process->getProcessID()) + " " + "resumed";
	threadPackageObj->outputFileLinesQueuePtr->push(printOutString);

	if (threadPackageObj->process->getExecutionTime() >= (threadPackageObj->process->getDurationTime() - threadPackageObj->process->getElapsedTime())) {

		threadPackageObj->process->setDone(true);
		threadPackageObj->process->setPaused(false);
		threadPackageObj->process->setUnusedTime(threadPackageObj->process->getExecutionTime() - (threadPackageObj->process->getDurationTime() - threadPackageObj->process->getElapsedTime()));
		threadPackageObj->process->setElapsedTime((threadPackageObj->process->getElapsedTime()) + (threadPackageObj->process->getExecutionTime() - threadPackageObj->process->getUnusedTime()));

		//*(threadPackageObj->timer) = *(threadPackageObj->timer) +
			//(threadPackageObj->process->getExecutionTime() - (threadPackageObj->process->getDurationTime() - threadPackageObj->process->getElapsedTime()));
		
			*(threadPackageObj->timer) = *(threadPackageObj->timer) +
			(threadPackageObj->process->getExecutionTime() - threadPackageObj->process->getUnusedTime());

		std::cout << *(threadPackageObj->timer) << " " << userID << " " << threadPackageObj->process->getProcessID() << " " << "finished" << std::endl;

		printOutString = std::to_string(*(threadPackageObj->timer)) + " " + (userID) + " " + std::to_string(threadPackageObj->process->getProcessID()) + " " + "finished";
		threadPackageObj->outputFileLinesQueuePtr->push(printOutString);

		threadPackageObj->process->setExecutionTime(0);

	}

	else {
		threadPackageObj->process->setDone(false);
		threadPackageObj->process->setPaused(true);
		threadPackageObj->process->setUnusedTime(0);
		threadPackageObj->process->setElapsedTime(threadPackageObj->process->getElapsedTime() + threadPackageObj->process->getExecutionTime());

		*(threadPackageObj->timer) = *(threadPackageObj->timer) +
			(threadPackageObj->process->getExecutionTime());

		std::cout << *(threadPackageObj->timer) << " " << userID << " " << threadPackageObj->process->getProcessID() << " " << "paused" << std::endl;

		printOutString = std::to_string(*(threadPackageObj->timer)) + " " + (userID) + " " + std::to_string(threadPackageObj->process->getProcessID()) + " " + "paused";
		threadPackageObj->outputFileLinesQueuePtr->push(printOutString);


		threadPackageObj->process->setExecutionTime(0);

	}

	return 0;
}


DWORD WINAPI scheduler(LPVOID lpParam) {
	schedulerPackage * schedulerPackagePtr;
	schedulerPackagePtr = (schedulerPackage *)lpParam;

	//bool firstTime = true;
	threadPackage * threadPackagePtr;

	double timer = 1;
	double *timerPtr;

	double quantumLength = *(schedulerPackagePtr->quantumTimePtr);

	while (true) {


		// Section - Check which processes are going to start this quantum
		// ================================================================

		// TODO : Fix the while(true) loop breaking
		// if() // all processes ended break the while loop
		//std::cout << "Scheduler Thread called" << std::endl;

		// Case: 1 --> First time the scheduler starts
		//if (firstTime) {
			// making a temporary Queue
		//std::queue<Process> tempPriorityQueue;

		// every process in the priority queue is checked if it has startTime < timer
		int priorityQueueSize = (schedulerPackagePtr->priorityQStartTimePtr->size());
		for (int i = 0; i < priorityQueueSize; i++) {
			// poping the highest priority -> eariest time
			Process poppedProcess = schedulerPackagePtr->priorityQStartTimePtr->top();
			// push the popped process to the temporary queue !!!not the priority Queue!!!
			//tempPriorityQueue.push(poppedProcess);
			if (poppedProcess.getStartTime() < (timer + *(schedulerPackagePtr->quantumTimePtr))) {

				// save the popped process to its respective user queue
				schedulerPackagePtr->vecOfUserQueuesPtr->at(poppedProcess.getUser()).push(poppedProcess);
				schedulerPackagePtr->priorityQStartTimePtr->pop();
			}
			else {
				break;
			}
		}

		// variables declaration - required for both userTime and executionTime calculation
		// --------------------------------------------
		double userTimeThisQuantum = 0;
		int numberOfuserThisQuantum = 0;
		std::map<int, bool> userQueuePos_isActiveThisQuantumMap;
		std::vector<double>executionTimeThisQuantum;

		// --------------------------------------------
		// variables required for both userTime and executionTime calculation



		// calculating the userTime for this quantum
		// --------------------------------------------

		int vectorOfUserQueueSize;
		vectorOfUserQueueSize = schedulerPackagePtr->vecOfUserQueuesPtr->size();
		// map conatianing : key->UserQueuePos, val->userTimeThisQuantum
		for (int i = 0; i < vectorOfUserQueueSize; i++) {
			if (schedulerPackagePtr->vecOfUserQueuesPtr->at(i).empty()) {
				// don't increment the numberOfUserThisQuantum;
				userQueuePos_isActiveThisQuantumMap[i] = false;

			}
			else {
				numberOfuserThisQuantum++;
				userQueuePos_isActiveThisQuantumMap[i] = true;
			}


			/*int queueSize = schedulerPackagePtr->vecOfUserQueues->at(i).size();
			for (int j = 0; j < queueSize; j++) {
			}*/
		}
		if (numberOfuserThisQuantum != 0) {
			userTimeThisQuantum = *(schedulerPackagePtr->quantumTimePtr) / numberOfuserThisQuantum;
		}
		else {
			// TODO : confused what this line should be --- >> what happens when there are no users in the queue
			userTimeThisQuantum = *(schedulerPackagePtr->quantumTimePtr);

			break;  // <---------------- Break maybe??? 
		}
		// --------------------------------------------
		// calculated the userTime for this quantum


			// every queue in the vector is checked to find any process that starts in between allotted user time and pushed back to the priority queue
		int vectorOfQueuesSize = schedulerPackagePtr->vecOfUserQueuesPtr->size();
		for (int i = 0; i < vectorOfQueuesSize; i++) {
			std::queue<Process> tempUserQueue;
			std::queue<Process>* tempUserQueuePtr;
			tempUserQueuePtr = &tempUserQueue;

			while (!schedulerPackagePtr->vecOfUserQueuesPtr->at(i).empty()) {

				Process tempProcessObj = schedulerPackagePtr->vecOfUserQueuesPtr->at(i).front();
				schedulerPackagePtr->vecOfUserQueuesPtr->at(i).pop();

				if ((tempProcessObj.getStartTime() + userTimeThisQuantum) > (timer + *(schedulerPackagePtr->quantumTimePtr))) {
					schedulerPackagePtr->priorityQStartTimePtr->push(tempProcessObj);
				}
				else {
					tempUserQueue.push(tempProcessObj);
				}
			}
			schedulerPackagePtr->vecOfUserQueuesPtr->at(i) = tempUserQueue;
		}


		// calculating process execution timer This Quantum
		// --------------------------------------------
		//std::vector<double>executionTimeThisQuantum;

		for (int i = 0; i < userQueuePos_isActiveThisQuantumMap.size(); i++) {
			if (userQueuePos_isActiveThisQuantumMap.at(i) == true) {
				int numberOfProcessesInAQueue = schedulerPackagePtr->vecOfUserQueuesPtr->at(i).size();
				executionTimeThisQuantum.push_back(userTimeThisQuantum / numberOfProcessesInAQueue);
			}
			else {
				executionTimeThisQuantum.push_back(0);
			}
		}

		// --------------------------------------------
		// calculated process execution timer This Quantum


		// running the processes in inside a thread from the user queues with approprite timer and userTime and executionTime 
		// ------------------------------------------------------------------------------------------------------------------

		for (int m = 0; m < schedulerPackagePtr->vecOfUserQueuesPtr->size(); m++) {

			Process * tempProcess;
			threadPackagePtr = new threadPackage();
			timerPtr = &timer;
			std::queue<Process> queueHoldingPausedProcesses;

			while (!schedulerPackagePtr->vecOfUserQueuesPtr->at(m).empty()) {


				tempProcess = &(schedulerPackagePtr->vecOfUserQueuesPtr->at(m).front());
				schedulerPackagePtr->vecOfUserQueuesPtr->at(m).pop();
				tempProcess->setExecutionTime(executionTimeThisQuantum.at(m));

				// creating a new threadPackage
				threadPackagePtr->process = tempProcess;
				threadPackagePtr->timer = timerPtr;
				threadPackagePtr->userPosition_userCharMAPptr = schedulerPackagePtr->userPosition_userCharMAPptr;
				threadPackagePtr->outputFileLinesQueuePtr = schedulerPackagePtr->outputFileLinesQueuePtr;

				// Call the thread with the temp processs
				HANDLE processThread = CreateThread(NULL, 0, createProcess, threadPackagePtr, 0, NULL);
				WaitForSingleObject(processThread, INFINITE);

				if (threadPackagePtr->process->isPaused()) {
					queueHoldingPausedProcesses.push(*(threadPackagePtr->process));
				}

			}

			while (!queueHoldingPausedProcesses.empty()) {
				schedulerPackagePtr->vecOfUserQueuesPtr->at(m).push(queueHoldingPausedProcesses.front());
				queueHoldingPausedProcesses.pop();
			}

			// once the tread completes push the process back in to the queue --> if it is not done
			int iii = 0;

		}

		// ------------------------------------------------------------------------------------------------------------------
		// running the processes in inside a thread from the user queues with approprite timer and userTime and executionTime 

		int iiii = 0;

	}


	std::cout << "Scheduler Thread End" << std::endl;

	return 0;
}



void convertToPointers(std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> &priorityQueueStartTime,
	std::vector<std::queue<Process>> &vectorOfUserQueues, double &quantumTime, schedulerPackage * &schedulerPackagePtr, std::map<int, char> &userPosition_userChar, std::queue<std::string> &outputFileLinesQueue)
{

	// get a pointer to the priority Start Time Queue
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> * priorityQStartTimePtr = &priorityQueueStartTime;;

	// get a pointer of vector of user Queues
	std::vector<std::queue<Process> > *vecOfUserQPtr = &vectorOfUserQueues;;

	// get the pointer of quantumTime
	double * quantumTimePtr = &quantumTime;


	std::map<int, char> * userPosition_userCharMapPtr = &userPosition_userChar;

	std::queue<std::string>* outputFileLinesQueuePtr = &outputFileLinesQueue;

	schedulerPackagePtr = new schedulerPackage();
	schedulerPackagePtr->priorityQStartTimePtr = priorityQStartTimePtr;
	schedulerPackagePtr->vecOfUserQueuesPtr = vecOfUserQPtr;
	schedulerPackagePtr->quantumTimePtr = quantumTimePtr;
	schedulerPackagePtr->userPosition_userCharMAPptr = userPosition_userCharMapPtr;
	schedulerPackagePtr->outputFileLinesQueuePtr = outputFileLinesQueuePtr;
}

void clearingVectorOfUserQueues(std::vector<std::queue<Process>> &vectorOfUserQueues)
{
	int vectorSize = vectorOfUserQueues.size();
	vectorOfUserQueues.clear();
	for (int i = 0; i < vectorSize; i++) {
		std::queue<Process> newEmptyQueue;
		vectorOfUserQueues.push_back(newEmptyQueue);
	}
}

int main() {

	//randomNumberFileGenerator();



// Declaration of Main Variables needed for the Assignment
// requires the comparator struct to be declared
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> priorityQueueStartTime;
	std::priority_queue<Process, std::vector<Process>, compareProcessDurationTime> priorityQueueDurationTime;
	std::vector<std::queue<Process> > vectorOfUserQueues;
	std::vector<std::string> vectorOfLines;
	std::vector<numberOfLinesAndFromTheLine> vectorOfLinesAndFromLines;
	std::queue<std::string> outputFileLinesQueue;

	std::map<int, char> userPosition_userChar;


	double quantumTime;
	double userTime;
	std::vector<double> processTime;


	// =============== above all the variables required are declared above =============


	readFileToVectors(vectorOfUserQueues, vectorOfLines, vectorOfLinesAndFromLines, userPosition_userChar);
	populateUserQueuesFromVectors(vectorOfUserQueues, vectorOfLines, vectorOfLinesAndFromLines);
	populatePriorityQueueFromUserQueues(vectorOfUserQueues, priorityQueueStartTime, priorityQueueDurationTime);

	// calculate the time variables needed for the user
	calcQuantumTime(quantumTime);


	// =============== above these are file parsing to vector and Queues --> setting up functions =============



	// ============= Below this line we are setting all the variables to pointers and using pointers at all times ============

	// make a structure pointer containing all variables
	schedulerPackage * schedulerPackagePtr;

	// clear vector of user queues before creating its pointer and passing it to the scheduler
	clearingVectorOfUserQueues(vectorOfUserQueues);
	// function to convert variables to pointer 
	convertToPointers(priorityQueueStartTime, vectorOfUserQueues, quantumTime, schedulerPackagePtr, userPosition_userChar, outputFileLinesQueue);

	// crate a scheduler thread
	HANDLE schedulerThread = CreateThread(NULL, 0, scheduler, schedulerPackagePtr, 0, NULL);
	WaitForSingleObject(schedulerThread, INFINITE);


	outputFileGenerator(outputFileLinesQueue);



	std::cout << "pause... " << std::endl;
	int i = 0;
	std::cin >> i;

	return 0;
}




































