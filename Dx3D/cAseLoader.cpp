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
				m_vecV.clear();
				m_vecVT.clear();
				m_vecVN.clear();
				m_vecVF.clear();
				m_vecVTF.clear();
				
				m_nMtlId = -1;//mtl ref id가없으면 바패드이다.

				GeometryProc();
				
				std::vector<ST_PNT_VERTEX> vecVertex;
				vecVertex.clear();

				ST_PNT_VERTEX v;
				if (m_nMtlId != -1)
				{
					for (int i = 0,k = 0; i < m_vecVF.size(); i++)
					{
						for (int j = 0; j < 3; j++,k++)
						{
							v.p = m_vecV[m_vecVF[i][j]];
							v.t = m_vecVT[m_vecVTF[i][j]];
							v.n = m_vecVN[k];

							D3DXVec3TransformCoord(&v.p, &v.p, &m_matInverseWorldTM);
							D3DXVec3TransformNormal(&v.n, &v.n, &m_matInverseWorldTM);
							vecVertex.push_back(v);
						}
					}
					m_pNode->Setup(vecVertex,m_vecMtlTex[m_nMtlId]);
				}
				else
				{
					for (int i = 0, k = 0; i < m_vecVF.size(); i++)
					{
						for (int j = 0; j < 3; j++, k++)
						{
							v.p = m_vecV[m_vecVF[i][j]];
							v.t = D3DXVECTOR2(0, 0);
							v.n = m_vecVN[k];
							
							D3DXVec3TransformCoord(&v.p, &v.p, &m_matInverseWorldTM);
							D3DXVec3TransformNormal(&v.n, &v.n, &m_matInverseWorldTM);
							vecVertex.push_back(v);
						}
					}					
					m_pNode->Setup(vecVertex);
				}
				m_pGeometryObj->AddChild(m_strParentName, m_pNode);				
			}

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

	while (!feof(m_pFile))
	{
		char ch = fgetc(m_pFile);
		if (ch < 33 && !isQuote && nReadCount != 0)
		{			
			break;
		}
		if (ch == '\"')
		{
			isQuote = !isQuote;
			continue;
		}
		if (nReadCount == 0 && ch < 33)
			continue;

		m_cstrToken[nReadCount++] = ch;
	}

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
			char* strNodeName = GetToken();
			m_pNode->SetNodeName(strNodeName);
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

			m_matWorldTM = D3DXMATRIXA16(
				row0.x, row0.y, row0.z, 0.0f,
				row1.x, row1.y, row1.z, 0.0f,
				row2.x, row2.y, row2.z, 0.0f,
				row3.x, row3.y, row3.z, 1.0f
				);
			m_pNode->SetWorldMatrix(m_matWorldTM);//월드 현재의 로컬화된 버택스에 적용하면 모댈이 와성
			D3DXMatrixInverse(&m_matInverseWorldTM, 0, &m_matWorldTM);//버택스를 로컬화 시킨다.
			//m_pNode->SetLocalMatrix(m_matInverseWorldTM);//로컬로 만드는 로컬로 만드는 트랜스폼
			/*if (m_pPrentNode == NULL)
			{
				D3DXMATRIXA16 mat;
				D3DXMatrixIdentity(&mat);
				D3DXMatrixTranslation(&mat, 0, 1, 0);
				D3DXMatrixInverse(&m_matInversePWorldTM, 0, &mat);
			}
			else
			{
				m_matPWorldTM = m_pPrentNode->GetWorldMatrix();
				D3DXMatrixInverse(&m_matInversePWorldTM, 0, &m_matPWorldTM);
			}*/
			//matLocalTM = matInversePWorldTM * matWorldTM;
			//m_matLocalTM = m_matWorldTM * m_matInversePWorldTM;

			//m_pNode->SetLocalMatrix(m_matInverseWorldTM);
		}
		else if (IsEqual(ch, ID_MESH))
		{
			MeshProc();
		}
		else if (IsEqual(ch, ID_TM_ANIMATION))
		{
			SkipProc();
		}
		else if (IsEqual(ch, ID_MATERIAL_REF))
		{
			m_nMtlId = atoi(GetToken());
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
		else if (IsEqual(ch, ID_MESH_VERTEX_LIST))
		{
			MeshVertexListProc();
		}
		else if (IsEqual(ch, ID_MESH_FACE_LIST))
		{
			MeshFaceListProc();
		}
		else if (IsEqual(ch, ID_MESH_TVERTLIST))
		{
			MeshTVertListProc();
		}
		else if (IsEqual(ch, ID_MESH_TFACELIST))
		{
			MeshTFaceListProc();
		}
		else if (IsEqual(ch, ID_MESH_NORMALS))
		{
			MeshTNormalsProc();
		}
	} while (nLevelNum > 0);
}

void cAseLoader::MeshVertexListProc()
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
		else if (IsEqual(ch, ID_MESH_VERTEX))
		{			
			float x = 0.0f, y = 0.0f, z = 0.0f;
			GetToken();
			x = (float)atof(GetToken());
			z = (float)atof(GetToken());
			y = (float)atof(GetToken());
			
			m_vecV.push_back(D3DXVECTOR3(x, y, z));
		}
	} while (nLevelNum > 0);
}

void cAseLoader::MeshFaceListProc()
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
		else if (IsEqual(ch, ID_MESH_FACE))
		{
			std::vector<int> index;
			index.clear();
			index.resize(3);
			GetToken();//id:
			GetToken();//A:
			index[0] = atoi(GetToken());
			GetToken();//B:
			index[2] = atoi(GetToken());
			GetToken();//C:
			index[1] = atoi(GetToken());
			
			m_vecVF.push_back(index);
		}
	} while (nLevelNum > 0);
}

void cAseLoader::MeshTVertListProc()
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
		else if (IsEqual(ch, ID_MESH_TVERT))
		{
			float u = 0.0f, v = 0.0f;
			GetToken();//pass
			u = (float)atof(GetToken());
			v = (float)atof(GetToken());

			m_vecVT.push_back(D3DXVECTOR2(u, 1.0f - v));
		}
		
	} while (nLevelNum > 0);
}

void cAseLoader::MeshTFaceListProc()
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
		else if (IsEqual(ch, ID_MESH_TFACE))
		{
			std::vector<int> vtf;
			vtf.clear();
			vtf.resize(3);

			GetToken();//pass
			vtf[0] = atoi(GetToken());
			vtf[2] = atoi(GetToken());
			vtf[1] = atoi(GetToken());

			m_vecVTF.push_back(vtf);
		}

	} while (nLevelNum > 0);
}

void cAseLoader::MeshTNormalsProc()
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
		else if (IsEqual(ch, ID_MESH_VERTEXNORMAL))
		{
			D3DXVECTOR3 v0,v1,v2;
			float x0 = 0.0f, y0 = 0.0f, z0 = 0.0f;
			float x1 = 0.0f, y1 = 0.0f, z1 = 0.0f;
			float x2 = 0.0f, y2 = 0.0f, z2 = 0.0f;
			GetToken();//pass
			x0 = (float)atof(GetToken());
			z0 = (float)atof(GetToken());
			y0 = (float)atof(GetToken());
			v0 = D3DXVECTOR3(x0, y0, z0);

			GetToken();//mesh vertex noraml
			GetToken();//pass
			x2 = (float)atof(GetToken());
			z2 = (float)atof(GetToken());
			y2 = (float)atof(GetToken());
			v2 = D3DXVECTOR3(x2, y2, z2);

			GetToken();//mesh vertex noraml
			GetToken();//pass
			x1 = (float)atof(GetToken());
			z1 = (float)atof(GetToken());
			y1 = (float)atof(GetToken());
			v1 = D3DXVECTOR3(x1, y1, z1);
			D3DXVec3Normalize(&v0, &v0);
			D3DXVec3Normalize(&v1, &v1);
			D3DXVec3Normalize(&v2, &v2);

			m_vecVN.push_back(v0);
			m_vecVN.push_back(v1);
			m_vecVN.push_back(v2);
		}

	} while (nLevelNum > 0);
}