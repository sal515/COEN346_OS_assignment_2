
#include"fileParsing.h"
#include "Source.h"

// ========== Structure declarations ==============================================
struct schedulerPackage {
	std::vector<std::queue<Process>> * vecOfUserQPtr;
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> * priorityQStartTimePtr;
	double * quantumTimePtr;
	double * userTimePtr;
	std::vector<double> * processTimePtr;
};

// ========== Structure declarations ==============================================


void testTimerLib() {
	//// clock Variable
	//clock_t schedulerClockStart = 0;
	//clock_t schedulerClockEnd = 0;
	//clock_t pausedStart = 0;
	//clock_t pausedEnd = 0;
	//
	//// start scheduler clock
	//schedulerClockStart = clock();

	//// Do something

	//// scheduler needs to be paused
	//pausedStart = clock();
	//
	//// Do something
	//
	//pausedStart = clock();





	//clock_t looper = secondsToMilli(1);
	//std::cout << "time before clock(): " << time << std::endl;

	////time is in milliseconds 
	//time = clock();

	////sleep is in milliseconds 
	//Sleep(secondsToMilli(1));

	//std::cout << "time after clock(): " << time << std::endl;

	//time = clock() % looper ;

	//std::cout << "time after after clock(): " << time << std::endl;

	//


}

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


//HANDLE createThreadFunction(DWORD WINAPI(functionName)(), Process * objParam1) {
	//return CreateThread(NULL, 0, (functionName), objParam1, 0, NULL);
//}


//DWORD WINAPI testThreadFunc1(LPVOID lpParam) {
//	threadData * dataPtr = new threadData();
//	dataPtr = (threadData*)lpParam;
//	int counter = 0;
//	while (TRUE) {
//		if (counter < 5) {
//			std::cout << "Test func 1 " << dataPtr->getProcessTime() << std::endl;
//			counter++;
//		}
//		else break;
//	}

void createProcess() {


}

DWORD WINAPI createProcess(LPVOID lpParam) {
	Process * processObj = (Process *)lpParam;

	//processObj->getStartTime();
	processObj->setElapsedTime(processObj->getElapsedTime() + processObj->getExecutionTime());
	if (processObj->getElapsedTime() == processObj->getDurationTime()) {
		std::cout << "Done" << std::endl;
	}
	else {
		std::cout << "process start time : " << processObj->getStartTime() << " process duration : " << processObj->getDurationTime() << std::endl;
	}

		// below is good algo
		// Timer = 0;
		// process.start()
		// if(process.duraton == process.elapsed){
		//		cout << "Process paused"; 
		//		process.suspend()
		//}

	return 0;
}

DWORD WINAPI scheduler(LPVOID lpParam) {
	schedulerPackage * sPObjPtr;
	sPObjPtr = (schedulerPackage *)lpParam;

	// calculate execution time in everyloop
	// executionTime

	bool firstTime = true;

	while (true) {
		// TODO : Fix the while(true) loop breaking
		// if() // all processes ended break the while loop
		//std::cout << "Scheduler Thread called" << std::endl;
		double timer = 0;
		// Case: 1 --> First time the scheduler starts
		if (firstTime) {
			// making a temporary Queue
			std::queue<Process> tempPriorityQueueProcessHolder;
			// every process in the priority queue is checked if it has startTime < Quantum time
			for (int i = 0; i < (sPObjPtr->priorityQStartTimePtr->size()); i++) {
				// poping the highest priority -> eariest time
				Process poppedProcess = sPObjPtr->priorityQStartTimePtr->top();
				sPObjPtr->priorityQStartTimePtr->pop();
				// push the popped process to the temporary queue !!!not the priority Queue!!!
				tempPriorityQueueProcessHolder.push(poppedProcess);
				if (poppedProcess.getStartTime() < *(sPObjPtr->quantumTimePtr)) {
					// save the popped process to its respective user queue
					sPObjPtr->vecOfUserQPtr->at(poppedProcess.getUser()).push(poppedProcess);
				}
			}
			while (!tempPriorityQueueProcessHolder.empty()) {
				sPObjPtr->priorityQStartTimePtr->push(tempPriorityQueueProcessHolder.front());
				tempPriorityQueueProcessHolder.pop();
			}

			for (int z = 0; z < tempPriorityQueueProcessHolder.size(); z++) {
			}

			// calculate the user process executionTime
			std::vector<double>executionTime;
			calcProcesTime(executionTime, *(sPObjPtr->userTimePtr), sPObjPtr->vecOfUserQPtr);

			for (int m = 0; m < sPObjPtr->vecOfUserQPtr->size(); m++) {
				for (int n = 0; n < sPObjPtr->vecOfUserQPtr->at(m).size(); n++) {
					Process * tempProcess = &(sPObjPtr->vecOfUserQPtr->at(n).front());
					sPObjPtr->vecOfUserQPtr->at(n).pop();
					// Call the thread with the temp processs
					HANDLE processThread = CreateThread(NULL, 0, createProcess, tempProcess, 0, NULL);
					WaitForSingleObject(processThread, INFINITE);
					// once the tread completes push the process back in to the queue --> if it is not done
					sPObjPtr->vecOfUserQPtr->at(n).push(*(tempProcess));

				}
			}
			int iii = 0;

		}
		// case 2 --> Second time the scheduler starts and onwards

		// increment the timer by the quantum time ---> because once a loop is done means a quantum is finished
		timer += *(sPObjPtr->quantumTimePtr);
	}
	//Process * processObj = new Process(schedulerPackageObjPtr->priorityQStartTimePtr, ;
	//Process * processObj = new Process();




	std::cout << "Scheduler Thread End" << std::endl;



	return 0;
}


void testThreadWait() {

}





void convertToPointers(std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> &priorityQueueStartTime, std::vector<std::queue<Process>> &vectorOfUserQueues, double &quantumTime, double &userTime, schedulerPackage * &schedulerPackagePtr)
{

	// get a pointer to the priority Start Time Queue
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> * priorityQStartTimePtr = &priorityQueueStartTime;;

	// get a pointer of vector of user Queues
	std::vector<std::queue<Process> > *vecOfUserQPtr = &vectorOfUserQueues;;

	// get the pointer of quantumTime
	double * quantumTimePtr = &quantumTime;

	// get the pointer of userTime
	double * userTimePtr = &userTime;



	schedulerPackagePtr = new schedulerPackage();
	schedulerPackagePtr->priorityQStartTimePtr = priorityQStartTimePtr;
	schedulerPackagePtr->vecOfUserQPtr = vecOfUserQPtr;
	schedulerPackagePtr->quantumTimePtr = quantumTimePtr;
	schedulerPackagePtr->userTimePtr = userTimePtr;
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

	//    testingPriorityQueueFunc();

		//randomNumberFileGenerator();
	//    std::cout << determineQuantum() << std::endl;

	// ===================================================

	//	// This is the comparison function for the Priority Queue
	//struct compareProcessStartTime {
	//	bool operator()(Process const &p1, Process const &p2) {
	//		// return "true" if "p1" is ordered before "p2", for example:
	//		return p1.getStartTime() > p2.getStartTime();
	//	}
	//};

	// Declaration of Main Variables needed for the Assignment
	// requires the comparator struct to be declared
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> priorityQueueStartTime;
	std::priority_queue<Process, std::vector<Process>, compareProcessDurationTime> priorityQueueDurationTime;
	std::vector<std::queue<Process> > vectorOfUserQueues;
	std::vector<std::string> vectorOfLines;
	std::vector<numberOfLinesAndFromTheLine> vectorOfLinesAndFromLines;

	double quantumTime;
	double userTime;
	std::vector<double> processTime;


	readFileToVectors(vectorOfUserQueues, vectorOfLines, vectorOfLinesAndFromLines);
	populateUserQueuesFromVectors(vectorOfUserQueues, vectorOfLines, vectorOfLinesAndFromLines);
	populatePriorityQueueFromUserQueues(vectorOfUserQueues, priorityQueueStartTime, priorityQueueDurationTime);

	// calculate the time variables needed for the user
	timeCalculation(quantumTime, userTime, vectorOfUserQueues);

	// Test the function
	// calcProcesTime(processTime, userTime, vectorOfUserQueues);

	// test priority queues
	//popFromPriorityQueue(priorityQueueStartTime);
	//popFromPriorityQueue(priorityQueueDurationTime);

	// =============== above these are file parsing to vector and Queues --> setting up functions =============



	// ============= Below this line we are setting all the variables to pointers and using pointers at all times ============

	// make a structure pointer containing all variables
	schedulerPackage * schedulerPackagePtr;

	// clear vector of user queues before creating its pointer and passing it to the scheduler
	clearingVectorOfUserQueues(vectorOfUserQueues);
	// function to convert variables to pointer 
	convertToPointers(priorityQueueStartTime, vectorOfUserQueues, quantumTime, userTime, schedulerPackagePtr);

	// crate a scheduler thread
	HANDLE schedulerThread = CreateThread(NULL, 0, scheduler, schedulerPackagePtr, 0, NULL);
	WaitForSingleObject(schedulerThread, INFINITE);



	// Thread-Ception Starts 








	std::cout << "pause... " << std::endl;
	int i = 0;
	std::cin >> i;

	return 0;
}




































