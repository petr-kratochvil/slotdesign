#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include "vs_slotview/resource.h"
#include "Settings.hpp"
#include "src_slotview/WinMain.h"
#include <sstream>

#ifndef INPUT_LOADER_HPP
#define INPUT_LOADER_HPP

#define INPUT_TYPE_FILE 1
#define INPUT_TYPE_RESOURCE 2

#ifndef INPUT_TYPE
	#define INPUT_TYPE INPUT_TYPE_FILE
#endif

#if (INPUT_TYPE == INPUT_TYPE_FILE)
	#define INPUT(x) #x ".txt"
#else
	#define INPUT(x) (x)
#endif

class Input
{
public:
	Input()
	{}
	virtual ~Input()
	{}
	virtual int getInt() = 0;
	virtual void loadFile(char*)
	{}
	virtual void loadResource(int)
	{}
};

class InputFile : public Input
{
	FILE* fr;

public:
	InputFile()
		: fr(NULL)
	{}
	~InputFile()
	{
		fclose(this->fr);
	}
	void loadFile(char* fileName)
	{
		char filePath[512];
		strcpy(filePath, Settings::pathInputs);
		strcat(filePath, fileName);
		this->fr = fopen(filePath, "r");
		assert(fr != NULL);
	}
	virtual int getInt()
	{
		int result;
		int cnt = fscanf(this->fr, "%d", &result);
		assert(cnt == 1);
		return result;
	}
};

class InputResource : public Input
{
	char* buff;
	int buffLength;
	std::string* cppstring;
	std::stringstream* stream;
	HRSRC hResInfo;
    HGLOBAL hResource;

public:
	InputResource()
		: buff(NULL)
	{}
	~InputResource()
	{
		delete this->cppstring;
		delete this->stream;
	}
	void loadResource(int resourceID)
	{
		hResInfo = FindResource(WinGlobal::hInst, MAKEINTRESOURCE(resourceID), L"TEXT");
		assert(hResInfo != NULL);
		hResource = LoadResource(WinGlobal::hInst, hResInfo);
		assert(hResource != NULL);
		this->buff = (char*)LockResource(hResource);
		this->cppstring = new std::string(buff);
		this->stream = new std::stringstream(*this->cppstring);
	}
	int getInt()
	{
		int result;
		*this->stream >> result;
		return result;
	}
};

class InputLoader
{
public:
	static Input* open(char* name)
	{
		Input* input = new InputFile();
		input->loadFile(name);
		return input;
	}
	static Input* open(int resourceID)
	{
		Input* input = new InputResource();
		input->loadResource(resourceID);
		return input;
	}
	static void close(Input* f)
	{
		delete f;
	}
};

#endif