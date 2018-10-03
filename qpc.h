#pragma once

#include <iostream>
#include <cstdint>
#include <windows.h>

class QPC {
private:
	LARGE_INTEGER startTime, stopTime, ticks, ns, us;
	LARGE_INTEGER frequency;

public:
	QPC() {
		QueryPerformanceFrequency(&frequency);
	}
	~QPC() {}

	inline void Start() {
		QueryPerformanceCounter(&startTime);
	}

	inline void Stop() {
		QueryPerformanceCounter(&stopTime);
		ticks.QuadPart = stopTime.QuadPart - startTime.QuadPart;
		ns.QuadPart = (ticks.QuadPart * 1e9) / frequency.QuadPart;
		us.QuadPart = (ticks.QuadPart * 1e6) / frequency.QuadPart;
	}

	uint64_t GetTicks() {
		return ticks.QuadPart;
	}

	uint64_t GetUS() {
		return us.QuadPart;
	}

	uint64_t GetNS() {
		return ns.QuadPart;
	}

	void PrintAll() {
		std::cout << std::endl << "----------" << std::endl;
		std::cout << "Ticks: " << ticks.QuadPart << std::endl;
		std::cout << "ns: " << ns.QuadPart << std::endl;
		std::cout << "us: " << us.QuadPart ;
		std::cout << std::endl << "----------" << std::endl;
	}

	/*
	qpc.Start();
	// foo();
	qpc.Stop();
	qpc.PrintAll();
	*/
};