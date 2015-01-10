#include "StdAfx.h"
#include "cMainGame.h"
#include "cCube.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cCubeMan.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "iMap.h"
#include "cIndexCube.h"
#include "cMtlTex.h"
#include "cAseLoader.h"
#include "cGeometryObj.h"

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL)
	, m_pCubeMan(NULL)
	, m_pFont(NULL)
	, m_pMap(NULL)
	, m_pIndexCube(NULL)
	, m_pMesh(NULL)
	, m_pGeometryObj(NULL)
{
}


cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pGeometryObj);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCubeMan);
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pIndexCube);
	SAFE_RELEASE(m_pMesh);

	for each(auto p in m_vecGroup)
	{
		SAFE_DELETE(p);
	}

	for each(auto p in m_vecSurface)
	{
		SAFE_DELETE(p);
	}

	for each(auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}

	g_pTextureManager->Destroy();
	cDeviceManager* pDevice = cDeviceManager::GetInstance();
	pDevice->Destroy();
}

void cMainGame::Setup()
{
	m_pIndexCube = new cIndexCube;
	m_pIndexCube->Setup();
#pragma region MyRegion

	//D3DXCreateSphere(g_pD3DDevice, 3, 100, 100, &m_pMesh, NULL);
	//D3DXCreateBox(g_pD3DDevice, 3, 3, 3, &m_pMesh, NULL);
	//D3DXCreateTeapot(g_pD3DDevice, &m_pMesh, NULL);

	//D3DXIntersectTri(��1, ��2, ��3, ������ ������, ������ ����, u, v, ������ ���������κ��� ���������� �Ÿ���);
	//cObjLoader ObjLoader;
	//std::string sFolder(RESOURCE_FOLDER);
	//sFolder += std::string("obj/");
	//
	//
	//D3DXMATRIXA16 matWorld;
	//D3DXMATRIXA16 matS, matR;
	//D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	//D3DXMatrixRotationX(&matR, -D3DX_PI / 2.f);
	//matWorld = matS * matR;

	//ObjLoader.Load(m_vecGroup, sFolder, std::string("Map.obj"));
	//ObjLoader.Load(m_vecSurface, sFolder, std::string("map_surface.obj"));
	//
	//m_pMesh = ObjLoader.LoadMesh(m_vecMtlTex, sFolder, std::string("Map.obj"));

	/*cObjMap* pMap = new cObjMap;
	pMap->Load(sFolder, std::string("map_surface.obj"), &matWorld);
	m_pMap = pMap;*/

#pragma endregion

	m_pGeometryObj = new cGeometryObj;
	cAseLoader aseLoader;
	aseLoader.Load(m_pGeometryObj, "../../Resources/ase/woman/", "woman_01_all.ASE");

	m_pGrid = new cGrid;
	m_pGrid->Setup(30, 1.0f);

	m_pCubeMan = new cCubeMan;
	m_pCubeMan->Setup();

	m_pCamera = new cCamera;
	m_pCamera->Setup();
	m_pCamera->SetTarget(&m_pCubeMan->GetPosition());

	//��Ʈ ����
	D3DXFONT_DESC fd;
	ZeroMemory(&fd,sizeof(D3DXFONT_DESC));
	fd.Height			= 50;
	fd.Width			= 25;
	fd.Weight			= FW_MEDIUM;
	fd.Italic			= false;
	fd.CharSet			= DEFAULT_CHARSET;
	fd.OutputPrecision  = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily   = FF_DONTCARE;
	//strcpy_s(fd.FaceName, "�ü�ü");//�۲� ��Ÿ��
	// ttf ����ϱ�
	AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "umberto");//�۲� ��Ÿ��

	HRESULT hr = D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
	assert(S_OK == hr);

	

	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vDir = D3DXVECTOR3(1.5, -1, 1);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	stLight.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
}


void cMainGame::Update()
{
	m_pCubeMan->Update(m_pMap);
	m_pCamera->Update();
	//OutputDebugString("ASDF\n");
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		//D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);
	g_pD3DDevice->BeginScene();

	// �׸��� �׸���.
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pGrid->Render();

	m_pGeometryObj->Render();
//	//D3DXMATRIXA16 matWorld;
//	//D3DXMatrixIdentity(&matWorld);
//	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	//m_pMesh->DrawSubset(0);
//
//	//m_pIndexCube->Render();
//
//	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
//
//
//// 	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
//// 
//// 	D3DMATERIAL9 stMtl;
//// 	ZeroMemory(&stMtl, sizeof(D3DMATERIAL9));
//// 	stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
//// 	stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
//// 	stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
//// 	g_pD3DDevice->SetMaterial(&stMtl);
//// 
//// 	m_pCubeMan->Render();
//// 
//// 	D3DXMATRIXA16 matWorld;
//// 	D3DXMATRIXA16 matS, matR;
//// 	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
//// 	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.f);
//// 	matWorld = matS * matR;
//// 	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//// 
//// 	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//// 
//// 	if(GetKeyState(VK_SPACE) & 0x8000)
//// 	{
//// 		for each(auto p in m_vecSurface)
//// 		{
//// 			p->Render();
//// 		}
//// 	}
//// 	else
//// 	{
//// 		for each(auto p in m_vecGroup)
//// 		{
//// 			p->Render();
//// 		}
//// 	}
//// 	
//// 
//	/*RECT rc;
//	SetRect(&rc, 100, 100, 101, 101);
//	char szTemp[1024];
//	sprintf(szTemp, "%d / %d", dwGroup , dwMesh);
//	m_pFont->DrawTextA(NULL, szTemp, strlen(szTemp),
//		&rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255,0,0));
//*/


	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if(m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			int nX = LOWORD(lParam);
			int nY = HIWORD(lParam);
			//m_pCircle1->SetCenter(D3DXVECTOR2(nX, nY));
		}
		
		break;
	case WM_MOUSEWHEEL:
		GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
}
