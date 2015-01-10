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
	, m_pNode(NULL)
	, m_pPrentNode(NULL)
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
				m_pNode = new cMeshObj;

				GeometryProc();
				m_pGeometryObj->AddChild(m_strParentName, m_pNode);				
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
//	int n = 0;
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

void cAseLoader::GeometryProc()
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
			m_pNode->SetNodeName(GetToken());
		}
		else if (IsEqual(ch, ID_NODE_PARENT))
		{
			m_strParentName = std::string(GetToken());
			m_pPrentNode = m_pGeometryObj->GetNode(m_strParentName);
		}
		else if (IsEqual(ch, ID_NODE_TM))
		{
			D3DXVECTOR3 row0, row1, row2, row3;
			
			NodeTMProc(row0, row1, row2, row3);

			//파싱해서 받은 데이터는 월드 트렌스폼
			//W * inverse(PW) = L * PW * inverse(PW)
			D3DXMATRIXA16 matLocal;
			D3DXMATRIXA16 matWorld = D3DXMATRIXA16(
				row0.x, row0.y, row0.z, 1.0f,
				row1.x, row1.y, row1.z, 1.0f,
				row2.x, row2.y, row2.z, 1.0f,
				row3.x, row3.y, row3.z, 1.0f
				);
			if (m_pPrentNode == NULL)
			{
				D3DXMATRIXA16 mat, i_mat;
				D3DXMatrixIdentity(&mat);
				D3DXMatrixInverse(&i_mat, NULL, &mat);
				matLocal = matWorld * i_mat;
			}
			else
			{

				
			}
			/*D3DXMATRIXA16 matLocal;
			D3DXMatrixInverse(&matLocal, NULL, &matWorld);*/
		}
		else if (IsEqual(ch, ID_MESH))
		{
			MeshProc();
		}
		
	} while (nLevelNum > 0);
}

void cAseLoader::NodeTMProc(D3DXVECTOR3& row0, D3DXVECTOR3& row1, D3DXVECTOR3& row2, D3DXVECTOR3& row3)
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
			row0.x = (float)atof(GetToken());
			row0.z = (float)atof(GetToken());
			row0.y = (float)atof(GetToken());
		}
		else if (IsEqual(ch, ID_TM_ROW1))
		{
			row2.x = (float)atof(GetToken());
			row2.z = (float)atof(GetToken());
			row2.y = (float)atof(GetToken());
		}
		else if (IsEqual(ch, ID_TM_ROW2))
		{
			row1.x = (float)atof(GetToken());
			row1.z = (float)atof(GetToken());
			row1.y = (float)atof(GetToken());
		}
		else if (IsEqual(ch, ID_TM_ROW3))
		{
			row3.x = (float)atof(GetToken());
			row3.z = (float)atof(GetToken());
			row3.y = (float)atof(GetToken());
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
		//else if (IsEqual(ch, /*ID_TM_ROW0*/))
		//{

		//}	

	} while (nLevelNum > 0);
}