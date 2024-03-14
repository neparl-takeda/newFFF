/*==============================================================================

   タイトル画面処理 [Result.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "Result.h"
#include "texture.h"
#include "sprite.h"
#include "player.h"
//#include "input.h"
#include "inputx.h"
#include "sound.h"
#include "fade.h" 
#include "keyboard.h"
#include "score.h"
#include "combo.h"

#include "Config.h"
#include "UIText.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define CLEAR_TEXT_POS_Y (CENTER_Y - 250.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int g_TextureBand;//タイトル画面用テクスチャの識別子
static int g_TextureBgResult;
static int g_TextureShishamo;
static int g_TextureRankCoin;
static int g_TextureReButton;
static int g_SE_CLEAR;//タイトル用BGMの識別子
static int g_SE_FAILED;

D3DXCOLOR g_BandCol;
COIN g_Coin;

	PLAYER* pPlayerOne = GetPlayer();

float	Shishamo_pos;
float	Shishamo_spd;
float	ReButton_size;
float	ReButton_add;
float	ReBgU;
float	ReBgV;
int Addcol;
int ResultFrame;

bool	moving;

int		EnemyScore;

UIText* g_UITextClear{};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(int stagenum,int enemynum,int texnums)
{
	SCORE* pScore = GetScore();

	float criteria = 1.0f + (((float)stagenum / 10) * 2);

	EnemyScore		= enemynum;
	g_Coin.col		= D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	g_BandCol		= D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	g_Coin.TextCol	= D3DXCOLOR(1.0f, 0.6f, 0.4f, 1.0f);
	g_Coin.rad	= 0.0f;
	g_Coin.u	= 0.0f;

	g_TextureRankCoin	= LoadTexture((char*)"data/TEXTURE/Result_Ranks.png");

	g_TextureBand	  = LoadTexture((char*)"data/TEXTURE/UI_Result_Band.png");
	g_TextureReButton = LoadTexture((char*)"data/TEXTURE/UI_Buttons_B.png");

	char	file_SE_CLEAR[]  = "data\\SE\\SE_RESULT_CLEAR.wav";
	char	file_SE_FAILED[] = "data\\SE\\SE_RESULT_FAILED.wav";
	g_SE_CLEAR  = LoadSound(file_SE_CLEAR);
	g_SE_FAILED = LoadSound(file_SE_FAILED);

	ReBgU = 0.0f;
	ReBgV = 0.0f;

	if (pScore->ToResult >= 0)
	{
		g_Coin.rank = 7;
		g_BandCol	= D3DXCOLOR(0.5f, 0.2f, 0.5f, 1.0f);
	}
	if (pScore->ToResult >= 4000 * criteria)
	{
		g_Coin.rank = 6;
		g_BandCol = D3DXCOLOR(0.6f, 0.6f, 0.7f, 1.0f);
	}
	if (pScore->ToResult >= 8000 * criteria)
	{
		g_Coin.rank = 5;
		g_BandCol = D3DXCOLOR(0.9f, 0.3f, 0.1f, 1.0f);
	}
	if (pScore->ToResult >= 17000 * criteria)
	{
		g_Coin.rank = 4;
		g_BandCol = D3DXCOLOR(0.9f, 0.9f, 0.95f, 1.0f);
	}
	if (pScore->ToResult >= 27000 * criteria)
	{
		g_Coin.rank = 3;
		g_BandCol = D3DXCOLOR(0.9f, 0.8f, 0.3f, 1.0f);
	}
	if (pScore->ToResult >= 38000 * criteria)
	{
		g_Coin.rank = 2;
		g_BandCol = D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f);

		if ((pPlayerOne->C_Bad == 0)||(pPlayerOne->C_HitB==0))
		{
			g_Coin.rank = 1;
			g_BandCol = D3DXCOLOR(0.4f, 1.0f, 0.5f, 1.0f);
		}

		if ((pPlayerOne->C_Bad == 0) && (pPlayerOne->C_HitB == 0))
		{
			g_Coin.rank = 0;
			g_BandCol = D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f);
		}
	}

	//UiText Clear
	g_UITextClear = new UIText;
	g_UITextClear->Init();
	D3DXVECTOR2 uiTextPos = { CENTER_X,CLEAR_TEXT_POS_Y };
	D3DXVECTOR2 uiTextSize = { 100.0f ,100.0f };

	//死亡時
	if (pPlayerOne->dead)
	{
		//g_TextureClear = LoadTexture((char*)"data/TEXTURE/text_failed.png");
		g_TextureShishamo = LoadTexture((char*)"data/TEXTURE/Shishamo_end_3.png");
		g_Coin.TextCol = D3DXCOLOR(0.8f, 0.5f, 1.0f, 1.0f);
		g_Coin.rank	   = 7;
		g_BandCol	   = D3DXCOLOR(0.5f, 0.2f, 0.5f, 1.0f);

		g_UITextClear->SetUITextPram(uiTextPos, uiTextSize, g_Coin.TextCol, (char*)"FAILED", FONT_GRADATION);
		PlaySound(g_SE_FAILED, 0);
	}
	else
	{
		g_TextureShishamo = LoadTexture((char*)"data/TEXTURE/Shishamo_end_1.png");

		g_UITextClear->SetUITextPram(uiTextPos, uiTextSize, g_Coin.TextCol, (char*)"CLEAR", FONT_GRADATION);
		PlaySound(g_SE_CLEAR, 0);
	}

	g_Coin.alpha = 0.3f;
	g_Coin.size = D3DXVECTOR2(300.0f, 300.0f);
	g_Coin.roted = false;

	InitScoreResult(texnums, enemynum);

	Shishamo_pos = -450.0f;
	Shishamo_spd = 85.0f;
	ReButton_size = 200.0f;
	ReButton_add  = 1.0f;
	Addcol = 0;
	ResultFrame = 0;
	moving = true;


	g_TextureBgResult = LoadTexture((char*)"data/TEXTURE/Stage_Panels.png");

	if (stagenum == 0)
	{
		ReBgU = 0.0f;
	}
	else
	{
		ReBgU = (1.0f * (stagenum % 6));
	}
	if (stagenum>=6)
	{
		ReBgU += 1.0f;
		ReBgV += 1.0f;
	}
	if (stagenum >= 11)
	{
		ReBgU += 1.0f;
		ReBgV += 1.0f;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	UninitPlayer();
	UninitScore();
	UninitCombo();
	StopSound(g_SE_CLEAR);
	StopSound(g_SE_FAILED);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	CONFIG* pConfig = GetConfig();

	SetVolume(g_SE_CLEAR,  pConfig->cSoundBGMv);
	SetVolume(g_SE_FAILED, pConfig->cSoundBGMv);

	ResultFrame++;

	//エンターキーが押されたらSCENE_STAGESELECTへ移行する
	if ((Keyboard_IsKeyDown(KK_SPACE))&&(ResultFrame>=150))
	{
		SceneTransition(SCENE_STAGESELECT);
	}
	//コントローラーBボタン押したらSCENE_TITLEへ移行
	if ((IsButtonTriggered(0, XINPUT_GAMEPAD_B)) && (ResultFrame >= 150))
	{
		SceneTransition(SCENE_STAGESELECT);
	}

	//ししゃも移動
	if (moving)
	{
		Shishamo_pos += Shishamo_spd;	//スピードを足して移動
		Shishamo_spd *= 0.9f;			//スピード減衰
	}
	//移動完了
	if (Shishamo_spd <= 1.0f)
	{
		moving = false;
	}

	if (Addcol < 100)
	{
		Addcol++;
	}
	if (g_Coin.rad<=360*2-10)
	{
		g_Coin.rad+=10.0f;
	}
	else
	{
		g_Coin.roted = true;
	}

	g_Coin.col = D3DXCOLOR(0.0f + Addcol * 0.01f,
		0.0f + Addcol * 0.01f,
		0.0f + Addcol * 0.01f,
		1.0f);

	g_Coin.size.x += 8.0f;
	g_Coin.size.y += 8.0f;
	g_Coin.alpha -= 0.02f;

	if (g_Coin.alpha <= 0.0f)
	{
		g_Coin.size.x = 300.0f;
		g_Coin.size.y = 300.0f;
		g_Coin.alpha = 0.3f;
	}

	ReButton_size += ReButton_add;
	if ((ReButton_size >= 210.0f) || (ReButton_size <= 190.0f))
	{
		ReButton_add *= -1;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	//背景
	DrawSpriteColor(g_TextureBgResult, CENTER_X, CENTER_Y, SCREEN_WIDTH * 1.1f, SCREEN_WIDTH * 1.1f,
		(1.0f / 6.0f) * ReBgU, (1.0f / 3.0f) * ReBgV, (1.0f / 6.0f), (1.0f / 3.0f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	//リザルト
	DrawSpriteColor(g_TextureBand, CENTER_X, CENTER_Y - 30.0f, SCREEN_WIDTH, 600.0f,
		0.0f, 0.0f, 1.0f, 1.0f, g_BandCol);
	//シシャモ
	DrawSpriteColor(g_TextureShishamo, Shishamo_pos, CENTER_Y, 800.0f, 800.0f,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	GetDeviceContext()->PSSetShaderResources(0, 1,
		GetTexture(g_TextureRankCoin));

	DrawSpriteColorRotation(
		CENTER_X,
		CENTER_Y+25.0f,
		300,
		300,
		g_Coin.rad,
		g_Coin.col,
		(float)g_Coin.rank,
		0.125f,
		1.0f,
		8
	);

	if (g_Coin.roted)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1,
			GetTexture(g_TextureRankCoin));
		DrawSpriteColorRotation(
			CENTER_X,
			CENTER_Y+25.0f,
			(int)g_Coin.size.x,
			(int)g_Coin.size.y,
			g_Coin.rad,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Coin.alpha),
			(float)g_Coin.rank,
			0.125f,
			1.0f,
			8
		);

	}

	if (ResultFrame >= 150)
	{
		DrawSpriteColor(g_TextureReButton, 1700.0f, 950.0f, ReButton_size, ReButton_size,
			0.167f * 2.0f, 0.249f, 0.167f, 0.249f, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
	}

	DrawScore();
	DrawEnemyScore();
	DrawMaxCombo();

	g_UITextClear->Draw();
}
