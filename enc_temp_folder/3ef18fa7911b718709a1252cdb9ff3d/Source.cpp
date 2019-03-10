
#include"fileParsing.h"

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
	//Process * processObj = new Process(1, 2, 0, 4);
	//processObj = (Process*)lpParam;

	// below is good algo
	// Timer = 0;
	// process.start()
	// if(process.duraton == process.elapsed){
	//		cout << "Process paused"; 
	//		process.suspend()
	//}

	std::cout << "create process func called" << std::endl;

	return 0;
}

DWORD WINAPI schedulerFunc(LPVOID lpParam) {
	schedulerPackage * schedulerPackageObjPtr;
	schedulerPackageObjPtr = (schedulerPackage *)lpParam;

	// calculate execution time in everyloop
	// executionTime

	//while (true) {
	//	//std::cout << "Scheduler Thread called" << std::endl;
	//}
	//Process * processObj = new Process(schedulerPackageObjPtr->priorityQStartTimePtr, ;
	Process * processObj = new Process();

	HANDLE processThread = CreateThread(NULL, 0, createProcess, processObj, 0, NULL);
	WaitForSingleObject(processThread, INFINITE);


	std::cout << "Scheduler Thread called" << std::endl;



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
	calcProcesTime(processTime, userTime, vectorOfUserQueues);

	// test priority queues
	//popFromPriorityQueue(priorityQueueStartTime);
	//popFromPriorityQueue(priorityQueueDurationTime);

	// =============== above these are file parsing to vector and Queues --> setting up functions =============



	// ============= Below this line we are setting all the variables to pointers and using pointers at all times ============

	// make a structure pointer containing all variables
	schedulerPackage * schedulerPackagePtr;

	// function to convert variables to pointer 
	convertToPointers(priorityQueueStartTime, vectorOfUserQueues, quantumTime, userTime, schedulerPackagePtr);

	// crate a scheduler thread
	HANDLE schedulerThread = CreateThread(NULL, 0, schedulerFunc, schedulerPackagePtr, 0, NULL);
	WaitForSingleObject(schedulerThread, INFINITE);



	// Thread-Ception Starts 








	std::cout << "pause... " << std::endl;
	int i = 0;
	std::cin >> i;

	return 0;
}




































