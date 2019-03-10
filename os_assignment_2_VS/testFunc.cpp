//// DWORD has to be the type of the function -> WhY? 
//// The parameter of the func
//DWORD WINAPI testThreadFunc2(LPVOID lpParam) {
//	threadData * dataPtr = new threadData();
//	dataPtr = (threadData*)lpParam;
//	while (TRUE) {
//		std::cout << "Test func 2 " << dataPtr->getProcessTime() << std::endl;
//	}
//
//	//HANDLE testHandle = GetCurrentThread();
//	//SuspendThread(testHandle);
//
//	return 0;
//}
//
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
//
//	HANDLE testHandle = GetCurrentThread();
//	SuspendThread(testHandle);
//
//	std::cout << "Test func after suspend " << dataPtr->getProcessTime() << std::endl;
//
//	return 0;
//}
//
//void learningFunction() {
//
//	HANDLE myThreadHandle1;
//	HANDLE myThreadHandle2;
//
//	threadData * obj1;
//	obj1 = new threadData(8.5);
//
//	threadData * obj2;
//	obj2 = new threadData(105);
//
//	// ========================================================
//	// Thread Creation 
//	myThreadHandle1 = CreateThread(NULL, 0, testThreadFunc1, obj1, 0, NULL);
//
//	//SuspendThread(myThreadHandle1);
//
//	//WaitForSingleObject(myThreadHandle1, INFINITE);
//
//
//
//
//	//myThreadHandle2 = CreateThread(NULL, 0, testThreadFunc2, obj2, 0, NULL);
//
//	//SuspendThread(myThreadHandle2);
//
//
//
//	//HANDLE myThreadHandle = CreateThread(
//	//	NULL,                   // default security attributes
//	//	0,                      // use default stack size  
//	//	testFunc,				// thread function name
//	//	NULL,          // argument to thread function 
//	//	0,                      // use default creation flags 
//	//	&dwThreadIdArray[i]);   // returns the thread identifier 
//	// ========================================================
//
//	// ========================================================
//	// Functions similar to thread Join
//	// https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-waitforsingleobject
//	//WaitForSingleObject(myThreadHandle2, INFINITE);
//
//	// https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-waitformultipleobjects
//	//WaitForMultipleObjects()
//	// ========================================================
//
//
//	// ========================================================
//	// Function to close the handle - Not working yet
//	//CloseHandle(myThreadHandle);
//	// ========================================================
//
//
//
//	//std::cout << "Does this get called when the handle is closed" << std::endl;
//
//}