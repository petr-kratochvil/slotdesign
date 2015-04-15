#include <stdio.h>
#include <assert.h>

#ifndef INPUT_LOADER_HPP
#define INPUT_LOADER_HPP

#define INPUT_TYPE_FILE 1
#define INPUT_TYPE_RESOURCE 2
#define INPUT_TYPE INPUT_TYPE_FILE

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

public:
	InputResource()
		: buff(NULL)
	{}
	void loadResource(char* name)
	{
	}
};

class InputLoader
{
public:
	static Input* open(char* name)
	{
		Input* input;
#if (INPUT_TYPE == INPUT_TYPE_FILE)
		input = new InputFile();
		input->loadFile(name);
		return input;
#elif (INPUT_TYPE == INPUT_TYPE_RESOURCE)
		input = new InputResource();
		input->loadResource(name);
#endif
		return input;
	}
	static void close(Input* f)
	{
		delete f;
	}
};

#endif