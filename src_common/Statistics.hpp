#include <stdio.h>
#include "Settings.hpp"

#ifndef STATISTICS_HPP
#define STATISTICS_HPP

// Averaging (and more statistical stuff) of one value
class StatItem
{
	// sum of data
	long data;
	// sum of (data squared)
	long dataSquared;
	// data items count
	long count;
	// file to write stats to
	FILE* fw;
public:
	StatItem()
		: data(0)
		, dataSquared(0)
		, count(0)
		, fw(NULL)
	{}
	~StatItem()
	{
		if (this->fw != NULL)
			fclose(this->fw);
	}
	void setOutputFile(char* fileName)
	{
		assert(this->fw == NULL);
		char filePath[250];
		strcpy(filePath, Settings::pathOutputs);
		strcat(filePath, fileName);
		this->fw = fopen(filePath, "w");
	}
	void printToFile() const
	{
		fprintf(this->fw, "%d %d\n", this->count, this->data);
	}
	void addData(int d)
	{
		this->data += d;
		this->dataSquared += d * d;
		this->count++;
	}
	// data average (mean value)
	double getAvg() const
	{
		return double(this->data)/double(this->count);
	}
	// data variance
	double getVar() const
	{
		// var X = E (X^2) - (E X)^2
		return double(this->dataSquared) / double(this->count)
				- double(this->data) * double(this->data)
				/ ( double(this->count) * double(this->count) );
	}
	double getPct(long base) const
	{
		return 100.0 * double(this->data)/double(base);
	}
	long getTotal() const
	{
		return this->data;
	}
	long getCount() const
	{
		return this->count;
	}
};

// Contains all global statistics from the simulation
// Evolution of stats after individual spis (to be graphed) will be written to file immediately
struct Statistics
{
	// count the total win
	StatItem statWin;
	// count the percentage of individual symbols
	StatItem statSymbols[Settings::symbolCount];
	// ...more statistical idicators...
	Statistics()
	{
		this->statWin.setOutputFile("statWin.txt");
		char fileName[50];
		for (int i = 0; i < Settings::symbolCount; i++)
		{
			sprintf(fileName, "statSymbol%d.txt", i);
			this->statSymbols[i].setOutputFile(fileName);
		}
	}
	void printToFile() const
	{
		this->statWin.printToFile();
		for (int i = 0; i < Settings::symbolCount; i++)
		{
			this->statSymbols[i].printToFile();
		}
	}
};

#endif