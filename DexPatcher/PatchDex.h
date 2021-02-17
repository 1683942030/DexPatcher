#pragma once

#include "ParseDex.h"
class PatchDex
{
public:

	PatchDex(ParseDex* parseDex)
	{
		this->mParseDex = parseDex;
	}
	~PatchDex()
	{
		if (mParseDex != NULL)
		{
			delete mParseDex; 
			mParseDex = NULL;
		}
		if (mMethodInfoBuffer != NULL)
		{
			delete mMethodInfoBuffer;
			mMethodInfoBuffer = NULL;
		}
	}

	/// <summary>
	/// �޸�Dex Magic��Ϣ
	/// </summary>
	void fixDexMagic();

	/// <summary>
	/// �޸�����ȡ�ĺ���
	/// </summary>
	/// <param name="methodIdx">��������</param>
	/// <param name="codeItem">The code item.</param>
	/// <param name="codeItemLength">Length of the code item.</param>
	void fixMethod(int methodIdx, char* codeItem, int codeItemLength);

	void parseMethodInfo(char* fileName);

private:
	ParseDex* mParseDex = NULL;

	char* mMethodInfoBuffer = NULL;
};

