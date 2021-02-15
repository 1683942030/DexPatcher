// ***********************************************************************
// Assembly         : 
// Author           : FeJQ
// Created          : 02-15-2021
//
// Last Modified By : FeJQ
// Last Modified On : 02-15-2021
// ***********************************************************************
// <copyright file="ParseDex.h" company="">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#pragma once
#include "DexFile.h"
#define OUT

class ParseDex
{
public:

	ParseDex()
	{

	}
	ParseDex(const char* filePath)
	{
		openDexFile(filePath);
	}


	DexFile* getDexFile()
	{
		return mDexFile;
	}
	void setDexFile(DexFile* buffer)
	{
		mDexFile = buffer;
	}


	/// <summary>
	/// ��Dex�ļ�
	/// </summary>
	/// <param name="fileName">�ļ�·��.</param>
	/// <returns>�Ƿ�ɹ�</returns>
	bool openDexFile(const char* filePath);

	/// <summary>
	/// �����ļ�
	/// </summary>
	/// <returns>�Ƿ�ɹ�</returns>
	bool saveDexFile();

	/// <summary>
	/// �ر�Dex�ļ�,�����ƺ�����
	/// </summary>
	void closeDexFile();

	/// <summary>
	/// ���Dex��Ч��
	/// </summary>
	/// <returns>�Ƿ���Ч</returns>
	bool checkValidity();

	/// <summary>
	/// �޸�Dex Magic��Ϣ
	/// </summary>
	void fixDexMagic();


	void parseDexClassDef();

	void parseDexMethod(int methodIdx);

	void fixMethod(int methodIdx, char* codeItem,int codeItemLength);

	int getCodeOffset(ClassDataItem* classDataItem,int methodIdx);

	char* getString(int stringIdx);
	int getType(int typeIdx);

	
	void parseClassDataItem(u1* classData, OUT ClassDataItem* classDataItem);



private:
	/// <summary>
	/// The dex file
	/// </summary>
	DexFile* mDexFile = NULL;

	char* mFilePath = NULL;
	char* mFileName = NULL;
	char* mDexBuffer = NULL;
	int mFileSize = 0;

};

