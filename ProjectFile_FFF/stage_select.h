
#pragma once


#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGE_MAX (6)

enum STAGE
{
	STAGE_0,
	STAGE_1,
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
	STAGE_6,
	STAGE_7,
	STAGE_8,
	STAGE_9,
	STAGE_10,
};

enum PANEL_LANE
{
	PLANE_MIN,
	PLANE_0,
	PLANE_1,
	PLANE_2,
	PLANE_3,
	PLANE_4,
	PLANE_5,
	PLANE_6,
	PLANE_7,
	PLANE_8,
	PLANE_9,
	PLANE_10,
	PLANE_11

};

enum DIRECTION_PANEL	//移動の向き
{
	D_LEFT = 0,
	D_RIGHT = 1,
};

struct STAGE_PANEL
{
	D3DXVECTOR2 pos;
	float spd;
	D3DXVECTOR2 size;
	D3DXCOLOR	col;
	int texnoC		= 0;
	int direction	= 0;
	int NowLane		= 0;
	bool moving;
};

struct WHITEBOX
{
	D3DXVECTOR2 pos	{};
	float		spd	{};
	D3DXVECTOR2 size{};
	float		rot	{};
	D3DXCOLOR	col	{};
	bool use		{};
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStageSelect(void);
void UninitStageSelect(void);
void UpdateStageSelect(void);
void DrawStageSelect(void);
int GetGemeStageNum(void);
void SetWhite(float x, float sz, float rt);
int  GetDifficulty();
bool GetUra();
bool GetSceneChange();
void SetBossNameText();
void ChangeDifficulty(int difficulty);
STAGE_PANEL GetNowSelect();