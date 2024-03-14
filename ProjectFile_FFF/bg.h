#pragma once

#include "main.h"
#include "renderer.h"

struct BG
{
	D3DXVECTOR2 size;
	D3DXVECTOR2	pos;
	float	spdY	= 0.0f;
	float	rot		= 0.0f;
	float	rotAdd	= 0.0f;
	bool	use		= false;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(int StageNum);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

