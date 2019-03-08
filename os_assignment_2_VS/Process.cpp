#include "Process.h"

Process::~Process() {

}

double Process::getExecutionTime() const {
	return executionTime;
}

void Process::setExecutionTime(double executionTime) {
	Process::executionTime = executionTime;
}

double Process::getDurationTime() const {
	return durationTime;
}

void Process::setDurationTime(double durationTime) {
	Process::durationTime = durationTime;
}

double Process::getElapsedTime() const {
	return elapsedTime;
}

void Process::setElapsedTime(double elapsedTime) {
	Process::elapsedTime = elapsedTime;
}

HANDLE Process::getCurrentThreadHandle() {
	return currentThreadHandle;
}

void Process::setCurrentThreadHandle(HANDLE currentThreadHandle) {
	Process::currentThreadHandle = currentThreadHandle;
}

double Process::getStartTime() const {
	return startTime;
}

void Process::setStartTime(double startTime) {
	Process::startTime = startTime;
}

Process::Process(double startTime, double executionTime, double elapsedTime, double durationTime) : startTime(startTime),
executionTime(executionTime), elapsedTime(elapsedTime), durationTime(durationTime) {
	Process::currentThreadHandle = NULL;
	// no logic

}

Process::Process(double startTime, double durationTime) : startTime(startTime), durationTime(durationTime) {
	Process::currentThreadHandle = NULL;
	// no logic

}

//Process::Process(double startTime, double executionTime) : startTime(startTime), executionTime(executionTime) {
//	Process::elapsedTime = 0;
//	Process::currentThreadHandle = NULL;
//	// no logic
//}

Process::Process() {
	Process::startTime = 0;
	Process::executionTime = 0;
	Process::elapsedTime = 0;
	Process::durationTime = NULL;
	Process::currentThreadHandle = NULL;
}


Process::Process( const Process &P1) {
	Process::startTime = P1.getStartTime();
	Process::executionTime = P1.getExecutionTime();
	Process::elapsedTime = P1.getElapsedTime();
	Process::durationTime = P1.getDurationTime();
}