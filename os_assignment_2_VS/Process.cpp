#include "Process.h"

Process::~Process() {

}

double Process::getExecutionTime() const {
	return executionTime;
}

void Process::setExecutionTime(double executionTime) {
	Process::executionTime = executionTime;
}

double Process::getUserTime() const {
	return userTime;
}

void Process::setUserTime(double userTime) {
	Process::userTime = userTime;
}

int Process::getUser() const {
	return user;
}

void Process::setUser(int user) {
	Process::user = user;
}

int Process::getProcessID() const {
	return processID;
}

void Process::setProcessID(int processID) {
	Process::processID = processID;
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

bool Process::isDone() const {
	return done;
}

void Process::setDone(bool val) {
	Process::done = val;
}

bool Process::isStarted() const {
	return started;
}

void Process::setStarted(bool val) {
	Process::started = val;
}

bool Process::isPaused() const {
	return paused;
}

void Process::setPaused(bool val) {
	Process::paused = val;
}

double Process::getUnusedTime() const {
	return unusedTime;
}

void Process::setUnusedTime(double unusedTime) {
	Process::unusedTime = unusedTime;
}


//Process::Process(double startTime, double executionTime, double elapsedTime, double durationTime) 
//	: startTime(startTime), executionTime(executionTime), elapsedTime(elapsedTime), durationTime(durationTime) {
//	Process::currentThreadHandle = NULL;
//	// no logic
//
//}
//
//Process::Process(double startTime, double durationTime) : startTime(startTime), durationTime(durationTime) {
//	Process::currentThreadHandle = NULL;
//	// no logic
//
//}

Process::Process() {
	Process::startTime = 0;
	Process::executionTime = 0;
	Process::userTime = 0;
	Process::elapsedTime = 0;
	Process::durationTime = 0;


	Process::done = false;
	Process::started = false;
	Process::paused = false;

	Process::unusedTime = 0;

	Process::processID = -1;
	Process::user = -1;

	Process::currentThreadHandle = NULL;
}

// copy constructor
Process::Process(const Process &P1) {
	Process::startTime = P1.getStartTime();
	Process::executionTime = P1.getExecutionTime();
	Process::userTime = P1.getUserTime();
	Process::elapsedTime = P1.getElapsedTime();
	Process::durationTime = P1.getDurationTime();

	Process::done = P1.isDone();
	Process::started = P1.isStarted();
	Process::paused = P1.isPaused();

	Process::unusedTime = P1.getUnusedTime();

	Process::user = P1.getUser();
	Process::processID = P1.getProcessID();

}