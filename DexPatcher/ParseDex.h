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
#include <Windows.h>
#define OUT

class ParseDex
{
public:

	ParseDex()
	{

	}
	ParseDex(const char* filePath)
	{
		if (!openDexFile(filePath))
		{
			int error = GetLastError();
			printf("error:%d\n", error);

		}
	}
	~ParseDex()
	{
		if (mDexBuffer != NULL)
		{
			delete mDexBuffer;
			mDexBuffer = NULL;
		}
		if (mFileName != NULL)
		{
			delete mFileName;
			mFileName = NULL;
		}
		if (mFilePath != NULL)
		{
			delete mFilePath;
			mFilePath = NULL;
		}
	}


	DexFile* getDexFile()
	{
		return mDexFile;
	}

	char* getDexBuffer()
	{
		return mDexBuffer;
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
	/// ��ȡcodeƫ��
	/// </summary>
	/// <param name="methodIdx">��������</param>
	/// <returns>ƫ��</returns>
	int getCodeOffset(int methodIdx);





private:
	/// <summary>
	/// DexFile����ָ��,ָ���mDexBuffer��ͬ���ڴ�����
	/// </summary>
	DexFile* mDexFile = NULL;

	char* mFilePath = NULL;
	char* mFileName = NULL;
	char* mDexBuffer = NULL;
	int mFileSize = 0;

	/// <summary>
	/// ��ȡmethod��Ӧ������Ϣ
	/// </summary>
	/// <param name="dexMethodId"></param>
	/// <param name="dexClassDef"></param>
	void getMethodClassDef(DexMethodId* dexMethodId, OUT DexClassDef** dexClassDef);

	/// <summary>
	/// ��string_ids���ȡ����ΪstringIdx����
	/// </summary>
	/// <param name="stringIdx">string_ids����</param>
	/// <returns>�ַ���</returns>
	char* getString(int stringIdx);

	/// <summary>
	/// ��type_ids���ȡ����ΪtypeIdx����
	/// </summary>
	/// <param name="typeIdx">type_ids����</param>
	/// <returns>type������,ֵΪstring_ids����</returns>
	int getType(int typeIdx);

	/// <summary>
	/// ����ClassDef->ClassData
	/// </summary>
	/// <param name="classData"></param>
	/// <param name="classDataItem">��������</param>
	void parseClassDataItem(u1* classData, OUT ClassDataItem* classDataItem);

};

