#pragma once
class ParseDex
{
public:
	char* getDexFile()
	{
		return dexFile;
	}
	void setDexFile(char* buffer)
	{
		dexFile = buffer;
	}

	/*
	 * ��Dex�ļ�
	 */
	char* openDexFile(char*fileName);

	void closeDexFile();
	

private:
	char* dexFile;

};

