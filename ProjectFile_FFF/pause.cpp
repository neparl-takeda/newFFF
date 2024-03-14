/*==============================================================================

   �|�[�Y�̐��� [pause.cpp]
														 Author :�@�ēc �S��
														 Date   :�@12/13
--------------------------------------------------------------------------------
P�{�^���Ń|�[�Y(ESC�̓R���\�[�����I�����邩��ł��Ȃ�����)

W��S�ŋ@�\�̑I��(�ォ��R���e�B�j���[�A���g���C�A�^�C�g����ʂɖ߂�)

ENTER�ŋ@�\�̎��s�ł��B

����̂��߂ɍ��܂ł�ENTER�Ń^�C�g���ɖ߂�́uG�L�[�v�ɓ�����ڂ��Ă��܂��B
*/

//==============================================================================

#define MENU_DISTANCE	(150)
#define	COUNT_TEXT_SIZE	(300.0f)
#define PRESSING_COUNT	(15)

#include "pause.h"
#include "main.h"
#include "Config.h"
#include "UIText.h"

static int PauseTexture;
static int ArrowTexture;

int arrowNum;
float num;

PAUSE hoge;

CONFIG* pPauseConfig = GetConfig();

UIText* g_UITextPause{};
UIText* g_UITextRestartCount{};
UIText* g_UITextMenu[PAUSE_NUM]{};

void InitPause()
{
	PauseTexture		= LoadTexture((char*)"data/TEXTURE/UI_Back_B.png");
	ArrowTexture		= LoadTexture((char*)"data/TEXTURE/arrow.png");

	hoge.pause = false;		//PAUSE�����ǂ����̔���
	hoge.restart = false;	//�R���e�B�j���[����
	hoge.pause_frame = 0;	//PAUSE���畜�A����܂ł̎���(3�b)
	hoge.PauseAction = 0;	//PAUSE�őI�������@�\�A�y�і��̏ꏊ(Continue�ARetry�ABackTitle)
	hoge.alpha = 0.7f;		//�Ó]�

	D3DXVECTOR2 uiTextPos	= { CENTER_X, CENTER_Y };
	D3DXVECTOR2 uiTextSize	= { 100.0f ,100.0f };
	D3DXCOLOR   uiTextColor = { 1.0f,1.0f,1.0f,1.0f };
	for (int i = 0; i < PAUSE_NUM; i++)
	{
		uiTextPos = { CENTER_X, CENTER_Y + MENU_DISTANCE * (i - 1), };
		g_UITextMenu[i] = new UIText;
		g_UITextMenu[i]->Init();
		g_UITextMenu[i]->SetUITextPram(uiTextPos, uiTextSize, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (char*)"NONE", FONT_NORMAL);
	}

	uiTextPos		= { CENTER_X,CENTER_Y - 400, };
	g_UITextPause	= new UIText;
	g_UITextPause	->Init();
	g_UITextPause	->SetUITextPram(uiTextPos, uiTextSize, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (char*)"PAUSE", FONT_NORMAL);

	g_UITextMenu[PAUSE_CONTINUE]->SetText((char*)"START");
	g_UITextMenu[PAUSE_RETRY]	->SetText((char*)"RETRY");
	g_UITextMenu[PAUSE_OPTION]	->SetText((char*)"OPTION");
	g_UITextMenu[PAUSE_BACK]	->SetText((char*)"BACK");

	uiTextPos	= { CENTER_X,CENTER_Y };
	uiTextSize	= { COUNT_TEXT_SIZE,COUNT_TEXT_SIZE };
	g_UITextRestartCount = new UIText;
	g_UITextRestartCount->Init();
	g_UITextRestartCount->SetUITextPram(uiTextPos, uiTextSize, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (char*)"0", FONT_NORMAL);

}

void UninitPause()
{
	hoge.pause = false;
	hoge.restart = false;
	hoge.pause_frame = 0;
	hoge.PauseAction = 0;
}


void UpdatePause()
{
	//�R���e�B�j���[�̏���
	//
	if (hoge.restart == true)
	{
		hoge.pause_frame++;
	}

	if (hoge.pause_frame == 180)
	{
		hoge.pause_frame = 0;
		hoge.restart = false;
		hoge.pause = false;
		RePlaySound(BGM_RE());
	}

	if ((IsButtonGroupPressing(KEY_GROUP::KEY_UP, PRESSING_COUNT)) &&(!pPauseConfig->cScreenActive))
	{
		hoge.PauseAction -= 1;
		if (hoge.PauseAction == -1)
		{	hoge.PauseAction = PAUSE_BACK;	}
	}

	if ((IsButtonGroupPressing(KEY_GROUP::KEY_DOWN, PRESSING_COUNT)) && (!pPauseConfig->cScreenActive))
	{
		hoge.PauseAction += 1;
		if (hoge.PauseAction == 4)
		{	hoge.PauseAction = PAUSE_CONTINUE;	}
	}

	if ((IsButtonGroupTriggered(KEY_GROUP::KEY_START)) && hoge.pause == false)
	{
		PauseSound(BGM_RE());
		hoge.pause = true;
		hoge.PauseAction = 0;
	}

	if ((IsButtonGroupTriggered(KEY_GROUP::KEY_ENTER)) && (hoge.pause == true) && (!pPauseConfig->cScreenActive))
	{
		switch (hoge.PauseAction)
		{
		case PAUSE_CONTINUE:
			hoge.restart = true;
			break;
		case PAUSE_RETRY:
			ReTry();
			break;
		case PAUSE_OPTION:
			Option();
			break;
		case PAUSE_BACK:
			Back();
			break;
		}
	}


	for (int i = 0; i < PAUSE_NUM; i++)	//�I�𒆍��ڂ�UIText�̐F��ς���
	{
		if (hoge.PauseAction == i)
		{	g_UITextMenu[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f));	}
		else
		{	g_UITextMenu[i]->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));	}
	}
}

void DrawPause()
{
	if (hoge.pause == true)
	{
		//�Ó]
		DrawSpriteColor(PauseTexture,
			0.0f,
			0.0f,
			10000.0f,
			10000.0f,
			0.0f,
			0.0f,
			1.0f,
			1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, hoge.alpha));

		if (hoge.restart == false)
		{
			g_UITextPause->Draw();	// "PAUSE" �e�L�X�g
			
			for (int i = 0; i < PAUSE_NUM; i++)
			{	g_UITextMenu[i]->Draw();	}	// Menu ���� �e�L�X�g
		}

		//���
		if (hoge.restart == false)
		{
			DrawSpriteColor(ArrowTexture,
				CENTER_X / 2 + 125,
				CENTER_Y - 150 + (hoge.PauseAction * 150),
				100.0f,
				100.0f,
				0.25f,
				0.0f,
				0.25f,
				1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	if (hoge.restart == true)
	{
		TranslationCount();
		g_UITextRestartCount->Draw();
	}
}

void ReTry()
{
	NOW_SCENE* pNowScene = GetNowScene();
	if (pNowScene->SceneNow == SCENE_GAME)
	{
		SceneTransition(SCENE_GAME);
	}
	if (pNowScene->SceneNow == SCENE_TUTO)
	{
		SceneTransition(SCENE_TUTO);
	}
}

void Back()
{ 
	SceneTransition(SCENE_STAGESELECT);
}

void Option()
{
	pPauseConfig->cLine = 0;
	pPauseConfig->cScreenActive = true;
}

PAUSE* GetPause()
{
	return &hoge;
}

void TranslationCount()
{
	char countChar[256] = "0";

	countChar[0] = (3 - (hoge.pause_frame / 60)) + ASCII_NUM_0;
	g_UITextRestartCount->SetText(countChar);
}
