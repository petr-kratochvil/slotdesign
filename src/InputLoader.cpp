#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include "InputLoader.h"
#include "resource.h"
#include "Settings.h"

#if (INPUT_TYPE == INPUT_TYPE_RESOURCE)
#include "WinGlobal.h"
#endif

#include <sstream>

InputFile::InputFile()
	: fr(NULL)
{}

InputFile::~InputFile()
{
	fclose(this->fr);
}

void InputFile::loadFile(char* fileName)
{
	char filePath[512];
	strcpy(filePath, Settings::pathInputs);
	strcat(filePath, fileName);
	this->fr = fopen(filePath, "r");
	assert(fr != NULL);
}
int InputFile::getInt()
{
	int result;
	int cnt = fscanf(this->fr, "%d", &result);
	assert(cnt == 1);
	return result;
}

#if (INPUT_TYPE == INPUT_TYPE_RESOURCE)
InputResource::InputResource()
	: buff(NULL)
{}
InputResource::~InputResource()
{
	delete this->cppstring;
	delete this->stream;
}
void InputResource::loadResource(int resourceID)
{
	hResInfo = FindResource(WinGlobal::hInst, MAKEINTRESOURCE(resourceID), L"TEXT");
	assert(hResInfo != NULL);
	hResource = LoadResource(WinGlobal::hInst, hResInfo);
	assert(hResource != NULL);
	this->buff = (char*)LockResource(hResource);
	this->cppstring = new std::string(buff);
	this->stream = new std::stringstream(*this->cppstring);
}
int InputResource::getInt()
{
	int result;
	*this->stream >> result;
	return result;
}
#endif

Input* InputLoader::open(char* name)
{
	Input* input = new InputFile();
	input->loadFile(name);
	return input;
}

#if (INPUT_TYPE == INPUT_TYPE_RESOURCE)
Input* InputLoader::open(int resourceID)
{
	Input* input = new InputResource();
	input->loadResource(resourceID);
	return input;
}
#endif

void InputLoader::close(Input* f)
{
	delete f;
}