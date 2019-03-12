
#include"fileParsing.h"
#include "Source.h"

// ========== Structure declarations ==============================================
struct schedulerPackage {
	std::vector<std::queue<Process>> * vecOfUserQueuePtr;
	std::priority_queue<Process, std::vector<Process>, compareProcessStartTime> * priorityQStartTimePtr;
	double * quantumTimePtr;
	double * userTimePtr;
	std::vector<double> * processTimePtr;
};

struct threadPackage {
	double * timer;
	Process * process;
};

// ========== Structure declarations ==============================================



// ========== Protyping Functions ==============================================

void createThreadPackage(threadPackage * &threadPackagePtr, Process * tempProcess, double * timer);

// ========== Protyping Functions ==============================================



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

	if (!threadPackageObj->process->isStarted()) {
		threadPackageObj->process->setStarted(true);
		std::cout << *(threadPackageObj->timer) << " " << threadPackageObj->process->getUser() << " " << threadPackageObj->process->getProcessID() << " " << "started" << std::endl;
	}

	std::cout << *(threadPackageObj->timer) << " " << threadPackageObj->process->getUser() << " " << threadPackageObj->process->getProcessID() << " " << "resumed" << std::endl;

	if (threadPackageObj->process->getExecutionTime() >= (threadPackageObj->process->getDurationTime() - threadPackageObj->process->getElapsedTime())) {

		threadPackageObj->process->setDone(true);
		threadPackageObj->process->setPaused(false);
		threadPackageObj->process->setUnusedTime(threadPackageObj->process->getExecutionTime() - (threadPackageObj->process->getDurationTime() - threadPackageObj->process->getElapsedTime()));
		threadPackageObj->process->setElapsedTime(threadPackageObj->process->getExecutionTime() - threadPackageObj->process->getUnusedTime() );

		*(threadPackageObj->timer) = *(threadPackageObj->timer) +
			(threadPackageObj->process->getExecutionTime() - (threadPackageObj->process->getDurationTime() - threadPackageObj->process->getElapsedTime()));

		std::cout << *(threadPackageObj->timer) << " " << threadPackageObj->process->getUser() << " " << threadPackageObj->process->getProcessID() << " " << "finished" << std::endl;


		threadPackageObj->process->setExecutionTime(0);

	}

	else {
		threadPackageObj->process->setDone(false);
		threadPackageObj->process->setPaused(true);
		threadPackageObj->process->setUnusedTime(0);
		threadPackageObj->process->setElapsedTime(threadPackageObj->process->getExecutionTime());

		*(threadPackageObj->timer) = *(threadPackageObj->timer) +
			(threadPackageObj->process->getExecutionTime());

		std::cout << *(threadPackageObj->timer) << " " << threadPackageObj->process->getUser() << " " << threadPackageObj->process->getProcessID() << " " << "paused" << std::endl;


		threadPackageObj->process->setExecutionTime(0);

	}

	return 0;
}


DWORD WINAPI scheduler(LPVOID lpParam) {
	schedulerPackage * sPObjPtr;
	sPObjPtr = (schedulerPackage *)lpParam;

	bool allProcessesDone = false;
	double timer = 1;
	double *timerPtr;
	double quantumTime = *(sPObjPtr->quantumTimePtr);
	int quantumCounter = 0;
	while (true) {
		quantumCounter++;
		quantumTime = quantumTime * quantumCounter;



		// making a temporary Queue
		std::queue<Process> tempPriorityQueueProcessHolder;
		
		// every process in the priority queue is checked if it has startTime < Quantum time
		for (int i = 0; i < (sPObjPtr->priorityQStartTimePtr->size()); i++) {
			
			// poping the highest priority -> eariest time
			Process poppedProcess = sPObjPtr->priorityQStartTimePtr->top();
			sPObjPtr->priorityQStartTimePtr->pop();
		
			
			if (poppedProcess.getStartTime() < quantumTime) {
				// save the popped process if it is to run during this quantum to its respective user queue
				sPObjPtr->vecOfUserQueuePtr->at(poppedProcess.getUser()).push(poppedProcess);
			}
			else {
				// push the popped process to the temporary queue !!!not the priority Queue!!!
				tempPriorityQueueProcessHolder.push(poppedProcess);
			}
		}

		while (!tempPriorityQueueProcessHolder.empty()) {
			sPObjPtr->priorityQStartTimePtr->push(tempPriorityQueueProcessHolder.front());
			tempPriorityQueueProcessHolder.pop();
		}

		// increment the timer by the quantum time ---> because once a loop is done means a quantum is finished

		for (int queues = 0; queues < sPObjPtr->vecOfUserQueuePtr->size(); queues++) {
			if (!sPObjPtr->vecOfUserQueuePtr->at(queues).empty()) {
				allProcessesDone = false;
				break;
			}
			else {
				allProcessesDone = true;
			}
		}

		// if all the processe are processed then the while loop will break; 
		if (allProcessesDone) {
			break;
		}


		// calculate the user process executionTime
		std::vector<double>executionTime;
		calcProcesTime(executionTime, *(sPObjPtr->userTimePtr), sPObjPtr->vecOfUserQueuePtr);

		int vectorQueueSize = sPObjPtr->vecOfUserQueuePtr->size();
		for (int vecQueuesPos = 0; vecQueuesPos <vectorQueueSize ; vecQueuesPos++) {
			int queueSize = sPObjPtr->vecOfUserQueuePtr->at(vecQueuesPos).size();
			for (int queuePos = 0; queuePos < queueSize; queuePos++) {
				Process * tempProcess = &(sPObjPtr->vecOfUserQueuePtr->at(queuePos).front());
				sPObjPtr->vecOfUserQueuePtr->at(queuePos).pop();

				// set execution time for every process before it is passed to the thread to run 
				tempProcess->setExecutionTime(executionTime.at(tempProcess->getUser()));

				// geting reference to the timer pointer 
				timerPtr = &timer;

				threadPackage * threadPackagePtr;
				// creating threadPackage containing process object and the timer to be passed to the createThread method
				createThreadPackage(threadPackagePtr, tempProcess, timerPtr);

				//*(threadPackagePtr->timer) = 99;

				// Call the thread with the temp processs
				HANDLE processThread = CreateThread(NULL, 0, createProcess, threadPackagePtr, 0, NULL);


				WaitForSingleObject(processThread, INFINITE);

				// once the tread completes push the process back in to the queue --> if it is not done
				if (threadPackagePtr->process->isPaused()) {
					sPObjPtr->vecOfUserQueuePtr->at(queuePos).push(*(threadPackagePtr->process));
				}
			}
		}
		int iii = 0;


	}

	std::cout << "Scheduler Thread End" << std::endl;

	return 0;
}

void createThreadPackage(threadPackage * &threadPackagePtr, Process * tempProcess, double *timer)
{
	threadPackagePtr = new threadPackage();

	threadPackagePtr->process = tempProcess;
	threadPackagePtr->timer = timer;
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
	schedulerPackagePtr->vecOfUserQueuePtr = vecOfUserQPtr;
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

	//// crate a scheduler thread
	HANDLE schedulerThread = CreateThread(NULL, 0, scheduler, schedulerPackagePtr, 0, NULL);
	WaitForSingleObject(schedulerThread, INFINITE);

	TimerPtrTestFunc();
		



	// Thread-Ception Starts 








	std::cout << "pause... " << std::endl;
	int i = 0;
	std::cin >> i;

	return 0;
}

void TimerPtrTestFunc()
{
	int timer = 0;
	int * timerPtr = &timer;

	std::cout << *(timerPtr) << std::endl;
	*(timerPtr) = 6;
	std::cout << *(timerPtr) << std::endl;
	std::cout << (timer) << std::endl;
}




































