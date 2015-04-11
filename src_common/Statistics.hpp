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
public:
	StatItem()
		: data(0)
		, dataSquared(0)
		, count(0)
	{}
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
	void writeToFile() const
	{
	}
};

#endif