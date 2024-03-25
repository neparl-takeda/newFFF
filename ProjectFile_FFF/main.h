/*==============================================================================

   テクスチャの描画 [main.h]
                                                         Author : 
                                                         Date   : 
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "xinput.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1920.0f)						// ウインドウの幅
#define SCREEN_HEIGHT	(1080.0f)						// ウインドウの高さ

#define CENTER_X		(SCREEN_WIDTH/2)					//中心X
#define CENTER_Y		(SCREEN_HEIGHT/2)					//中心Y

#define LANE_SIZE_X		(170.0f)						//レーンの幅
#define LANE_SIZE_Y		(1080.0f)						//レーンの高さ

#define BULLETLANE_1 (SCREEN_WIDTH / 2 - LANE_SIZE_X * 2)	//レーン1
#define BULLETLANE_2 (SCREEN_WIDTH / 2 - LANE_SIZE_X)		//レーン2
#define BULLETLANE_3 (SCREEN_WIDTH / 2)						//レーン3
#define BULLETLANE_4 (SCREEN_WIDTH / 2 + LANE_SIZE_X)		//レーン4
#define BULLETLANE_5 (SCREEN_WIDTH / 2 + LANE_SIZE_X * 2)	//レーン5

#define PLAYER_SIZE_X	(100*0.6)						//プレイヤーの幅
#define PLAYER_SIZE_Y	(360*0.6)						//プレイヤーの高さ

#define PLAYER_POS_DEFAULT_X	(CENTER_X)			//プレイヤーの位置
#define PLAYER_POS_DEFAULT_Y	(CENTER_Y+380.0f)	//プレイヤーの位置

#define PLAYER_HP_DEFAULT	(300.0f)
//#define GOOD_BAD_X			(400.0f*0.7f)
//#define GOOD_BAD_Y			(100.0f*0.7f)



//ノーツの初期値
#define		NOTES_POS_X_1		(582.5f)
#define		NOTES_POS_X_2		(1337.5f)
#define		NOTES_POS_Y			(SCREEN_HEIGHT-100)
#define		NOTES_SIZE_X		(75.0f)
#define		NOTES_SIZE_Y		(75.0f)
#define		NOTES_DIST			(6)
#define		NOTES_SP			(1.5f*NOTES_DIST)
#define		NOTES_TEX_RIGHT			(((char*)"data\\texture\\notes_right.png"))
#define		NOTES_TEX_LEFT			(((char*)"data\\texture\\notes_left.png"))
#define		NOTES_TEX_CENTER		(((char*)"data\\texture\\notes_center.png"))
#define		NOTESLANE_POS_X		(SCREEN_WIDTH/2)
#define		NOTESLANE_POS_Y		(NOTES_POS_Y)
#define		NOTESLANE_SIZE_X	(LANE_SIZE_X*5)
#define		NOTESLANE_SIZE_Y	(100.0f)
#define		NOTESLANE_TEX		(((char*)"data\\texture\\notes_lane.png"))

#define		SCORE_SIZE_X	(45.0f)
#define		SCORE_SIZE_Y	(60.0f)
#define		SCORE_POS_X		(1700.0f)
#define		SCORE_POS_Y		(80.0f)
#define		SCOER_DIGIT		(8)

//レーンの初期値
#define		LANE_MAX			(5)				//レーン数
#define		LANE_DRAW_SIZE_X	(150)
#define		LANE_DRAW_SIZE_Y	(SCREEN_HEIGHT)

#define		PLAYER_HP_POS_X		(CENTER_X+460.0f)							//プレイヤー体力ゲージ座標X
#define		PLAYER_HP_POS_Y		(CENTER_Y-10.0f)							//プレイヤー体力ゲージ座標Y
#define		PLAYER_HP_SIZE_X	(50.0f)										//プレイヤー体力サイズX
#define		PLAYER_HP_SIZE_Y	(900.0f)									//プレイヤー体力サイズY
#define		PLAYER_HPB_SIZE_X	(60.0f)										//プレイヤー体力サイズX
#define		PLAYER_HPB_SIZE_Y	(910.0f)									//プレイヤー体力サイズY
#define		PLAYER_HP_TEX_A		((char*)"data\\texture\\enemyHpBar00.png")	//プレイヤーテクスチャ
#define		PLAYER_HP_TEX_B		((char*)"data\\texture\\enemyHpBar01.png")	//プレイヤーテクスチャ

#define		ASCII_NUM_0	(48)	

//シーンの定義
enum SCENE
{
	SCENE_NONE,			//シーン無し
	SCENE_TITLE,		//タイトル画面
	SCENE_GAME,			//ゲーム本編
	SCENE_RESULT,		//リザルト画面
	SCENE_GAMEOVER,		//ゲームオーバー
	SCENE_STAGESELECT,	//セレクト
	SCENE_SKILLSELECT,	//スキルセレクト
	SCENE_TUTO,
	SCENE_STAGE_0,
	SCENE_CREDIT,

	SCENE_NUM,		//終端コード
};

struct NOW_SCENE
{
	int SceneNow;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void SetScene(int nextScene);
bool GetCtrl();
bool GetDebugFlag();
NOW_SCENE* GetNowScene();
float frand(void);

