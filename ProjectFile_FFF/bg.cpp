/*==============================================================================
	
	bg.cpp

--------------------------------------------------------------------------------

==============================================================================*/
#include "bg.h"
#include "tuto.h"
#include "texture.h"
#include "sprite.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX 4	//必要な頂点の数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int g_TextureBg;	//背景テクスチャ
static int g_TextureUIBack;
static int g_TextureUICtrl;
static int g_TextureUICtrlKey;
 
bool  tutoFlag;

BG g_Bg[2];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBG(int StageNum)
{
	tutoFlag = false;
	ID3D11Device *pDevice = GetDevice();

	for (int i = 0 ; i < 2 ; i++)
	{
		g_Bg[i].spdY	= 0.25f;
		g_Bg[i].rot		= 0.0f;
		g_Bg[i].rotAdd	= 0.0f;
		g_Bg[i].use		= true;
	}
	g_Bg[0].pos		= D3DXVECTOR2(CENTER_X, CENTER_Y);
	g_Bg[0].size	= D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT * 2.0f);
	g_Bg[1].pos		= D3DXVECTOR2(CENTER_X, CENTER_Y + SCREEN_HEIGHT * 2.0f);
	g_Bg[1].size	= D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT * 2.0f);

	g_TextureBg			= LoadTexture((char*)"data/TEXTURE/BG_River.png");
	g_TextureUIBack		= LoadTexture((char*)"data/TEXTURE/UI_Back_B.png");
	g_TextureUICtrl		= LoadTexture((char*)"data/TEXTURE/UI_Ctrl_C.png");
	g_TextureUICtrlKey	= LoadTexture((char*)"data/TEXTURE/UI_Ctrl_A.png");
	
	switch (StageNum)
	{
	default:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/BG_River.png");
		break;

	case 0:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_Tuto_large.png");
		tutoFlag = true;
		break;

	case 1:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_SeaCity_large.png");
		break;

	case 2:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_River_large.png");
		break;

	case 3:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_SeaRock_large.png");
		break;

	case 4:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/BG_Bubbles.png");
		break;

	case 5:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_River_large.png");
		break;

	case 6:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_River_large.png");
		break;

	case 7:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_SeaCity_large.png");
		break;

	case 8:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_SeaRock_large.png");
		break;

	case 9:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/BG_Bubbles.png");
		break;

	case 10:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/BG_Space_Large.png");
		g_Bg[0].pos		= D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT);
		g_Bg[0].size	= D3DXVECTOR2(SCREEN_WIDTH * 4.25f, SCREEN_HEIGHT * 4.25f);
		g_Bg[0].spdY	= 0.0f;
		g_Bg[0].rotAdd  = 0.01f;
		g_Bg[1].use		= false;
		break;

	case 11:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_Elevator_large.png");
		break;

	case 12:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_River_large.png");
		break;

	case 13:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_SeaRock_large.png");
		break;

	case 14:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/BG_Bubbles.png");
		break;

	case 15:
		g_TextureBg		= LoadTexture((char*)"data/TEXTURE/Bg_River_large.png");
		break;

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBG(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBG(void)
{
	g_Bg[0].rot += g_Bg[0].rotAdd;

	for (int i = 0; i < 2; i++)
	{
		if (g_Bg[i].use)
		{
			g_Bg[i].pos.y -= g_Bg[i].spdY;

			if (g_Bg[i].pos.y <= -SCREEN_HEIGHT*2.0f)
			{
				g_Bg[i].pos.y = SCREEN_HEIGHT * 2.0f;
			}

		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
{
	T_CIRCLE* pTuto = GetTuto();

	for (int i = 0; i < 2; i++)
	{
		if (g_Bg[i].use)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1,
				GetTexture(g_TextureBg));
			DrawSpriteColorRotation(
				g_Bg[i].pos.x, g_Bg[i].pos.y, 
				(int)g_Bg[i].size.x, 
				(int)g_Bg[i].size.y,
				g_Bg[i].rot,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f, 1.0f, 1.0f, 1);
		}
	}

	DrawSpriteColor(g_TextureUIBack, 0.0f + 200.0f, CENTER_Y, 400.0f, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
	DrawSpriteColor(g_TextureUIBack, SCREEN_WIDTH - 200.0f, CENTER_Y, 400.0f, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
	
	//チュートリアル時
	if (tutoFlag)
	{
		if (GetCtrl())
		{
			DrawSpriteColor(g_TextureUICtrlKey, 0.0f + 200.0f, CENTER_Y + 25.0f - ((350.0f * 0.8f) / 3.0f), 500.0f * 0.8f, (350.0f * 0.8f) / 3.0f,
				0.0f, (1.0f / 4.0f) * 3.0f, 1.0f, (1.0f / 4.0f), D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

			if (pTuto->Phase >= 7)
			{
				DrawSpriteColor(g_TextureUICtrlKey, 0.0f + 200.0f, CENTER_Y - 250.0f, 500.0f * 0.8f, (350.0f * 0.8f) / 3.0f,
					0.0f, (1.0f / 4.0f) * 1.0f, 1.0f, (1.0f / 4.0f), D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
			}
			if (pTuto->Phase >= 12)
			{
				DrawSpriteColor(g_TextureUICtrlKey, 0.0f + 200.0f, CENTER_Y - 250.0f - ((350.0f * 0.8f) / 3.0f), 500.0f * 0.8f, (350.0f * 0.8f) / 3.0f,
					0.0f, (1.0f / 4.0f) * 0.0f, 1.0f, (1.0f / 4.0f), D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
			}
			if (pTuto->Phase >= 17)
			{
				DrawSpriteColor(g_TextureUICtrlKey, 0.0f + 200.0f, CENTER_Y - 250.0f + ((350.0f * 0.8f) / 3.0f), 500.0f * 0.8f, (350.0f * 0.8f) / 3.0f,
					0.0f, (1.0f / 4.0f) * 2.0f, 1.0f, (1.0f / 4.0f), D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
			}
		}
		else
		{
			DrawSpriteColor(g_TextureUICtrl, 0.0f + 200.0f, CENTER_Y + 25.0f - ((350.0f * 0.8f) / 3.0f), 500.0f * 0.8f, (350.0f * 0.8f) / 3.0f,
				0.0f, (1.0f / 4.0f) * 3.0f, 1.0f, (1.0f / 4.0f), D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

			if (pTuto->Phase >= 7)
			{
				DrawSpriteColor(g_TextureUICtrl, 0.0f + 200.0f, CENTER_Y - 250.0f, 500.0f * 0.8f, (350.0f * 0.8f) / 3.0f,
					0.0f, (1.0f / 4.0f) * 1.0f, 1.0f, (1.0f / 4.0f), D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
			}
			if (pTuto->Phase >= 12)
			{
				DrawSpriteColor(g_TextureUICtrl, 0.0f + 200.0f, CENTER_Y - 250.0f - ((350.0f * 0.8f) / 3.0f), 500.0f * 0.8f, (350.0f * 0.8f) / 3.0f,
					0.0f, (1.0f / 4.0f) * 0.0f, 1.0f, (1.0f / 4.0f), D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
			}
			if (pTuto->Phase >= 17)
			{
				DrawSpriteColor(g_TextureUICtrl, 0.0f + 200.0f, CENTER_Y - 250.0f + ((350.0f * 0.8f) / 3.0f), 500.0f * 0.8f, (350.0f * 0.8f) / 3.0f,
					0.0f, (1.0f / 4.0f) * 2.0f, 1.0f, (1.0f / 4.0f), D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
			}
		}

	}
	else
	{
		if (GetCtrl())
		{
			DrawSpriteColor(g_TextureUICtrlKey, 0.0f + 200.0f, CENTER_Y - 250.0f, 500.0f * 0.8f, 350.0f * 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
		}
		else
		{
			DrawSpriteColor(g_TextureUICtrl, 0.0f + 200.0f, CENTER_Y - 250.0f, 500.0f * 0.8f, 350.0f * 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
		}

	}
}
