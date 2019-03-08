#pragma once

#include <windows.h>
#include <handleapi.h>

class Process
{
public:
	Process(double startTime, double executionTime);

	Process(double startTime, double executionTime, double elapsedTime);

	Process();

	~Process();

private:
	double startTime;
private:
	double executionTime;
private:
	double elapsedTime;
private:
	HANDLE getCurrentThreadHandle();


public:
	double getStartTime() const;
	void setStartTime(double startTime);

public:
	double getExecutionTime() const;
	void setExecutionTime(double executionTime);

	double getElapsedTime() const;
	void setElapsedTime(double elapsedTime);

public:
	void setCurrentThreadHandle(HANDLE currentThreadHandle);

private:
	HANDLE currentThreadHandle;

};


