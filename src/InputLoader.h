#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include "resource.h"
#include "Settings.h"
#include "WinGlobal.h"
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
	InputFile();
	~InputFile();
	void loadFile(char* fileName);
	virtual int getInt();
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
	InputResource();
	~InputResource();
	void loadResource(int resourceID);
	int getInt();
};

class InputLoader
{
public:
	static Input* open(char* name);
	static Input* open(int resourceID);
	static void close(Input* f);
};

#endif