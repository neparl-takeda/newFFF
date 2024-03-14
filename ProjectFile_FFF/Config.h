#pragma once
#include "main.h"
#include "renderer.h"

#include <stdio.h>

#define CONFIG_SIZE_A (0.7f)

struct CONFIG
{
	//設定項目
	float cSoundSEv;
	float cSoundBGMv;
	float cSoundGSEv;
	
	int cLight;
	int	cBulletCol;

	int g_TextureLight;


	//コンフィグクラスのステータス
	bool  cScreenActive;
	float cPosY;
	float cSpdY;
	int	  cPage;
	int	  cLine;
	int	  cFrame;
};

void InitConfig();
void UninitConfig();
void UpdateConfig();
void DrawConfig();
CONFIG* GetConfig();

