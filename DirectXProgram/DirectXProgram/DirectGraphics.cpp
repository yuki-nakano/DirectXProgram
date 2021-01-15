#include "DirectGraphics.h"
#include "DirectInput.h"
#define _USE_MATH_DEFINES
#include <math.h>

struct XFile
{
	DWORD MaterialNum;
	LPD3DXMESH Meshes;
	LPD3DXBUFFER Materials;
	LPDIRECT3DTEXTURE9* Textures;
};
XFile g_XFile[XFileDateMax];

LPDIRECT3D9 g_Interface = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;

LPDIRECT3DTEXTURE9 g_Textures[TextureID::TexIDMAX];
LPCWSTR g_TextureNameList[] =
{
	TEXT("res/9549.jpg")
};

LPCWSTR XFileNameList[] =
{
	TEXT("Witchwait.X"),
	//	TEXT("res/9549.jpg")
};

bool InitDirectGraphics(HWND window_handle)
{
	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr)
	{
		return false;
	}

	//Deviceの情報を設定する
	D3DPRESENT_PARAMETERS parameters;

	//指定されたデータをサイズ分だけ0で初期化する
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	//バックバッファーの数
	parameters.BackBufferCount = 1;
	// バックバッファのフォーマット
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	//ウィンドウモードの指定
	parameters.Windowed = true;

	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//Zバッファの使用
	parameters.EnableAutoDepthStencil = TRUE;
	parameters.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = g_Interface->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window_handle,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&parameters,
		&g_Device
	);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ReleseDirectGraphics()
{
	if (g_Device != nullptr)
	{
		//Release ⇒　開放関数
		g_Device->Release();
		//解放後の不正アクセス対応
		g_Device = nullptr;
	}

	if (g_Interface != nullptr)
	{
		//Release ⇒　開放関数
		g_Interface->Release();
		//解放後の不正アクセス対応
		g_Interface = nullptr;
	}
}

void StartDrawing()
{
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zバッファの使用をONにする
	g_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	//バックバッファをクリア
	g_Device->Clear(
		0,
		nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 255, 255),
		1.0f,
		0
	);

	//DirectXで描画開始
	g_Device->BeginScene();
}

void FinishDrawing()
{
	//DirectXの描画終了
	g_Device->EndScene();

	g_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void DrawTriangle()
{
	CustomVertex vertices[] =
	{
		{320.0f, 0.0f, 0.0f, 1.0f, 0x000000 },
		{640.0f, 480.0f, 0.0f, 1.0f, 0x000000 },
		{0.0f, 480.0f, 0.0f, 1.0f, 0x000000 },
	};

	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		1,
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawRect(float posX, float posY, float sizeX, float sizeY, colorPattern colorpattern)
{
	DWORD Color = colorCheck(colorpattern);
	CustomVertex vertices[] =
	{
		{posX, posY, 0.0f, 1.0f, Color},
		{posX + sizeX, posY, 0.0f, 1.0f, Color},
		{posX, posY + sizeY, 0.0f, 1.0f, Color},
		{posX + sizeX, posY + sizeY, 0.0f, 1.0f, Color}
	};

	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->SetTexture(0, NULL);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2,
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawRectTexture(TextureID tex_id)
{
	if (g_Textures[tex_id] == nullptr)
	{
		return;
	}

	CustomVertex_tex vertices[] = {
		{0.0f, 0.0f, 0.0f, 1.0f, 0xffffff, 0.0f, 0.0f},
		{100.0f, 0.0f, 0.0f, 1.0f, 0xffffff, 1.0f, 0.0f},
		{100.0f, 100.0f, 0.0f, 1.0f, 0xffffff, 1.0f, 1.0f},
		{0.0f, 100.0f, 0.0f, 1.0f, 0xffffff, 0.0f, 1.0f},
	};

	g_Device->SetFVF(FVF_2D_VERTEX_TEXTURE);

	g_Device->SetTexture(0, g_Textures[tex_id]);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertices,
		sizeof(CustomVertex_tex)
	);
}

void SetUpView()
{
	D3DXMATRIX matrix;
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(0.0f, 0.0f, -150.0f);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(
		&matrix,
		&camera_pos,
		&eye_pos,
		&up_vec);

	g_Device->SetTransform(D3DTS_VIEW, &matrix);
}

void SetUpProjection()
{
	D3DXMATRIX matrix;
	float aspect = 640.0f / 480.0f;

	D3DXMatrixPerspectiveFovLH(
		&matrix,
		D3DXToRadian(60.0f),
		aspect,
		0.1f,
		1000.0f);

	g_Device->SetTransform(D3DTS_PROJECTION, &matrix);
}

void Draw3DPorigon(float posx_, float posy_, float posz_, colorPattern colorpattern)
{
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	DWORD Color = colorCheck(colorpattern);

	CustomVertex3D vertices[]
	{
		{posx_, 0.0f, 0.0f, Color},
		{0.0f, posy_, 0.0f, Color},
		{0.0f, 0.0f, posz_, Color},
	};

	g_Device->SetFVF(FVF_3D_VERTEX);


	g_Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, vertices, sizeof(CustomVertex3D));
}

bool LoadXFile(int fileName_)
{
	HRESULT hr = D3DXLoadMeshFromX(
		XFileNameList[fileName_],
		D3DXMESH_SYSTEMMEM,
		g_Device,
		nullptr,
		&g_XFile[fileName_].Materials,
		nullptr,
		&g_XFile[fileName_].MaterialNum,
		&g_XFile[fileName_].Meshes);

	if (FAILED(hr))
	{
		return false;
	}

	g_XFile[fileName_].Textures = new LPDIRECT3DTEXTURE9[g_XFile[fileName_].MaterialNum];

	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile[fileName_].Materials->GetBufferPointer();

	for (int i = 0; i < g_XFile[fileName_].MaterialNum; i++)
	{
		if (materials[i].pTextureFilename != nullptr)
		{
			LPSTR cha_file_name = materials[i].pTextureFilename;
			wchar_t wtext[256];
			size_t ret;
			mbstowcs_s(&ret, wtext, cha_file_name, strlen(cha_file_name) + 1);
			LPWSTR file_name = wtext;

			D3DXCreateTextureFromFile(
				g_Device,
				file_name,
				&g_XFile[fileName_].Textures[i]);
		}
		else
		{
			g_XFile[fileName_].Textures[i] = nullptr;
		}
	}

	return true;
}

void DrawXFile(float theta[], float pos[], float scale[], XFileDate xfiledate_)
{
	D3DXMATRIX matrix;
	D3DXMATRIX scale_matrix;
	D3DXMATRIX trans_matrix;

	D3DXMATRIX rotate_x_matrix;
	D3DXMATRIX rotate_y_matrix;
	D3DXMATRIX rotate_z_matrix;

	D3DXMatrixIdentity(&matrix);

	D3DXMatrixScaling(&scale_matrix, scale[X], scale[Y], scale[Z]);

	D3DXMatrixRotationX(&rotate_x_matrix, D3DXToRadian(theta[X]));
	D3DXMatrixRotationY(&rotate_y_matrix, D3DXToRadian(theta[Y]));
	D3DXMatrixRotationZ(&rotate_z_matrix, D3DXToRadian(theta[Z]));

	D3DXMatrixTranslation(&trans_matrix, pos[X], pos[Y], pos[Z]);

	matrix *= scale_matrix;

	matrix *= rotate_x_matrix;
	matrix *= rotate_y_matrix;
	matrix *= rotate_z_matrix;

	matrix *= trans_matrix;

	g_Device->SetTransform(D3DTS_WORLD, &matrix);

	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile[xfiledate_].Materials->GetBufferPointer();

	for (DWORD i = 0; i < g_XFile[xfiledate_].MaterialNum; i++)
	{
		g_Device->SetMaterial(&materials[i].MatD3D);
		g_Device->SetTexture(0, g_XFile[xfiledate_].Textures[i]);
		g_XFile[xfiledate_].Meshes->DrawSubset(i);
	}
}

DWORD colorCheck(colorPattern colorpattern)
{
	switch (colorpattern)
	{
	case black:
		return 0x000000;
		break;
	case white:
		return 0xffffff;
		break;
	case red:
		return 0xff0000;
		break;
	case green:
		return 0x00ff00;
		break;
	case blue:
		return 0x0000ff;
		break;
	}
}

bool LoadTexture(TextureID tex_id)
{
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,
		g_TextureNameList[tex_id],
		&g_Textures[tex_id]
	);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void ReleaseTexture()
{
	for (int i = 0; i < TextureID::TexIDMAX; i++)
	{
		if (g_Textures[i] != nullptr)
		{
			g_Textures[i]->Release();
			g_Textures[i] = nullptr;
		}
	}
}

void ReleaseXFile(int fileName_)
{
	for (int i = 0; i < g_XFile[fileName_].MaterialNum; i++)
	{
		if (g_XFile[fileName_].Textures[i] != nullptr)
		{
			g_XFile[fileName_].Textures[i]->Release();
			g_XFile[fileName_].Textures[i] = nullptr;
		}
	}

	// テクスチャ配列も解放する
	delete[] g_XFile[fileName_].Textures;

	// マテリアルの解放
	if (g_XFile[fileName_].Materials != nullptr)
	{
		g_XFile[fileName_].Materials->Release();
		g_XFile[fileName_].Materials = nullptr;
	}

	// メッシュの解放
	if (g_XFile[fileName_].Meshes != nullptr)
	{
		g_XFile[fileName_].Meshes->Release();
		g_XFile[fileName_].Meshes = nullptr;
	}
}

void  UpdateCamera(float posx_, float posz_, float theta_)
{
	theta_ += 180.0f;

	float vec_z = cosf(theta_ / 180.0f * M_PI) * 150.0f;
	float vec_x = sinf(theta_ / 180.0f * M_PI) * 150.0f;

	D3DXMATRIX matrix;
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(vec_x + posx_, 0.0f, vec_z + posz_);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(posx_, 0.0f, posz_);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(
		&matrix,
		&camera_pos,
		&eye_pos,
		&up_vec);

	g_Device->SetTransform(D3DTS_VIEW, &matrix);

}

void Move(float* theta_, float* posx_, float* posz_)
{
	if (IsHeldKey(DIK_LEFT))
	{
		*theta_ += 0.5f;
	}
	else if (IsHeldKey(DIK_RIGHT))
	{
		*theta_ -= 0.5f;
	}
	if (IsHeldKey(DIK_UP))
	{
		*posz_ += cosf(*theta_ / 180.0f * M_PI) * 2.0f;
		*posx_ += sinf(*theta_ / 180.0f * M_PI) * 2.0f;
	}
	if (IsHeldKey(DIK_DOWN))
	{
		*posz_ -= cosf(*theta_ / 180.0f * M_PI) * 2.0f;
		*posx_ -= sinf(*theta_ / 180.0f * M_PI) * 2.0f;
	}
}
