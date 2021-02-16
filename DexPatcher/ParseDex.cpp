// ***********************************************************************
// Assembly         : 
// Author           : FeJQ
// Created          : 02-15-2021
//
// Last Modified By : FeJQ
// Last Modified On : 02-15-2021
// ***********************************************************************
// <copyright file="ParseDex.cpp" company="">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************


#include "ParseDex.h"
#include <iostream>
#include "Utils.h"
using namespace Utils;


/// <summary>
/// ��Dex�ļ�
/// </summary>
/// <param name="fileName">�ļ�·��.</param>
/// <returns>�Ƿ�ɹ�</returns>
bool ParseDex::openDexFile(const char* filePath)
{
	// ���ļ�
	if (!File::openFile(filePath, &mFileSize, &mDexBuffer))
	{
		return false;
	}
	this->mDexFile = (DexFile*)&mDexBuffer;

	// �����ļ�����·��
	mFilePath = (char*)malloc(MAX_PATH);
	if (mFilePath == NULL)
	{
		return false;
	}
	strcpy_s(mFilePath, MAX_PATH, filePath);

	// �������׺���ļ���
	mFileName = (char*)malloc(MAX_PATH);
	if (mFileName == NULL)
	{
		return false;
	}
	File::getFileName(mFilePath, mFileName);

	return true;
}

/// <summary>
/// �����ļ�
/// </summary>
/// <returns>�Ƿ�ɹ�</returns>
bool ParseDex::saveDexFile()
{
	if (mDexFile == NULL)
		return false;
	// ��ȡ�򿪵�dex�ļ�·��
	char path[MAX_PATH] = { 0 };
	File::getPath(mFilePath, path);

	// ��ȡ������׺���ļ���
	char fileNameWithoutExtension[MAX_PATH] = { 0 };
	File::getFileNameWithoutExtension(mFileName, fileNameWithoutExtension);

	// ���챣���ļ���
	char fileName[MAX_PATH] = { 0 };
	sprintf_s(fileName, "%s%s_patched.dex", path, fileNameWithoutExtension);

	// �����ļ�
	Utils::File::saveFile(mDexBuffer, fileName, mFileSize);
	return false;
}

/// <summary>
/// �ر�Dex�ļ�,�����ƺ�����
/// </summary>
void ParseDex::closeDexFile()
{
	if (mFilePath != NULL)
	{
		free(mFilePath);
		mFilePath = NULL;
	}
	if (mFileName != NULL)
	{
		free(mFileName);
		mFileName = NULL;
	}
	if (mDexBuffer != NULL)
	{
		free(mDexBuffer);
		mDexBuffer = NULL;
		mDexFile = NULL;
	}
	
	
}

/// <summary>
/// ���Dex��Ч��
/// </summary>
/// <returns>�Ƿ���Ч</returns>
bool ParseDex::checkValidity()
{
	if (mDexFile == NULL)
		return false;

	// magicΪ����ֵʱ������Ч
	//"dex\n035\0"
	//"dex\n036\0"
	//"dex\n037\0"

	const u1* magic = mDexFile->pHeader->magic;
	const u1* version = &magic[4];

	if (memcmp(magic, DEX_MAGIC, 4) != 0)
	{
		return false;
	}
	if ((memcmp(version, DEX_MAGIC_VERS, 4) != 0) &&
		(memcmp(version, DEX_MAGIC_VERS_API_13, 4) != 0) &&
		(memcmp(version, DEX_MAGIC_VERS_37, 4) != 0)) {
		return false;
	}
	return true;
}



/// <summary>
/// ��ȡmethod��Ӧ������Ϣ
/// </summary>
/// <param name="dexMethodId"></param>
/// <param name="dexClassDef"></param>
void ParseDex::getMethodClassDef(DexMethodId* dexMethodId, OUT DexClassDef** dexClassDef)
{
	bool flag1 = false;
	bool flag2 = false;
	*dexClassDef = (DexClassDef*)(mDexBuffer + mDexFile->pHeader->classDefsOff);
	for (int i = 0; i < mDexFile->pHeader->classDefsSize; i++)
	{
		int type1 = getType(dexMethodId->classIdx);
		int type2 = getType((*dexClassDef)[i].classIdx);
		char* className1 = getString(type1);
		char* className2 = getString(type2);

		flag1 = false;
		flag2 = false;

		if (type1 == type2)
		{
			flag1 = true;
		}
		if (strcmp(className1, className2) == 0)
		{
			flag2 = true;
		}
		if (flag1 != flag2)
		{
			printf("flag1!=flag2 �쳣!!!");
			throw;
		}
		if (flag1 == true && flag2 == true)
		{
			*dexClassDef += i;
			break;
		}
	}
}

/// <summary>
/// ��ȡcodeƫ��
/// </summary>
/// <param name="methodIdx">��������</param>
/// <returns>ƫ��</returns>
int ParseDex::getCodeOffset(int methodIdx)
{
	// ��ȡmethodIdx��Ӧ��method_ids_item
	DexMethodId* dexMethodId = (DexMethodId*)(mDexBuffer + mDexFile->pHeader->methodIdsOff);
	dexMethodId += methodIdx;

	// Method��Ӧ��DexClassDefItem��Ϣ
	DexClassDef* dexClassDef = NULL;
	getMethodClassDef(dexMethodId, &dexClassDef);

	// ��ȡclassDataƫ�Ƶ�ַ
	u1* classData = (u1*)mDexBuffer + dexClassDef->classDataOff;
	// ��ȡClassDataItem
	ClassDataItem classDataItem;
	parseClassDataItem(classData, &classDataItem);

	int codeOffset = NULL;
	int directMethodIdx = 0;
	for (int i = 0; i < classDataItem.directMethodsSize; i++)
	{
		if (classDataItem.directMethods[i].method_idx_diff == methodIdx)
		{
			codeOffset = classDataItem.directMethods[i].code_off;
			return codeOffset;
		}
		directMethodIdx += classDataItem.directMethods[i].method_idx_diff;
	}

	int virtualMethodIdx = 0;
	for (int i = 0; i < classDataItem.virtualMethodsSize; i++)
	{
		if (classDataItem.virtualMethods[i].method_idx_diff == methodIdx)
		{
			codeOffset = classDataItem.virtualMethods[i].code_off;
			return codeOffset;;
		}
		virtualMethodIdx += classDataItem.virtualMethods[i].method_idx_diff;
	}
	if (codeOffset == NULL)
	{
		printf("δ�ҵ�������Ӧ��codeOffset");
	}
	return codeOffset;
}

/// <summary>
/// ��string_ids���ȡ����ΪstringIdx����
/// </summary>
/// <param name="stringIdx">string_ids����</param>
/// <returns>�ַ���</returns>
char* ParseDex::getString(int stringIdx)
{
	int* stringIdsOff = (int*)(mDexBuffer + mDexFile->pHeader->stringIdsOff);
	char* str = mDexBuffer + stringIdsOff[stringIdx];
	Leb128::decodeUnsignedLeb128((u1**)&str);
	return str;
}

/// <summary>
/// ��type_ids���ȡ����ΪtypeIdx����
/// </summary>
/// <param name="typeIdx">type_ids����</param>
/// <returns>type������,ֵΪstring_ids����</returns>
int ParseDex::getType(int typeIdx)
{
	int* typeIdsOff = (int*)(mDexBuffer + mDexFile->pHeader->typeIdsOff);

	int* type = typeIdsOff + typeIdx;
	return *type;
}

/// <summary>
/// ����ClassDef->ClassData
/// </summary>
/// <param name="classData"></param>
/// <param name="classDataItem">��������</param>
void ParseDex::parseClassDataItem(u1* classData, OUT ClassDataItem* classDataItem)
{
	classDataItem->staticFiledsSize = Leb128::decodeUnsignedLeb128(&classData);
	classDataItem->instanceFieldsSize = Leb128::decodeUnsignedLeb128(&classData);
	classDataItem->directMethodsSize = Leb128::decodeUnsignedLeb128(&classData);
	classDataItem->virtualMethodsSize = Leb128::decodeUnsignedLeb128(&classData);

	//����static_fields����
	EncodedField* staticField = (EncodedField*)malloc(sizeof(EncodedField) * classDataItem->staticFiledsSize);
	if (staticField == NULL)
	{
		throw "�ڴ�����ʧ��";
	}
	memset(staticField, 0, sizeof(EncodedField) * classDataItem->staticFiledsSize);
	classDataItem->staticFields = staticField;
	for (int i = 0; i < classDataItem->staticFiledsSize; i++)
	{
		staticField[i].filed_idx_diff = Leb128::decodeUnsignedLeb128(&classData);
		staticField[i].access_flags = Leb128::decodeUnsignedLeb128(&classData);
	}

	//����instance_fields����
	EncodedField* instanceField = (EncodedField*)malloc(sizeof(EncodedField) * classDataItem->instanceFieldsSize);
	if (staticField == NULL)
	{
		throw "�ڴ�����ʧ��";
	}
	memset(instanceField, 0, sizeof(EncodedField) * classDataItem->instanceFieldsSize);
	classDataItem->instanceFields = instanceField;
	for (int i = 0; i < classDataItem->instanceFieldsSize; i++)
	{
		instanceField[i].filed_idx_diff = Leb128::decodeUnsignedLeb128(&classData);
		instanceField[i].access_flags = Leb128::decodeUnsignedLeb128(&classData);
	}

	// ����ֱ�ӷ���
	EncodedMethod* directMethod = (EncodedMethod*)malloc(sizeof(EncodedMethod) * classDataItem->directMethodsSize);
	if (staticField == NULL)
	{
		throw "�ڴ�����ʧ��";
	}
	memset(directMethod, 0, sizeof(EncodedMethod) * classDataItem->directMethodsSize);
	classDataItem->directMethods = directMethod;
	for (int i = 0; i < classDataItem->directMethodsSize; i++)
	{
		directMethod[i].method_idx_diff = Leb128::decodeUnsignedLeb128(&classData);
		directMethod[i].access_flags = Leb128::decodeUnsignedLeb128(&classData);
		directMethod[i].code_off = Leb128::decodeUnsignedLeb128(&classData);
	}

	//�����鷽��
	EncodedMethod* virtualMethod = (EncodedMethod*)malloc(sizeof(EncodedMethod) * classDataItem->virtualMethodsSize);
	if (staticField == NULL)
	{
		throw "�ڴ�����ʧ��";
	}
	memset(virtualMethod, 0, sizeof(EncodedMethod) * classDataItem->virtualMethodsSize);
	classDataItem->virtualMethods = virtualMethod;
	for (int i = 0; i < classDataItem->virtualMethodsSize; i++)
	{
		virtualMethod[i].method_idx_diff = Leb128::decodeUnsignedLeb128(&classData);
		virtualMethod[i].access_flags = Leb128::decodeUnsignedLeb128(&classData);
		virtualMethod[i].code_off = Leb128::decodeUnsignedLeb128(&classData);
	}
}


