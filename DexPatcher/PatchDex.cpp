#include "PatchDex.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;
using namespace Utils;
using namespace std;

/// <summary>
/// �޸�Dex Magic��Ϣ
/// </summary>
void PatchDex::fixDexMagic()
{
	if (mParseDex->checkValidity())
	{
		return;
	}
	const u1* magic = mParseDex->getDexFile()->pHeader->magic;
	const u1* version = &magic[4];
	memcpy((void*)magic, DEX_MAGIC_VERS, 4);
	memcpy((void*)version, DEX_MAGIC_VERS_API_13, 4);
}

/// <summary>
/// �޸�����ȡ�ĺ���
/// </summary>
/// <param name="methodIdx">��������</param>
/// <param name="codeItem">The code item.</param>
/// <param name="codeItemLength">Length of the code item.</param>
void PatchDex::fixMethod(int methodIdx, char* codeItem, int codeItemLength)
{
	parseMethodInfo(nullptr);

	int codeOffset = mParseDex->getCodeOffset(methodIdx);
	// �������޸�
	DexCode* dexCode = (DexCode*)(mParseDex->getDexBuffer() + codeOffset);
	if (codeItemLength != dexCode->insnsSize * 2)
	{
		printf("codeItemLength != dexCode->insnsSize * 2 �쳣!!!");
		throw;
	}
	memcpy(dexCode->insns, codeItem, dexCode->insnsSize * 2);
}

void PatchDex::parseMethodInfo(char*fileName)
{
	//int fileSize = 0;
	//File::openFile("C:\\Users\\love0\\Desktop\\json.json", &fileSize, &mMethodInfoBuffer);

	json root;
	fstream file;
	file.open(fileName, ios::in);
	if (file.is_open())
	{
		file >> root;
		int count = root["count"];
		for (int i = 0; i < root["data"].size(); i++)
		{
			string name;
			root["data"][i]["name"].get_to(name);
			root["data"][i]["name"].get_to(name);
			root["data"][i]["name"].get_to(name);
			root["data"][i]["name"].get_to(name);
			root["data"][i]["name"].get_to(name);
		}
	}
	file.close();
}
