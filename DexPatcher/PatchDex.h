#pragma once

#include "ParseDex.h"
#include <string>
#include <vector>
using namespace std;
#define override 

struct MethodInfo
{
	string methodName;
	int methodIndex;
	int methodOffset;
	int codeItemLength;
	// 16byte+ָ���+try��
	string codeItem;
};

class PatchDex :public ParseDex
{
public:
	PatchDex(string dexFilePath) :ParseDex(dexFilePath.c_str())
	{
	}
	~PatchDex()
	{
		for (int i = 0; i < methods.size(); i++)
		{
			MethodInfo* method = methods[i];
			delete method;
		}
	}

	/// <summary>
	/// �޸�Dex Magic��Ϣ
	/// </summary>
	override void fixDexMagic(bool isSave);

	/// <summary>
	/// �޸�����ȡ�ĺ���
	/// </summary>
	/// <param name="methodInfoPath">The method information path.</param>
	override void fixMethod(string methodInfoPath);
	 
	// ���ڴ洢dump�����Ĵ�����Ϣ
	vector< MethodInfo*> methods;

	/// <summary>
	/// ����dump�����ĺ�����Ϣ
	/// ����������Ϣ��ӵ�methods
	/// </summary>
	/// <param name="methodInfoPath">The method information path.</param>
	void parseMethodInfo(string methodInfoPath);
};

