/*==============================================================================

   頂点管理 [polygon.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "sound.h"
#include "main.h"
#include "bullet.h"
#include "fade.h"
#include "rhythm.h"
#include "combo.h"
#include "score.h"
#include "keyboard.h"
#include "inputx.h"
#include "special.h"
#include <algorithm>
#include "enemy.h"
#include "effect.h"

#include "Config.h"
#include "UIText.h"

UIText* g_UITextGood{};

//*****************************************************************************
// マクロ定義
//*****************************************************************************


enum LANE		//レーンのやつ
{
	LANE_1 =-2,
	LANE_2 =-1,
	LANE_3 = 0,
	LANE_4 = 1,
	LANE_5 = 2,
	LANE_NUM,
};
enum DIRECTION	//移動の向き
{
	D_LEFT  = 0,
	D_RIGHT = 1,
	D_NONE = 2,
};
enum CHECK		//タイミング評価
{
	C_NONE	= 0,
	C_GOOD	= 1,
	C_BAD	= 2,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int g_TexturePlayer;	//テクスチャのやつ
static int g_TextureHpA;	//テクスチャのやつ
static int g_TextureHpB;	//テクスチャのやつ
static int g_TextureHpIcon; //テクスチャのやつ
static int g_TextureShield;


static int g_SE_Bullet;		//弾サウンド
static int g_SE_Damage;		//ダメージサウンド

static PLAYER g_Player;


int		fire_dist;
int		good;
int		checkFrame;
float	goodAlpha;
float	goodPosY;
float	ShieldAlpha;
bool	STICK;


float  effectPos[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,
						0.0f,0.0f,0.0f,0.0f,0.0f };

static float g_AnimeTable[4] =
{
	0.0f,
	0.125f,
	0.250f,
	0.125f,
};
//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	g_TexturePlayer = LoadTexture((char*)"data/TEXTURE/player.png");
	g_TextureHpA	= LoadTexture((char*)"data/TEXTURE/HP_player_A.png");
	g_TextureHpB	= LoadTexture((char*)"data/TEXTURE/HP_player_C.png");
	g_TextureHpIcon = LoadTexture((char*)"data/TEXTURE/HP_Icon.png");
	g_TextureShield = LoadTexture((char*)"data/TEXTURE/Shield.png");

	char	file_SE_Bullet[] = "data\\SE\\SE_bullet.wav";
	char	file_SE_Damage[] = "data\\SE\\SE_Damage_B.wav";
	g_SE_Bullet = LoadSound(file_SE_Bullet);
	g_SE_Damage = LoadSound(file_SE_Damage);
	//初期化
	g_Player.pos	= D3DXVECTOR2(PLAYER_POS_DEFAULT_X, PLAYER_POS_DEFAULT_Y);	//位置
	g_Player.oldpos = D3DXVECTOR2(PLAYER_POS_DEFAULT_X, PLAYER_POS_DEFAULT_Y);	//oldpos
	g_Player.size	= D3DXVECTOR2(PLAYER_SIZE_X,PLAYER_SIZE_Y);	//サイズ
	g_Player.speed	= D3DXVECTOR2(0.0f,	0.0f);					//スピード
	g_Player.uv		= D3DXVECTOR2(0.0f, 0.0f);					//uv

	g_Player.hp = PLAYER_HP_DEFAULT;			//HP

	g_Player.moving		= false;	//移動中フラグ
	g_Player.shooting	= false;	//移動中フラグ
	g_Player.direction	= D_NONE;	//移動方向
	g_Player.NowLane	= LANE_3;	//初期レーン
	g_Player.flag		= false;
	g_Player.dead		= false;	

	g_Player.C_Good = 0;
	g_Player.C_Bad  = 0;
	g_Player.C_HitG = 0;
	g_Player.C_HitB = 0;

	//effectPosを初期化
	for (int i = 0; i <= 9; i++)
	{
		effectPos[i] = g_Player.pos.x;
	}

	good		= C_NONE;
	checkFrame	= 0;
	goodAlpha	= 1.0f;
	goodPosY	= 0.0f;
	fire_dist	= 0;
	STICK		= false;

	ShieldAlpha = 1.0f;

	//UiText START
	g_UITextGood = new UIText;
	g_UITextGood->Init();
	D3DXVECTOR2 uiTextPos	= { NOTESLANE_POS_X, NOTESLANE_POS_Y };
	D3DXVECTOR2 uiTextSize	= { 70.0f ,70.0f };
	D3DXCOLOR   uiTextColor = { 1.0f,1.0f,1.0f,0.0f };
	g_UITextGood->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"GOOD", FONT_GRADATION);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	g_TexturePlayer = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	BULLET* pBullet = GetBullet();
	CONFIG* pConfig = GetConfig();

	SetVolume(g_SE_Bullet, pConfig->cSoundGSEv);
	SetVolume(g_SE_Damage, pConfig->cSoundGSEv);

	goodAlpha -= 0.05f;
	goodPosY  *= 0.1f;

	//残像用 effectPosを保存
	for (int i = 1; i <= 10;i++)
	{
		effectPos[10-i] = effectPos[10 - i - 1];
	}
	effectPos[0] = g_Player.pos.x;

	//体力0でゲームオーバー
	if (g_Player.hp <= 0)
	{
		g_Player.dead = true;
		if (BadEnd()) {
			SceneTransition(SCENE_RESULT);
		}
	}
	else {
		//----------------------------------------
		// コントローラー入力
		// ---------------------------------------
		if ((IsButtonTriggered(0, XINPUT_GAMEPAD_B)) && (g_Player.moving == false) && (g_Player.shooting == false))
		{
			PlayerCheck();
			if (g_Player.flag)
			{
				SetBullet(D3DXVECTOR2(g_Player.pos.x, g_Player.pos.y - g_Player.size.y / 2));
				PlaySound(g_SE_Bullet, 0);
				//g_Player.hp -= 30.0f;
			}
		
		}

		//スティックでの長押し移動を回避
		if ((GetThumbLeftX(0) > -0.2f) && (GetThumbLeftX(0) < 0.2f) && (STICK))
		{
			STICK = false;
		}

		//スティックで移動
		if ((GetThumbLeftX(0) < -0.3f) && (g_Player.NowLane >= LANE_2) && (g_Player.moving == false) && (!STICK))
		{
			STICK = true;
			g_Player.moving		= true;				//移動中
			g_Player.oldpos.x	= g_Player.pos.x;	//現在位置保存

			g_Player.speed.x	= -50.0f;			//スピードを-に
			g_Player.direction	= D_LEFT;			//左移動
			g_Player.NowLane   -= 1;				//レーン変更
			PlayerCheck();
		}
		//左
		if ((GetThumbLeftX(0) > 0.3f) && (g_Player.NowLane <= LANE_4) && (g_Player.moving == false) && (!STICK))
		{
			STICK = true;
			g_Player.moving		= true;				//移動中
			g_Player.oldpos.x	= g_Player.pos.x;	//現在位置保存

			g_Player.speed.x	= 50.0f;			//スピードを+に
			g_Player.direction	= D_RIGHT;			//右移動
			g_Player.NowLane   += 1;				//レーン変更
			PlayerCheck();
		}

		//----------------------------------------
		// キーボード入力
		// ---------------------------------------
		//弾発射
		if ((Keyboard_IsKeyDown(KK_SPACE)) && (g_Player.moving == false) && (g_Player.shooting == false))
		{
			g_Player.shooting = true;
			PlayerCheck();
			if (g_Player.flag) {
				SetBullet(D3DXVECTOR2(g_Player.pos.x, g_Player.pos.y - g_Player.size.y / 2));
				PlaySound(g_SE_Bullet, 0);
			}

		}
		if (Keyboard_IsKeyUp(KK_SPACE) && (g_Player.shooting == true)) {
			g_Player.shooting = false;
		}



		//Aキーで右移動
		if (((Keyboard_IsKeyDown(KK_A)) || (IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_LEFT))) && (g_Player.NowLane >= LANE_2) && (g_Player.moving == false))
		{
			g_Player.moving = true;				//移動中
			g_Player.oldpos.x = g_Player.pos.x;	//現在位置保存

			g_Player.speed.x = -50.0f;		//スピードを-に
			g_Player.direction = D_LEFT;		//左移動
			g_Player.NowLane -= 1;			//レーン変更
			PlayerCheck();
		}

		// Dキー で右移動
		if (((Keyboard_IsKeyDown(KK_D))|| (IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_RIGHT))) && (g_Player.NowLane <= LANE_4) && (g_Player.moving == false))
		{
			g_Player.moving = true;				//移動中
			g_Player.oldpos.x = g_Player.pos.x; //現在位置保存

			g_Player.speed.x = 50.0f;			//スピードを+に
			g_Player.direction = D_RIGHT;		//右移動
			g_Player.NowLane += 1;				//レーン変更
			PlayerCheck();
		}

		//キャラクター移動
		if ((g_Player.moving))
		{
			g_Player.pos.x += g_Player.speed.x;	//スピードを足して移動
			g_Player.speed.x *= 0.7f;			//スピード減衰
		}

		//右移動完了
		if ((g_Player.speed.x <= 1.0f) && (g_Player.direction == D_RIGHT))
		{
			g_Player.pos.x = CENTER_X + g_Player.NowLane * LANE_SIZE_X;	//レーンの中心に
			
			if (Keyboard_IsKeyUp(KK_D) && (g_Player.moving == true)) {
				g_Player.moving = false;
			}
		}
		//左移動完了
		if ((g_Player.speed.x >= -1.0f) && (g_Player.direction == D_LEFT))
		{
			g_Player.pos.x = CENTER_X + g_Player.NowLane * LANE_SIZE_X;	//レーンの中心に
			if (Keyboard_IsKeyUp(KK_A) && (g_Player.moving == true)) {
				g_Player.moving = false;
			}
		}
	}


	switch (good)
	{
	case C_GOOD:
		g_UITextGood->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.4f, goodAlpha));
		g_UITextGood->SetText((char*)"GOOD");
		break;

	case C_BAD:
		g_UITextGood->SetColor(D3DXCOLOR(0.8f, 0.5f, 1.0f, goodAlpha));
		g_UITextGood->SetText((char*)"BAD");
		break;

	default:

		break;
	}

	D3DXVECTOR2 uiTextPos = { NOTESLANE_POS_X, NOTESLANE_POS_Y - 75.0f + (goodPosY) };
	g_UITextGood->SetPosition(uiTextPos);

	//デバッグ用
	if (GetDebugFlag())
	{
		g_Player.hp = min(g_Player.hp += 1, PLAYER_HP_DEFAULT);	//デバッグ用HP自動回復
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	SPECIAL* Dsp = GetSpecial();

	//残像3描画
	DrawSpriteColor(g_TexturePlayer, effectPos[9], g_Player.pos.y, g_Player.size.x, g_Player.size.y,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

	//残像2描画
	DrawSpriteColor(g_TexturePlayer, effectPos[6], g_Player.pos.y, g_Player.size.x, g_Player.size.y,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

	//残像1描画
	DrawSpriteColor(g_TexturePlayer, effectPos[3], g_Player.pos.y, g_Player.size.x, g_Player.size.y,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

	//プレイヤー描画
	DrawSpriteColor(g_TexturePlayer, g_Player.pos.x, g_Player.pos.y, g_Player.size.x+GetGradation(), g_Player.size.y+ GetGradation(),
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//スキル:シールドの表示
	if (Dsp->get_damage_down == true)
	{
		DrawSpriteColor(g_TextureShield, g_Player.pos.x, g_Player.pos.y - 80.0f, 180.0f, 90.0f,
			0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		DrawSpriteColor(g_TextureShield, g_Player.pos.x, g_Player.pos.y - 80.0f, 180.0f + ShieldAlpha * 90.0f, 90.0f + ShieldAlpha * 90.0f,
			0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - ShieldAlpha));

		ShieldAlpha += 0.05f;
		if (ShieldAlpha>=1.0f)
		{
			ShieldAlpha = 0.0f;
		}
	}
}

void DrawHp(void)
{

	//HPバー
	DrawSpriteColor(g_TextureHpA, PLAYER_HP_POS_X, PLAYER_HP_POS_Y + ((PLAYER_HP_DEFAULT - g_Player.hp) / 0.665f), PLAYER_HP_SIZE_X, PLAYER_HP_SIZE_Y - (PLAYER_HP_DEFAULT - g_Player.hp) * 3.0f,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(0.5f, 1.0f, 0.4f, 1.0f));
	//HP下地
	DrawSpriteColor(g_TextureHpB, PLAYER_HP_POS_X, PLAYER_HP_POS_Y, PLAYER_HPB_SIZE_X, PLAYER_HPB_SIZE_Y,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//HPアイコン
	DrawSpriteColor(g_TextureHpIcon, PLAYER_HP_POS_X, PLAYER_HP_POS_Y + 490.0f, PLAYER_HP_SIZE_X+20.0f, PLAYER_HP_SIZE_X + 20.0f,
		0.5f, 0.0f, 0.5f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	g_UITextGood->Draw();
}

//=============================================================================
// ゲッター
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player;
}
//=============================================================================
// リズムチェック
//=============================================================================
void PlayerCheck(void)
{
	SPECIAL* sp = GetSpecial();

	//NOTESLANE* pRythm = GetNotesLane();
	goodAlpha = 1.0f;
	goodPosY  = 100.0f;

	//リズムOK判定
	if (GetRhythm())
	{
		good = C_GOOD;
		g_Player.C_Good++;

		if (sp->heal_up == true)
		{
			sp->heal_count++;
			g_Player.hp = min(g_Player.hp += sp->heal_count, PLAYER_HP_DEFAULT);
		}
		else
		{
			g_Player.hp = min(g_Player.hp += 1, PLAYER_HP_DEFAULT);
		}
		if (g_Player.hp > PLAYER_HP_DEFAULT)
		{
			g_Player.hp = PLAYER_HP_DEFAULT;
		}
		ComboPlus(1);
		SpecialPlus();
		ScorePlus((int)(10 * GetComboScoreUp()));
		g_Player.flag = true;
		ReleaseNotes();
		

		SetEffect(EFFECT_1, D3DXVECTOR2(CENTER_X, NOTESLANE_POS_Y), D3DXVECTOR2(300.0f, 300.0f));
	}
	else  //BAD評価
	{
		good = C_BAD;
		g_Player.C_Bad++;
		sp->heal_count = 1;
		//スペシャルの被ダメ減少
		if (sp->get_damage_down == true)
		{
			g_Player.hp -= 0.0f;
		}
		else
		{
			g_Player.hp -= 10.0f;
		}
		PlaySound(g_SE_Damage, 0);
		ResetCombo();
		g_Player.flag = false;
	}
}

