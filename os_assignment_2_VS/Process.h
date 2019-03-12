#pragma once

#include <windows.h>
#include <handleapi.h>




class Process
{
public:
	//Process(double startTime, double executionTime);

	//Process(double startTime, double executionTime, double elapsedTime, double durationTime);

	Process(const Process &P1);

	Process();

	~Process();

private:
	double startTime;
	// execution time is from the scheduler 
private:
	double executionTime;
	// how long the process needs to finish
private:
	double userTime;
private:
	double durationTime;
private:
	double elapsedTime;

private:
	bool done;
private:
	bool started;
private:
	bool paused;

private:
	double unusedTime;


private:
	int  processID;
private:
	int user;

private:
	HANDLE getCurrentThreadHandle();


public:
	double getStartTime() const;
	void setStartTime(double startTime);

public:
	double getExecutionTime() const;
	void setExecutionTime(double executionTime);
public:
	double getUserTime() const;
	void setUserTime(double userTime);

	int getUser() const;
	void setUser(int user);

	int getProcessID() const;
	void setProcessID(int processID);

	double getDurationTime() const;
	void setDurationTime(double durationTime);

	double getElapsedTime() const;
	void setElapsedTime(double elapsedTime);

	bool isDone() const;
	void setDone(bool val);

	bool isStarted() const;
	void setStarted(bool val);

	bool isPaused() const;
	void setPaused(bool val);

	double getUnusedTime() const;
	void setUnusedTime(double unusedTime);

public:
	void setCurrentThreadHandle(HANDLE currentThreadHandle);

private:
	HANDLE currentThreadHandle;

};


