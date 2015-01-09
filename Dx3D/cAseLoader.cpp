#include "stdafx.h"
#include "cAseLoader.h"
#include "Asciitok.h"

cAseLoader::cAseLoader()
	:m_pFile(NULL)
{
	memset(m_cstrToken, 0, sizeof(m_cstrToken));
}

cAseLoader::~cAseLoader()
{
}

//인자는 수정 할거
void cAseLoader::Load(std::string& sFolder, std::string& sFileName)
{
	errno_t file_open;
	file_open = fopen_s(&m_pFile, (sFolder + sFileName).c_str(), "r");
	assert(file_open == 0);
	{
		while (!feof(m_pFile))
		{
			char* cstrToken = GetToken();
			
			if (IsEqual(cstrToken,ID_SCENE))
			{
				SceneProc();
			}
			else if (IsEqual(cstrToken, ID_MATERIAL_LIST))
			{
				MaterialListPorc();
			}
			OutputDebugString(cstrToken);
			OutputDebugString("\n");
		}
	}
	fclose(m_pFile);
}

void cAseLoader::Load(const char* sFolder, const char* sFileName)
{
	Load(std::string(sFolder), std::string(sFileName));
}

char* cAseLoader::GetToken()
{
	//
	bool isQuote = FALSE;
	memset(m_cstrToken, 0, sizeof(m_cstrToken));	
	int nReadCount = 0;

	while (char ch = fgetc(m_pFile))
	{		
		if (ch < 33 && !isQuote)
		{			
			break;
		}
		if (ch == '\"')
		{
			isQuote = !isQuote;
			continue;
		}
		m_cstrToken[nReadCount++] = ch;
	}
	if (nReadCount == 0)
		return GetToken();

	m_cstrToken[nReadCount] = '\0';
	return m_cstrToken;
}

void cAseLoader::SkipProc()
{
	int nLevelNum = 0;
	do
	{
		char* ch = GetToken();
		if (IsEqual(ch,"{"))
		{
			nLevelNum++;
		}
		else if (IsEqual(ch, "}"))
		{
			nLevelNum--;
		}
	} while (nLevelNum > 0);
}

BOOL cAseLoader::IsEqual(char* str1, char* str2)
{	
	return strcmp(str1, str2) == 0;
}

void cAseLoader::SceneProc()
{
	SkipProc();
}

void cAseLoader::MaterialListPorc()
{
	int nLevelNum = 0;
	do
	{
		char* ch = GetToken();
		if (IsEqual(ch, "{"))
		{
			nLevelNum++;
		}
		else if (IsEqual(ch, "}"))
		{
			nLevelNum--;
		}
		else if (IsEqual(ch, ID_MATERIAL))
		{
			GetToken();
			MaterialPorc();
		}
	} while (nLevelNum > 0);
}

void cAseLoader::MaterialPorc()
{
	int nLevelNum = 0;
	do
	{
		char* ch = GetToken();
		if (IsEqual(ch, "{"))
		{
			nLevelNum++;
		}
		else if (IsEqual(ch, "}"))
		{
			nLevelNum--;
		}		
		else if (IsEqual(ch, ID_AMBIENT))
		{
			float r = (float)atof(GetToken());
			float g = (float)atof(GetToken());
			float b = (float)atof(GetToken());
		}
	} while (nLevelNum > 0);
}