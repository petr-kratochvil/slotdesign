#include <stdio.h>
#include <vector>
#include "Settings.h"

#ifndef STATISTICS_HPP
#define STATISTICS_HPP

// Averaging (and more statistical stuff) of one value
class StatItem
{/*pozor zmena nazvu*/
	long long sumData; // sum of data
	long long sumDataSquared; // sum of (data squared)
	int count; // data items count
	FILE* fw; // file to write stats to
	static FILE* fwCommon; // file to write final formatted output to
	std::vector<long long> data;
	wchar_t itemDesc[200];
	char itemName[100];
public:
	StatItem(const char* itemName, const wchar_t* itemDesc);
	~StatItem();
	void printToFile();
	void addData(int d);
	double getAvg() const; // data average
	double getVar() const; // data variance
	long long getTotal() const;
	int getCount() const;
	void printFinalFormatted() const;
	static void printFinalFormattedBegin(const char* fileName);
	static void printFinalFormattedEnd();
};

#endif