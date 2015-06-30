#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits>
#include <stdexcept>
#include <assert.h>
#include "Statistics.h"
#include "Settings.h"

StatItem::StatItem()
	: data(0)
	, dataSquared(0)
	, count(0)
	, fw(NULL)
{}
StatItem::~StatItem()
{
	if (this->fw != NULL)
		fclose(this->fw);
}
void StatItem::setOutputFile(char* fileName)
{
	assert(this->fw == NULL);
	char filePath[250];
	strcpy(filePath, Settings::pathOutputs);
	strcat(filePath, fileName);
	this->fw = fopen(filePath, "w");
}
void StatItem::printToFile() const
{
	fprintf(this->fw, "%d %d\n", this->count, this->data);
}
void StatItem::addData(int d)
{
	this->data += d;
	if (std::numeric_limits<long long>::max() - d*d <= this->dataSquared)
		throw std::runtime_error("Statistics overflow");
	this->dataSquared += d * d;
	this->count++;
}
// data average (mean value)
double StatItem::getAvg() const
{
	return double(this->data)/double(this->count);
}
// data variance
double StatItem::getVar() const
{
	// var X = E (X^2) - (E X)^2
	return double(this->dataSquared) / double(this->count-1)
			- double(this->data) * double(this->data)
			/ ( double(this->count) * double(this->count-1) );
}
double StatItem::getPct(long long base) const
{
	return 100.0 * double(this->data)/double(base);
}
long long StatItem::getTotal() const
{
	return this->data;
}
long long StatItem::getCount() const
{
	return this->count;
}
void StatItem::printFormatted(FILE* fw) const
{
	fprintf(fw, "<table border=1 cellpadding=3 cellspacing=0><tr><td></td><td><i>Value</i></td><td><i>%% of spin price</i></td></tr>"
				"<tr><td>Count:</td><td>%d</td></tr>\n", this->count);
	fprintf(fw, "<tr><td>Sum:</td><td>%llu</td></tr>\n", this->data);
	double avg = this->getAvg();
	double stddev = sqrt(this->getVar());
	double pctbase = Settings::bet;
	double stderror = stddev / sqrt(double(this->count));
	fprintf(fw, "<tr><td>Avg:</td><td>%10.3f</td><td><b>%10.3f %%</b></td></tr>\n", avg, 100.0 * avg / pctbase);
	fprintf(fw, "<tr><td>Std. dev.:</td><td>%10.3f</td><td>%10.3f %%</td></tr>\n", stddev, 100.0 * stddev / pctbase);
	fprintf(fw, "<tr><td>Std. err.:</td><td>%10.3f</td><td>%10.3f %%</td></tr>\n", stderror, 100.0 * stderror / pctbase);
	// magic number from normal distribution
	double rad = 1.96 * stderror;
	fprintf(fw, "<tr><td>95%% conf. interval:</td><td>&nbsp;</td><td><b>%10.3f %% - %10.3f %%</b></td></tr>\n", 100.0*(avg-rad)/pctbase, 100.0*(avg+rad)/pctbase);
	rad = 2.575 * stderror;
	fprintf(fw, "<tr><td>99%% conf. interval:</td><td>&nbsp;</td><td>%10.3f %% - %10.3f %%</td></tr></table>\n", 100.0*(avg-rad)/pctbase, 100.0*(avg+rad)/pctbase);
}

Statistics::Statistics()
{
	//this->statWin.setOutputFile("statWin.txt");
	this->maxWin = 0;
}
void Statistics::addWinFromOneSpin(int basicWin, int totalWin)
{
	this->statWin.addData(totalWin);
	this->statWinBasic.addData(basicWin);
	if (totalWin == 0)
		this->statWin0.addData();
	else if (totalWin <= 100)
		this->statWinU100.addData();
	else if (totalWin <= 200)
		this->statWinU200.addData();
	else
		this->statWinO200.addData();
	if (totalWin > this->maxWin)
		this->maxWin = totalWin;
}
void Statistics::printToFile() const
{
	this->statWin.printToFile();
}
void Statistics::printFinalFormatted() const
{
	char filePath[250];
	strcpy(filePath, Settings::pathOutputs);
	strcat(filePath, "Statistics.html");
	FILE* fw = fopen(filePath, "w");
	fprintf(fw
		, "<html><head><title>Statistics</title></head><body>\n"
		"<h1>Statistics</h1>\n"
		"<h2>Total win</h2>\n");
	this->statWin.printFormatted(fw);
	fprintf(fw, "<h2>Basic win</h2>\n");
	this->statWinBasic.printFormatted(fw);
	fprintf(fw, "<h2>Sevens win</h2>\n");
	this->statWin7.printFormatted(fw);
	fprintf(fw, "<h2>Star scatter win</h2>\n");
	this->statWinStar.printFormatted(fw);
	fprintf(fw, "<h2>Main reelset used</h2>\n");
	this->statReel0.printFormatted(fw);
	fprintf(fw, "</body></html>");
	fclose(fw);
}
