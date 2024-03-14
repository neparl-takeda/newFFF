#pragma once
#include "main.h"
#include "renderer.h"

#include <stdio.h>

#define CONFIG_SIZE_A (0.7f)

struct CONFIG
{
	//�ݒ荀��
	float cSoundSEv;
	float cSoundBGMv;
	float cSoundGSEv;
	
	int cLight;
	int	cBulletCol;

	int g_TextureLight;


	//�R���t�B�O�N���X�̃X�e�[�^�X
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

