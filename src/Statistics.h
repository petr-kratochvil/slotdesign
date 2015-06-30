#include <stdio.h>
#include "Settings.h"

#ifndef STATISTICS_HPP
#define STATISTICS_HPP

// Averaging (and more statistical stuff) of one value
class StatItem
{
	// sum of data
	long long data;
	// sum of (data squared)
	long long dataSquared;
	// data items count
	long long count;
	// file to write stats to
	FILE* fw;
public:
	StatItem();
	~StatItem();
	void setOutputFile(char* fileName);
	void printToFile() const;
	void addData(int d = 1);
	// data average (mean value)
	double getAvg() const;
	// data variance
	double getVar() const;
	double getPct(long long base) const;
	long long getTotal() const;
	long long getCount() const;
	void printFormatted(FILE* fw) const;
};

// Contains all global statistics from the simulation
// Evolution of stats after individual spins (to be graphed) will be written to file immediately
struct Statistics
{
	// count the total win
	StatItem statWin, statWinBasic, statWin7, statWinStar;
	StatItem statReel0;
	StatItem statWin0, statWinU100, statWinU200, statWinO200;
	int maxWin;
	// ...more statistical idicators...
	Statistics();
	void addWinFromOneSpin(int basicWin, int totalWin);
	void printToFile() const;
	void printFinalFormatted() const;
};

#endif