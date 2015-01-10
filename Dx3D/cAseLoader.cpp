#include "stdafx.h"
#include "cAseLoader.h"
#include "Asciitok.h"
#include "cMtlTex.h"
#include "cMeshObj.h"
#include "cGeometryObj.h"

cAseLoader::cAseLoader()
	:m_pFile(NULL)
	, m_nMtlId(-1)
	, m_pGeometryObj(NULL)
	, m_strParentName("root")
{
	memset(m_cstrToken, 0, sizeof(m_cstrToken));
}

cAseLoader::~cAseLoader()
{
}

//인자는 수정 할거
void cAseLoader::Load(cGeometryObj* geometryhObj, std::string& sFolder, std::string& sFileName)
{	
	m_pGeometryObj = geometryhObj;
	m_strFolder = sFolder;
	errno_t		file_open;
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
			else if (IsEqual(cstrToken, ID_GEOMETRY))
			{
				cMeshObj* meshObj = new cMeshObj;
				GeometryProc(meshObj);
				m_pGeometryObj->AddChild(m_strParentName, meshObj);
			}
			/*OutputDebugString(cstrToken);
			OutputDebugString("\n");*/
		}
	}
	fclose(m_pFile);
	for (auto p : m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}
}

void cAseLoader::Load(cGeometryObj* geometryhObj, const char* sFolder, const char* sFileName)
{
	Load(geometryhObj, std::string(sFolder), std::string(sFileName));
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
	//if (nReadCount == 0)
	//	return " ";// GetToken();

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
		else if (IsEqual(ch, ID_MATERIAL_COUNT))
		{
			int mtlCnt = atoi(GetToken());
			m_vecMtlTex.resize(mtlCnt);
			
		}
		else if (IsEqual(ch, ID_MATERIAL))
		{
			m_nMtlId = atoi(GetToken());
			m_vecMtlTex[m_nMtlId] = new cMtlTex;
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
			m_vecMtlTex[m_nMtlId]->stMtl.Ambient.r = (float)atof(GetToken());
			m_vecMtlTex[m_nMtlId]->stMtl.Ambient.g = (float)atof(GetToken());
			m_vecMtlTex[m_nMtlId]->stMtl.Ambient.b = (float)atof(GetToken());
			m_vecMtlTex[m_nMtlId]->stMtl.Ambient.a = 1.0f;
		}
		else if (IsEqual(ch, ID_DIFFUSE))
		{
			m_vecMtlTex[m_nMtlId]->stMtl.Diffuse.r = (float)atof(GetToken());
			m_vecMtlTex[m_nMtlId]->stMtl.Diffuse.g = (float)atof(GetToken());
			m_vecMtlTex[m_nMtlId]->stMtl.Diffuse.b = (float)atof(GetToken());
			m_vecMtlTex[m_nMtlId]->stMtl.Diffuse.a = 1.0f;
		}
		else if (IsEqual(ch, ID_SPECULAR))
		{
			m_vecMtlTex[m_nMtlId]->stMtl.Specular.r = (float)atof(GetToken());
			m_vecMtlTex[m_nMtlId]->stMtl.Specular.g = (float)atof(GetToken());
			m_vecMtlTex[m_nMtlId]->stMtl.Specular.b = (float)atof(GetToken());
			m_vecMtlTex[m_nMtlId]->stMtl.Specular.a = 1.0f;
		}
		else if (IsEqual(ch, ID_MAP_DIFFUSE))
		{
			MapDiffusePorc();
		}
	} while (nLevelNum > 0);
}

void cAseLoader::MapDiffusePorc()
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
		else if (IsEqual(ch, ID_BITMAP))
		{
			char* fpath = GetToken();
			std::string filePath = m_strFolder + std::string(fpath);
			m_vecMtlTex[m_nMtlId]->pTex = g_pTextureManager->GetTexture(filePath);
		}		
	} while (nLevelNum > 0);
}

void cAseLoader::GeometryProc(cMeshObj* meshObj)
{
	m_strParentName = "root";
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
		else if (IsEqual(ch, ID_NODE_NAME))
		{
			meshObj->SetNodeName(GetToken());			
		}
		else if (IsEqual(ch, ID_NODE_PARENT))
		{
			m_strParentName = std::string(GetToken());
		}
		else if (IsEqual(ch, ID_NODE_TM))
		{
			NodeTMProc();
		}
		else if (IsEqual(ch, ID_MESH))
		{
			MeshProc();
		}
		
	} while (nLevelNum > 0);
}

void cAseLoader::NodeTMProc()
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
		else if (IsEqual(ch, ID_TM_ROW0))
		{
			D3DXVECTOR3 v;
			v.x = (float)atof(GetToken());
			v.y = (float)atof(GetToken());
			v.z = (float)atof(GetToken());
		}
		else if (IsEqual(ch, ID_TM_ROW1))
		{
			D3DXVECTOR3 v;
			v.x = (float)atof(GetToken());
			v.y = (float)atof(GetToken());
			v.z = (float)atof(GetToken());
		}
		else if (IsEqual(ch, ID_TM_ROW2))
		{
			D3DXVECTOR3 v;
			v.x = (float)atof(GetToken());
			v.y = (float)atof(GetToken());
			v.z = (float)atof(GetToken());
		}
		else if (IsEqual(ch, ID_TM_ROW3))
		{
			D3DXVECTOR3 v;
			v.x = (float)atof(GetToken());
			v.y = (float)atof(GetToken());
			v.z = (float)atof(GetToken());
		}

	} while (nLevelNum > 0);
}

void cAseLoader::MeshProc()
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
		else if (IsEqual(ch, /*ID_TM_ROW0*/))
		{

		}	

	} while (nLevelNum > 0);
}