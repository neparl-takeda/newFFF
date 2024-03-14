#pragma once

#include "main.h"
#include "renderer.h"
#include "inputx.h"
#include "keyboard.h"
#include "sound.h"
#include "texture.h"
#include "sprite.h"
#include "rhythm.h"
#include "fade.h"

typedef struct
{
	bool pause;
	bool restart;
	int pause_frame;
	float alpha;

	int PauseAction;

}PAUSE;

enum PAUSE_STATE
{
	PAUSE_CONTINUE	= 0,
	PAUSE_RETRY,
	PAUSE_OPTION,
	PAUSE_BACK,
	PAUSE_NUM
};

void InitPause();
void UpdatePause();
void UninitPause();
void DrawPause();

void ReTry();
void Back();
void Option();

PAUSE* GetPause();

void TranslationCount();