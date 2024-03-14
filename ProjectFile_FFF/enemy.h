//====================================================
//エネミーへッダー
//作成：勝沼力樹
//		2022/10/26
//====================================================
#pragma once
#ifndef ENEMY_H_
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define			ENEMY00_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY00_SIZE_Y		(ENEMY00_SIZE_X*1.5f)							//エネミーサイズY
#define			ENEMY01_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY01_SIZE_Y		(ENEMY01_SIZE_X*1.5f)							//エネミーサイズY
#define			ENEMY02_SIZE_X		(LANE_SIZE_X*2)								//エネミーサイズX
#define			ENEMY02_SIZE_Y		(ENEMY02_SIZE_X*3.0f)							//エネミーサイズY
#define			ENEMY03_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY03_SIZE_Y		(ENEMY03_SIZE_X*1.5f)							//エネミーサイズY
#define			ENEMY04_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY04_SIZE_Y		(ENEMY04_SIZE_X*1.75f)							//エネミーサイズY
#define			ENEMY05_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY05_SIZE_Y		(ENEMY05_SIZE_X*1.5f)							//エネミーサイズY
#define			ENEMY06_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY06_SIZE_Y		(ENEMY06_SIZE_X*1.5f)							//エネミーサイズY
#define			ENEMY07_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY07_SIZE_Y		(ENEMY07_SIZE_X*1.0f)							//エネミーサイズY
#define			ENEMY08_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY08_SIZE_Y		(ENEMY08_SIZE_X)							//エネミーサイズY
#define			ENEMY09_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY09_SIZE_Y		(ENEMY09_SIZE_X*1.75f)							//エネミーサイズY
#define			ENEMY10_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY10_SIZE_Y		(ENEMY10_SIZE_X*1.5f)							//エネミーサイズY
#define			ENEMY11_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY11_SIZE_Y		(ENEMY11_SIZE_X*1.5f)							//エネミーサイズY
#define			ENEMY12_SIZE_X		(LANE_SIZE_X*3.0f)								//エネミーサイズX
#define			ENEMY12_SIZE_Y		(ENEMY12_SIZE_X*1.8f)							//エネミーサイズY
#define			ENEMY13_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY13_SIZE_Y		(ENEMY13_SIZE_X*1.25f)							//エネミーサイズY
#define			ENEMY14_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY14_SIZE_Y		(ENEMY14_SIZE_X*1.75f)							//エネミーサイズY
#define			ENEMY15_SIZE_X		(LANE_SIZE_X*3)								//エネミーサイズX
#define			ENEMY15_SIZE_Y		(ENEMY15_SIZE_X*1.25f)							//エネミーサイズY

#define			ENEMY_SP			(3.0f)										//エネミースピード
#define			ENEMY_SP_Y			(25.0f)										//エネミースピード
#define			ENEMY_SP_A			(0.925f)										//エネミースピード
#define			ENEMY_HP			(500)										//エネミーHP初期値
//#define			ENEMY_TEX			((char*)"data\\texture\\ZARI_BOSS_1.png")			//エネミーテクスチャ
#define			ENEMY_HP_POS_X		(CENTER_X-460.0f)									//エネミー体力ゲージ座標X
#define			ENEMY_HP_POS_Y		(CENTER_Y-15.0f)									//エネミー体力ゲージ座標Y
#define			ENEMY_HP_SIZE_X		(50.0f)									//エネミー体力サイズX
#define			ENEMY_HP_SIZE_Y		(900.0f)										//エネミー体力サイズY
//#define			ENEMY_HPB_SIZE_X	(510.0f)									//エネミー体力サイズX
//#define			ENEMY_HPB_SIZE_Y	(60.0f)										//エネミー体力サイズY
#define			ENEMY_HPB_SIZE_X	(60.0f)									//エネミー体力サイズX
#define			ENEMY_HPB_SIZE_Y	(910.0f)										//エネミー体力サイズY
#define			ENEMY_HP_TEX_A		((char*)"data\\texture\\HP_player_A.png")		//エネミーテクスチャ
#define			ENEMY_HP_TEX_B		((char*)"data\\texture\\HP_player_C.png")		//エネミーテクスチャ
#define			ENEMY_SPAWN_POS_X	(SCREEN_WIDTH/2)
#define			ENEMY_SPAWN_POS_Y	(-20.0f)


//*****************************************************************************
// エネミー　クラス
//*****************************************************************************
struct ENEMY
{
	D3DXVECTOR2		size;
	D3DXVECTOR2		pos;
	D3DXVECTOR2		sp;
	float			rot;
	D3DXCOLOR		col;
	float			texno;
	int				hp;
	bool			use;
};

struct ENEMYHP
{
	D3DXVECTOR2		size;
	D3DXVECTOR2		pos;
	float			rot;
	D3DXCOLOR		col;
	float			texnoA;
	float			texnoB;
};

HRESULT		InitEnemy(int StageNum);
void		UninitEnemy();
void		UpdateEnemy();
void		DrawEnemy();
ENEMY*		GetEnemy();
int			GetEnemyNum();
int			GetEnemyTex();
bool		BadEnd();
#endif // !EMEMY_H_



