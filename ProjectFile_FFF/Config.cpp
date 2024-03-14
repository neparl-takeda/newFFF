#include "Config.h"
#include "inputx.h"
#include "keyboard.h"
#include "renderer.h"
#include "texture.h"
#include "sprite.h"
#include <stdio.h>
#include "sound.h"

CONFIG g_Config;
FILE* FileConfigSound;
FILE* FileConfigVideo;

static int g_TextureConfigBg;
static int g_TextureConfigBanner;
static int g_TextureConfigPoint;
static int g_TextureConfigCol;
static int g_TextureConfigSound;
static int g_TextureConfigVideo;
static int g_TextureConfigSave;

float m_ConfigLight;
int m_ConfigSSEv;
int m_ConfigSBGMv;
int m_ConfigSGSEv;
int m_ConfigCol;

float  cLineCol[4];
float  cColCol [7];
int	   cADFrame;
static bool cKey_qe;
static bool cKey_ws;

float cGb[3];

void InitConfig()
{
	g_Config.g_TextureLight = LoadTexture((char*)"data/TEXTURE/fade_white.png");
	g_TextureConfigBg		= LoadTexture((char*)"data/TEXTURE/UI_Config.png");
	g_TextureConfigBanner	= LoadTexture((char*)"data/TEXTURE/UI_Config_banner.png");
	g_TextureConfigSound	= LoadTexture((char*)"data/TEXTURE/UI_Config_Sound.png");
	g_TextureConfigVideo	= LoadTexture((char*)"data/TEXTURE/UI_Config_Video.png");
	g_TextureConfigPoint	= LoadTexture((char*)"data/TEXTURE/UI_Config_Point.png");
	g_TextureConfigCol		= LoadTexture((char*)"data/TEXTURE/UI_Config_Col.png");
	g_TextureConfigSave		= LoadTexture((char*)"data/TEXTURE/UI_Config_Save.png");

	//ファイルを指定して開く
	FileConfigSound = fopen("data/SAVE/ConfigSound.txt", "r+");
	FileConfigVideo = fopen("data/SAVE/ConfigVideo.txt", "r+");

	//ファイルから変数を取得
	fscanf(FileConfigSound, "%d,%d,%d",&m_ConfigSBGMv, &m_ConfigSSEv, &m_ConfigSGSEv);
	fscanf(FileConfigVideo, "%d,%d", &g_Config.cLight,&m_ConfigCol);

	//音量を適正なfloat型に変換
	g_Config.cSoundBGMv = (m_ConfigSBGMv * 0.01f);
	g_Config.cSoundSEv  = (m_ConfigSSEv  * 0.01f);
	g_Config.cSoundGSEv = (m_ConfigSGSEv * 0.01f);

	//明るさを適正なα値に変換
	m_ConfigLight = (1.0f-(g_Config.cLight *0.01f))*0.75f;

	g_Config.cBulletCol = m_ConfigCol;
	
	g_Config.cScreenActive = false;
	g_Config.cPosY = SCREEN_HEIGHT*1.5f;
	g_Config.cSpdY = 90.0f;
	g_Config.cPage = -1;
	g_Config.cLine = 0;
	g_Config.cFrame= 0;

	cADFrame = 0;
	cKey_qe = false;
	cKey_ws = false;

	cLineCol[0] = 1.0f;
	cLineCol[1] = 1.0f;
	cLineCol[2] = 1.0f;
	cLineCol[3] = 1.0f;

	for (int i = 0; i < 7; i++)
	{
		cColCol[i] = 0.5f;
	}
}

void UninitConfig()
{
	fclose(FileConfigSound);
	fclose(FileConfigVideo);
}

void UpdateConfig()
{
	//仮操作
	if (Keyboard_IsKeyDown(KK_O))
	{
		g_Config.cScreenActive = true;
		g_Config.cLine = 0;
	}
	if ((Keyboard_IsKeyDown(KK_P))&&(g_Config.cScreenActive))
	{
		g_Config.cScreenActive = false;
		g_Config.cLine = 0;
	}
	
	if (g_Config.cFrame==2)
	{
		FileConfigSound = fopen("data/SAVE/ConfigSound.txt", "w");
		FileConfigVideo = fopen("data/SAVE/ConfigVideo.txt", "w");
		fprintf(FileConfigSound, "%d,%d,%d,",	m_ConfigSBGMv, m_ConfigSSEv, m_ConfigSGSEv);
		fprintf(FileConfigVideo, "%d,%d,",		g_Config.cLight, g_Config.cBulletCol);
		fclose(FileConfigSound);
		fclose(FileConfigVideo);
	}


	
	if (IsButtonGroupTriggered(KEY_GROUP::KEY_ENTER))
	{
		if ((g_Config.cLine == 3) && (g_Config.cPage == -1))
		{
			g_Config.cScreenActive = false;
			m_ConfigSBGMv = (int)(g_Config.cSoundBGMv * 100.0f);
			m_ConfigSSEv  = (int)(g_Config.cSoundSEv  * 100.0f);
			m_ConfigSGSEv = (int)(g_Config.cSoundGSEv * 100.0f);
		}
		if ((g_Config.cLine == 3) && (g_Config.cPage == 1))
		{
			g_Config.cScreenActive = false;
		}
	}

	if (((IsButtonGroupTriggered(KEY_GROUP::KEY_TAB_LEFT))|| (IsButtonGroupTriggered(KEY_GROUP::KEY_TAB_RIGHT)))
		&& (g_Config.cScreenActive) && (!cKey_qe))
	{
		g_Config.cPage *= -1;
		g_Config.cLine = 0;
		cKey_qe = true;
	}
	if ((IsButtonGroupTriggered(KEY_GROUP::KEY_UP)) && (g_Config.cScreenActive) && (!cKey_ws))
	{
		if (g_Config.cLine > 0)
		{
			g_Config.cLine--;
		}
		if ((g_Config.cLine == 2) && (g_Config.cPage == 1))
		{
			g_Config.cLine = 1;
		}
		cKey_ws = true;
	}
	if ((IsButtonGroupTriggered(KEY_GROUP::KEY_DOWN)) && (g_Config.cScreenActive) && (!cKey_ws))
	{
		if ((g_Config.cLine < 3)&&(g_Config.cPage==-1))
		{
			g_Config.cLine++;
		}
		if ((g_Config.cLine < 2) && (g_Config.cPage == 1))
		{
			g_Config.cLine++;
		}
		if ((g_Config.cLine == 2) && (g_Config.cPage == 1))
		{
			g_Config.cLine = 3;
		}
		cKey_ws = true;
	}

		switch (g_Config.cLine)
		{
		case 0:
			if (g_Config.cPage == -1)
			{
				if ((IsButtonGroupPressed(KEY_GROUP::KEY_LEFT)) && (g_Config.cScreenActive))
				{
					if (g_Config.cSoundBGMv >= 0.01f)
					{
						g_Config.cSoundBGMv-=0.01f;
					}
				}
				if ((IsButtonGroupPressed(KEY_GROUP::KEY_RIGHT)) && (g_Config.cScreenActive))
				{
					if (g_Config.cSoundBGMv <= 0.99f)
					{
						g_Config.cSoundBGMv += 0.01f;
					}
				}
			}
			if (g_Config.cPage == 1)
			{
				if ((IsButtonGroupPressed(KEY_GROUP::KEY_LEFT)) && (g_Config.cScreenActive))
				{
					if (g_Config.cLight >= 1)
					{
						g_Config.cLight--;
					}
				}
				if ((IsButtonGroupPressed(KEY_GROUP::KEY_RIGHT)) && (g_Config.cScreenActive))
				{
					if (g_Config.cLight <= 99)
					{
						g_Config.cLight++;
					}	
				}

			}

			break;
		case 1:
			if (g_Config.cPage == -1)
			{
				if ((IsButtonGroupPressed(KEY_GROUP::KEY_LEFT)) && (g_Config.cScreenActive))
				{
					if (g_Config.cSoundGSEv >= 0.01f)
					{
						g_Config.cSoundGSEv -= 0.01f;
					}

				}
				if ((IsButtonGroupPressed(KEY_GROUP::KEY_RIGHT)) && (g_Config.cScreenActive))
				{
					if (g_Config.cSoundGSEv <= 0.99f)
					{
						g_Config.cSoundGSEv += 0.01f;
					}
				}
			}
			if (g_Config.cPage == 1)
			{
				cADFrame++;
				
				if ((IsButtonGroupPressed(KEY_GROUP::KEY_LEFT)) && (g_Config.cScreenActive) && (cADFrame >= 11))
				{
					if (g_Config.cBulletCol >= 1)
					{
						g_Config.cBulletCol--;
					}
					cADFrame = 0;
				}
				if ((IsButtonGroupPressed(KEY_GROUP::KEY_RIGHT)) && (g_Config.cScreenActive) && (cADFrame >= 11))
				{
					if (g_Config.cBulletCol <= 5)
					{
						g_Config.cBulletCol++;
					}
					cADFrame = 0;
				}
			}
			break;
		case 2:
			if ((IsButtonGroupPressed(KEY_GROUP::KEY_LEFT)) && (g_Config.cScreenActive))
			{
				if (g_Config.cSoundSEv >= 0.01f)
				{
					g_Config.cSoundSEv -= 0.01f;
				}
			}
			if ((IsButtonGroupPressed(KEY_GROUP::KEY_RIGHT)) && (g_Config.cScreenActive))
			{
				if (g_Config.cSoundSEv <= 0.99f)
				{
					g_Config.cSoundSEv += 0.01f;
				}
			}
			break;

		default:
			break;
		}


	if (!IsButtonGroupTriggered(KEY_GROUP::KEY_TAB_LEFT) && !IsButtonGroupTriggered(KEY_GROUP::KEY_TAB_RIGHT))
	{
		cKey_qe = false;
	}
	if (!IsButtonGroupTriggered(KEY_GROUP::KEY_UP) && !IsButtonGroupTriggered(KEY_GROUP::KEY_DOWN))
	{
		cKey_ws = false;
	}

	cLineCol[0] = 0.5f;
	cLineCol[1] = 0.5f;
	cLineCol[2] = 0.5f;
	cLineCol[3] = 0.5f;
	cLineCol[g_Config.cLine] = 1.0f;
	
	for (int i = 0; i < 7; i++)
	{
		cColCol[i] = 0.3f;
	}
	cColCol[g_Config.cBulletCol] = 1.0f;

	if (g_Config.cScreenActive)
	{
		g_Config.cFrame = 0;
		if (g_Config.cPosY >= CENTER_Y)
		{
			g_Config.cPosY -= g_Config.cSpdY;
			g_Config.cSpdY *= 0.925f;
		}
		if (g_Config.cPosY <= CENTER_Y)
		{
			g_Config.cPosY = CENTER_Y;
		}
	}
	else
	{
		g_Config.cFrame++;
		if (g_Config.cPosY <= SCREEN_HEIGHT * 1.5f)
		{
			g_Config.cPosY += g_Config.cSpdY;
			g_Config.cSpdY = 90.0f;
		}
	}

	m_ConfigLight = (1.0f - (g_Config.cLight * 0.01f)) * 0.75f;
}

void DrawConfig()
{
	if (g_Config.cScreenActive)
	{
		DrawSpriteColor(g_Config.g_TextureLight, CENTER_X, CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	}
			DrawSpriteColor(g_TextureConfigBg,		CENTER_X, g_Config.cPosY, 900.0f*CONFIG_SIZE_A, 1200.0f * CONFIG_SIZE_A,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
			if (g_Config.cPage == -1)
			{
				DrawSpriteColor(g_TextureConfigBanner, CENTER_X- (450.0f * CONFIG_SIZE_A)/2.0f, g_Config.cPosY - (450.0f * CONFIG_SIZE_A)-36.525f, 450.0f * CONFIG_SIZE_A, 200.0f * CONFIG_SIZE_A+6.0f,
					0.0f, 0.01f, 1.0f, 0.99f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				
				for (int i = 0; i < 3;i++)
				{
					DrawSpriteColor(g_TextureConfigSound, CENTER_X, g_Config.cPosY - 200.0f + (150.0f*i), 900.0f * CONFIG_SIZE_A, 200.0f * CONFIG_SIZE_A,
						0.0f, 0.2f * i, 1.0f, 0.2f, D3DXCOLOR(cLineCol[i], cLineCol[i], cLineCol[i], 1.0f));
				}
					DrawSpriteColor(g_TextureConfigPoint, CENTER_X - 87.5f + (g_Config.cSoundBGMv * 345.0f), g_Config.cPosY - 200.0f + (150.0f * 0.0f), 40.0f, 40.0f,
						0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(cLineCol[0], cLineCol[0], cLineCol[0], 1.0f));
					DrawSpriteColor(g_TextureConfigPoint, CENTER_X - 87.5f + (g_Config.cSoundGSEv  * 345.0f), g_Config.cPosY - 200.0f + (150.0f * 1.0f), 40.0f, 40.0f,
						0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(cLineCol[1], cLineCol[1], cLineCol[1], 1.0f));
					DrawSpriteColor(g_TextureConfigPoint, CENTER_X - 87.5f + (g_Config.cSoundSEv * 345.0f), g_Config.cPosY - 200.0f + (150.0f * 2.0f), 40.0f, 40.0f,
						0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(cLineCol[2], cLineCol[2], cLineCol[2], 1.0f));
			}
			if (g_Config.cPage == 1)
			{
				DrawSpriteColor(g_TextureConfigBanner, CENTER_X + (450.0f * CONFIG_SIZE_A) / 2.0f, g_Config.cPosY - (450.0f * CONFIG_SIZE_A) - 36.525f, 450.0f * CONFIG_SIZE_A, 200.0f * CONFIG_SIZE_A + 6.0f,
					0.0f, 0.01f, 1.0f, 0.99f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				for (int i = 0; i < 2; i++)
				{
					DrawSpriteColor(g_TextureConfigVideo, CENTER_X, g_Config.cPosY - 180.0f + (200.0f * i), 900.0f * CONFIG_SIZE_A, 250.0f * CONFIG_SIZE_A,
						0.0f, 0.25f * i, 1.0f, 0.25f, D3DXCOLOR(cLineCol[i], cLineCol[i], cLineCol[i], 1.0f));
				}
				DrawSpriteColor(g_TextureConfigPoint, CENTER_X - 87.5f + (((g_Config.cLight*0.01f)) * 345.0f), g_Config.cPosY - 138.0f, 40.0f, 40.0f,
					0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(cLineCol[0], cLineCol[0], cLineCol[0], 1.0f));

				DrawSpriteColor(g_TextureConfigCol, (CENTER_X - 240.0f) + (80.0f * 0.0f), g_Config.cPosY + 45.0f, 70.0f, 70.0f,
					(1.0f / 3.0f) * 1.0f, 0.0f, 1.0f / 3.0f, 1.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, cColCol[0]));
				DrawSpriteColor(g_TextureConfigCol, (CENTER_X - 240.0f) + (80.0f * 1.0f), g_Config.cPosY + 45.0f, 70.0f, 70.0f,
					(1.0f / 3.0f) * 1.0f, 0.0f, 1.0f / 3.0f, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, cColCol[1]));
				DrawSpriteColor(g_TextureConfigCol, (CENTER_X - 240.0f) + (80.0f * 2.0f), g_Config.cPosY + 45.0f, 70.0f, 70.0f,
					(1.0f / 3.0f) * 1.0f, 0.0f, 1.0f / 3.0f, 1.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, cColCol[2]));
				DrawSpriteColor(g_TextureConfigCol, (CENTER_X - 240.0f) + (80.0f * 3.0f), g_Config.cPosY + 45.0f, 70.0f, 70.0f,
					(1.0f / 3.0f) * 1.0f, 0.0f, 1.0f / 3.0f, 1.0f, D3DXCOLOR(1.0f, 0.0f, 1.0f, cColCol[3]));
				DrawSpriteColor(g_TextureConfigCol, (CENTER_X - 240.0f) + (80.0f * 4.0f), g_Config.cPosY + 45.0f, 70.0f, 70.0f,
					(1.0f / 3.0f) * 1.0f, 0.0f, 1.0f / 3.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, cColCol[4]));
				DrawSpriteColor(g_TextureConfigCol, (CENTER_X - 240.0f) + (80.0f * 5.0f), g_Config.cPosY + 45.0f, 70.0f, 70.0f,
					(1.0f / 3.0f) * 1.0f, 0.0f, 1.0f / 3.0f, 1.0f, D3DXCOLOR(0.0f, 1.0f, 1.0f, cColCol[5]));
				DrawSpriteColor(g_TextureConfigCol, (CENTER_X - 240.0f) + (80.0f * 6.0f), g_Config.cPosY + 45.0f, 70.0f, 70.0f,
					(1.0f / 3.0f) * 2.0f, 0.0f, 1.0f / 3.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, cColCol[6]));

				DrawSpriteColor(g_TextureConfigCol, (CENTER_X - 240.0f) + (80.0f * g_Config.cBulletCol), g_Config.cPosY + 45.0f, 70.0f, 70.0f,
					(1.0f / 3.0f) * 0.0f, 0.0f, 1.0f / 3.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			DrawSpriteColor(g_TextureConfigSave, CENTER_X, g_Config.cPosY - 200.0f + (150.0f * 3.5f), 450.0f * CONFIG_SIZE_A, 150.0f * CONFIG_SIZE_A,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(cLineCol[3], cLineCol[3], cLineCol[3], 1.0f));

			//明るさ調整
			DrawSpriteColor(g_Config.g_TextureLight, CENTER_X, CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, m_ConfigLight));
}


CONFIG* GetConfig()
{
	return &g_Config;
}