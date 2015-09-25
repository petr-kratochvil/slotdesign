#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits>
#include <stdexcept>
#include <assert.h>
#include "Statistics.h"
#include "Settings.h"

FILE* StatItem::fwCommon = NULL;

StatItem::StatItem(const char* itemName, const wchar_t* itemDesc)
	: sumData(0)
	, sumDataSquared(0)
	, count(0)
	, fw(NULL)
{
	strcpy_s(this->itemName, 100, itemName);
	wcscpy_s(this->itemDesc, 200, itemDesc);
}
StatItem::~StatItem()
{
	if (this->fw != NULL)
		fclose(this->fw);
}

void StatItem::printToFile()
{
	assert(this->fw == NULL);
	char filePath[250];
	strcpy(filePath, Settings::pathOutputs);
	strcat(filePath, this->itemName);
	strcat(filePath, ".txt");
	this->fw = fopen(filePath, "w");

	for (int i = 0; i < this->count; i++)
	{
		fprintf(this->fw, "%d %lld\n", i, this->data[i]);
	}

	fclose(this->fw);
	this->fw = NULL;
}
void StatItem::addData(int d)
{
	this->data.push_back(d);
	
	this->sumData += d;

	if (std::numeric_limits<long long>::max() - d*d <= this->sumDataSquared)
		throw std::runtime_error("Numeric overflow in StatItem::addData");
	this->sumDataSquared += d * d;
	
	this->count++;
}
// data average (mean value)
double StatItem::getAvg() const
{
	return double(this->sumData)/double(this->count);
}
// data variance
double StatItem::getVar() const
{
	// var X = E (X^2) - (E X)^2
	return double(this->sumDataSquared) / double(this->count-1)
			- double(this->sumData) * double(this->sumData)
			/ ( double(this->count) * double(this->count-1) );
}

long long StatItem::getTotal() const
{
	return this->sumData;
}
int StatItem::getCount() const
{
	return this->count;
}
void StatItem::printFinalFormatted() const
{
	fwprintf(this->fwCommon, L"<h2>%s</h2>\n", this->itemDesc);
	fwprintf(this->fwCommon, L"<table border=1 cellpadding=4 cellspacing=0><tr><td></td><td><i>Value</i></td><td><i>%% of spin price</i></td></tr>"
				L"<tr><td>Count:</td><td>%d</td></tr>\n", this->count);
	fwprintf(this->fwCommon, L"<tr><td>Sum:</td><td>%llu</td></tr>\n", this->sumData);
	double avg = this->getAvg();
	double stddev = sqrt(this->getVar());
	double pctbase = Settings::bet;
	double stderror = stddev / sqrt(double(this->count));
	fwprintf(this->fwCommon, L"<tr><td>Avg:</td><td>%10.3f</td><td><b>%10.2f %%</b></td></tr>\n", avg, 100.0 * avg / pctbase);
	fwprintf(this->fwCommon, L"<tr><td>Std. dev.:</td><td>%10.3f</td><td>%10.2f %%</td></tr>\n", stddev, 100.0 * stddev / pctbase);
	fwprintf(this->fwCommon, L"<tr><td>Std. err.:</td><td>%10.3f</td><td>%10.2f %%</td></tr>\n", stderror, 100.0 * stderror / pctbase);
	// magic number from normal distribution
	double rad = 1.96 * stderror;
	fwprintf(this->fwCommon, L"<tr><td>95%% conf. interval:</td><td>%10.3f - %10.3f</td><td><b>%10.2f %% - %10.2f %%</b></td></tr>\n"
			, avg-rad, avg+rad, 100.0*(avg-rad)/pctbase, 100.0*(avg+rad)/pctbase);
	rad = 2.575 * stderror;
	fwprintf(this->fwCommon, L"<tr><td>99%% conf. interval:</td><td>%10.3f - %10.3f</td><td>%10.2f %% - %10.2f %%</td></tr></table>\n"
			, avg-rad, avg+rad, 100.0*(avg-rad)/pctbase, 100.0*(avg+rad)/pctbase);
}

void StatItem::printFinalFormattedBegin(const char* fileName)
{
	assert(StatItem::fwCommon == NULL);
	char filePath[250];
	strcpy(filePath, Settings::pathOutputs);
	strcat(filePath, fileName);
	strcat(filePath, ".html");
	StatItem::fwCommon = fopen(filePath, "wt,ccs=UTF-8");
	fwprintf(StatItem::fwCommon
		, L"<html><head><title>Statistika</title></head><body>\n"
		L"<h1>Statistika</h1>\n");
}

void StatItem::printFinalFormattedEnd()
{
	fwprintf(StatItem::fwCommon, L"</body></html>");
	fclose(StatItem::fwCommon);
	StatItem::fwCommon = NULL;
}
