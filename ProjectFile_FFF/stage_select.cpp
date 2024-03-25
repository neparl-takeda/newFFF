/*==============================================================================

   タイトル画面処理 [StageSelect.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "stage_select.h"
#include "skill_select.h"
#include "texture.h"
#include "sprite.h"
#include "inputx.h"
#include "sound.h"
#include "fade.h" 
#include "keyboard.h"
#include "main.h"

#include "UIText.h"
#include "Config.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WHITE_MAX	(24)
#define WHITE_DIST	(40)
#define OCT_ROT		(0.3f)
#define	HARD_SIZE_A	(0.4f)
#define	HARD_SIZE_B	(1.25f)
#define	HARD_SIZE_X	(800.0f * HARD_SIZE_A)
#define	HARD_SIZE_Y	(200.0f * HARD_SIZE_A)
#define HARD_POS_Y	(CENTER_Y - 400.0f)
#define BACK_POS_X	(45.0f)
#define BUTTON_SIZE	(150.0f)
#define BUTTON_ADD	(1)
#define STAGE_POS_Y (CENTER_Y - 100.0f)
#define VIBERATION_SPEED	(150)

#define PRESSING_COUNT	(5)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

CONFIG* pConfig = GetConfig();

enum DIFFICULTY
{
	DIFFICULTY_NONE = -1,
	DIFFICULTY_EASY,
	DIFFICULTY_HARD,
	DIFFICULTY_MYTHIC,
	DIFFICULTY_NUM
};

//テクスチャの識別子
static int g_TextureArrow;
static int g_TextureArrowBack;
static int g_TextureWhite;
static int g_TextureOct;
static int g_TextureUIButton;
static int g_TextureUIKey;
static int g_TextureUIHardFade;
static int g_TextureUIHardFrame;
static int g_TextureUIFrame;
static int g_TextureUIPlay;
static int g_TextureStagePanel;
//SEの識別子
static int g_SE_Start;
static int g_SE_Select;
static int g_SE_Stage;
static int g_SE_ChangeA;
static int g_SE_ChangeB;
static int g_SE_Back;
//BGMの識別子
static int g_BGMNo[16];//タイトル用BGMの識別子

int NowSelect = (int)STAGE_1;

float alpha;
float PlayAlpha;
float color;
float platePosY;
float octRot[2];
float arrowSize[2];
float buttonSize;
int	  buttonAdd;

STAGE_PANEL g_StagePanel[STAGE_MAX];
WHITEBOX	g_White[WHITE_MAX];
D3DXCOLOR ARROW_COL[2];
D3DXCOLOR HARD_COL;
D3DXCOLOR WHITE_COL;
D3DXCOLOR OCT_COL;

bool ura		= false;
bool change		= false;
bool first		= true;
bool playing	= true;
bool skillSlc	= false;

bool SceneChange= false;
int  playFst	= 0;
int  whiteCnt	= 0;
int	 StageSelectFrame = 0;

float	SsBgU[2];
float	SsBgV;

int  Difficulty = DIFFICULTY_EASY;

UIText* g_UITextBoss{};
UIText* g_UITextDifficulty[DIFFICULTY_NUM]{};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitStageSelect(void)
{
	//テクスチャを読み込んで識別子を受け取る
	//g_TextureBgStageSelect[0] = LoadTexture((char*)"data/TEXTURE/Back_Select.JPG");
	//g_TextureBgStageSelect[1] = LoadTexture((char*)"data/TEXTURE/Back_Select.JPG");

	g_TextureOct = LoadTexture((char*)"data/TEXTURE/oct.png");

	g_TextureArrow	    = LoadTexture((char*)"data/TEXTURE/arrow.png");
	g_TextureArrowBack  = LoadTexture((char*)"data/TEXTURE/arrow_Back.png");
	g_TextureUIHardFade = LoadTexture((char*)"data/TEXTURE/UI_Dif_Fade.png");
	g_TextureWhite		= LoadTexture((char*)"data/TEXTURE/fade_white.png");
	g_TextureUIButton	= LoadTexture((char*)"data/TEXTURE/UI_Buttons_B.png");
	g_TextureUIKey		= LoadTexture((char*)"data/TEXTURE/UI_Buttons_A.png");
	g_TextureUIHardFrame= LoadTexture((char*)"data/TEXTURE/UI_Dif_frame.png");
	g_TextureUIFrame	= LoadTexture((char*)"data/TEXTURE/UI_frame.png");
	g_TextureUIPlay		= LoadTexture((char*)"data/TEXTURE/UI_play.png");

	g_TextureStagePanel	= LoadTexture((char*)"data/TEXTURE/Stage_Panels.png");

	char	filename0[] = "data\\BGM\\00_Tutorial_120.wav";
	char	filename1[] = "data\\BGM\\01_Zarigani_120.wav";
	char	filename2[] = "data\\BGM\\02_Unagi_120.wav";
	char	filename3[] = "data\\BGM\\03_Tako_120.wav";
	char	filename4[] = "data\\BGM\\04_Shark_120.wav";
	char	filename5[] = "data\\BGM\\05_Shishamo_150.wav";
	char	filename6[] = "data\\BGM\\06_Zarigani_150.wav";
	char	filename7[] = "data\\BGM\\07_Unadon_150.wav";
	char	filename8[] = "data\\BGM\\08_Shiokara_150.wav";
	char	filename9[] = "data\\BGM\\09_Jawge_150.wav";
	char	filename10[]= "data\\BGM\\10_FullArmor_150.wav";
	char	filename11[]= "data\\BGM\\11_Prottype_150.wav";
	char	filename12[]= "data\\BGM\\12_Unagydra_150.wav";
	char	filename13[]= "data\\BGM\\13_Revenge_150.wav";
	char	filename14[]= "data\\BGM\\14_Jawge_180.wav";
	char	filename15[]= "data\\BGM\\15_Ganeshamo_180.wav";

	//初回のみ実行
	if (first)
	{
		//構造体の初期化
		for (int i = 0; i < STAGE_MAX; i++)
		{
			g_StagePanel[i].pos = D3DXVECTOR2(-480.0f * 2 + i * 480.0f, STAGE_POS_Y);
			g_StagePanel[i].spd = 10.0f;
			g_StagePanel[i].size = D3DXVECTOR2(300.0f, 300.0f);
			g_StagePanel[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_StagePanel[i].direction = D_RIGHT;
			g_StagePanel[i].moving = true;
			g_StagePanel[i].NowLane = i + 3;
		}
		ura = false;
		Difficulty = DIFFICULTY_EASY;
		first = false;
		playing = true;

		g_StagePanel[1].size = D3DXVECTOR2(395.0f, 395.0f);
		g_StagePanel[STAGE_5].NowLane = PLANE_1;

		g_BGMNo[0] = LoadSound(filename0);
		g_BGMNo[1] = LoadSound(filename1);
		g_BGMNo[2] = LoadSound(filename2);
		g_BGMNo[3] = LoadSound(filename3);
		g_BGMNo[4] = LoadSound(filename4);
		g_BGMNo[5] = LoadSound(filename5);
		g_BGMNo[6] = LoadSound(filename6);
		g_BGMNo[7] = LoadSound(filename7);
		g_BGMNo[8] = LoadSound(filename8);
		g_BGMNo[9] = LoadSound(filename9);
		g_BGMNo[10]= LoadSound(filename10);
		g_BGMNo[11]= LoadSound(filename11);
		g_BGMNo[12]= LoadSound(filename12);
		g_BGMNo[13]= LoadSound(filename13);
		g_BGMNo[14]= LoadSound(filename14);
		g_BGMNo[15]= LoadSound(filename15);

		HARD_COL = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		octRot[0] = 0.0f;
		octRot[1] = OCT_ROT;
		WHITE_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		OCT_COL = D3DXCOLOR(0.0f, 0.4f, 1.0f, 0.2f);

		SsBgU[0] = 0.0f;
		SsBgU[1] = 0.0f;
		SsBgV    = 0.0f;

	}

	char	file_SE_Select	[] = "data\\SE\\SE_MenuMove.wav";
	char	file_SE_Stage	[] = "data\\SE\\SE_MenuPush.wav";
	char	file_SE_ChangeA	[] = "data\\SE\\SE_Change.wav";
	char	file_SE_ChangeB	[] = "data\\SE\\SE_ChangeB.wav";
	char	file_SE_Back	[] = "data\\SE\\SE_MenuBack.wav";
	char	file_SE_Start	[] = "data\\SE\\SE_Don.wav";
	g_SE_Select = LoadSound(file_SE_Select);
	g_SE_Stage	= LoadSound(file_SE_Stage);
	g_SE_ChangeA= LoadSound(file_SE_ChangeA);
	g_SE_ChangeB= LoadSound(file_SE_ChangeB);
	g_SE_Back	= LoadSound(file_SE_Back);
	g_SE_Start	= LoadSound(file_SE_Start);

	change		= false;
	
	alpha		= 1.0f;
	color		= 1.0f;
	playFst		= 0;

	buttonSize	= BUTTON_SIZE;
	buttonAdd	= BUTTON_ADD;
	platePosY	= STAGE_POS_Y + 270.0f;
	PlayAlpha	= 0.0f;

	ARROW_COL[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ARROW_COL[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	arrowSize[0] = 100.0f;
	arrowSize[1] = 100.0f;

	StageSelectFrame = 0;

	//ホワイト
	for (int w = 0; w < WHITE_MAX; w++)
	{
		g_White[w].pos = D3DXVECTOR2(0.0f, 0.0f);
		g_White[w].spd = 5.0f;
		g_White[w].size= D3DXVECTOR2(50.0f, 50.0f);
		g_White[w].rot = 0.0f;
		g_White[w].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_White[w].use = false;
	}
	whiteCnt = 0;

	//スキルセレクト
	skillSlc = false;
	
	SceneChange = false;

	SetVolume(g_SE_Select, pConfig->cSoundSEv);
	SetVolume(g_SE_Stage, pConfig->cSoundSEv);
	SetVolume(g_SE_ChangeA, pConfig->cSoundSEv);
	SetVolume(g_SE_ChangeB, pConfig->cSoundSEv);
	SetVolume(g_SE_Back, pConfig->cSoundSEv);
	SetVolume(g_SE_Start, pConfig->cSoundSEv);
	SetVolume(g_BGMNo[NowSelect], pConfig->cSoundBGMv);

	//UiText Boss
	g_UITextBoss = new UIText;
	g_UITextBoss->Init();
	STAGE_PANEL nowPanel = GetNowSelect();
	D3DXVECTOR2 uiTextPos	= { nowPanel.pos.x, platePosY,};
	D3DXVECTOR2 uiTextSize	= { 55.0f ,55.0f };
	D3DXCOLOR   uiTextColor = { 1.0f,1.0f,1.0f,1.0f };
	g_UITextBoss->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"START", FONT_GRADATION);

	//UiText Difficulty
	for (int i = 0; i < DIFFICULTY_NUM; i++)
	{
		g_UITextDifficulty[i] = new UIText;
		g_UITextDifficulty[i]->Init();
		uiTextPos	= { CENTER_X + (HARD_SIZE_X * (i - 1)) , HARD_POS_Y, };
		uiTextSize	= { 45.0f ,45.0f };
		g_UITextDifficulty[i]->SetUITextPram(uiTextPos, uiTextSize, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (char*)"NONE", FONT_GRADATION);
	}

	g_UITextDifficulty[DIFFICULTY_EASY]	 ->SetText((char*)"EASY");
	g_UITextDifficulty[DIFFICULTY_HARD]	 ->SetText((char*)"HARD");
	g_UITextDifficulty[DIFFICULTY_MYTHIC]->SetText((char*)"MYTHIC");

	g_UITextDifficulty[DIFFICULTY_EASY]	 ->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	g_UITextDifficulty[DIFFICULTY_HARD]	 ->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	g_UITextDifficulty[DIFFICULTY_MYTHIC]->SetColor(D3DXCOLOR(0.8f, 0.0f, 1.0f, 1.0f));

	ChangeDifficulty(Difficulty);

	//BGMの再生（2つ目の引数はループ回数）
	PlaySound(g_BGMNo[NowSelect], -1);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStageSelect(void)
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (g_StagePanel[i].NowLane == PLANE_3)
		{
			g_StagePanel[i].pos.x = CENTER_X;
			g_StagePanel[i].pos.y = STAGE_POS_Y;
			g_StagePanel[i].size = D3DXVECTOR2(390.0f, 390.0f);
		}
	}

	color = 1.0f;
	skillSlc = false;
	change = false;

	for (int i = 0; i < 16; i++)
	{
		StopSound(g_BGMNo[i]);
	}
	g_TextureArrow = NULL;


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStageSelect(void)
{

	SetVolume(g_SE_Select,  pConfig->cSoundSEv);
	SetVolume(g_SE_Stage,   pConfig->cSoundSEv);
	SetVolume(g_SE_ChangeA, pConfig->cSoundSEv);
	SetVolume(g_SE_ChangeB, pConfig->cSoundSEv);
	SetVolume(g_SE_Back,	pConfig->cSoundSEv);
	SetVolume(g_SE_Start,	pConfig->cSoundSEv);

	if (StageSelectFrame <= 60)
	{
		StageSelectFrame++;
	}

	if ((g_StagePanel[0].moving == false) && (!playing))
	{
		playing = true;
		PlaySound(g_BGMNo[NowSelect], 0);
		SetVolume(g_BGMNo[NowSelect], pConfig->cSoundBGMv);
	}

	if (pConfig->cScreenActive)
	{
		SetVolume(g_BGMNo[NowSelect], pConfig->cSoundBGMv);
	}

	if ((buttonSize <= BUTTON_SIZE-(BUTTON_SIZE/18))||(buttonSize>=BUTTON_SIZE+(BUTTON_SIZE/18)))
	{
		buttonAdd *= -1;
	}

	for (int i=0;i<Difficulty+1;i++)
	{
		buttonSize += buttonAdd;
	}

	//スキル選択がFalseだったらステージ選択
	if (!skillSlc)
	{
		if ((IsButtonGroupTriggered(KEY_GROUP::KEY_TAB_LEFT)) && (change == false) && (!SceneChange) && (!pConfig->cScreenActive))
		{
			Difficulty--;			
			change = true;
			if (Difficulty <= DIFFICULTY_NONE)
			{
				Difficulty = DIFFICULTY_MYTHIC;
				PlaySound(g_SE_ChangeA, 0);
			}
			else
			{
				PlaySound(g_SE_ChangeB, 0);
			}
			SetLeftVibration(0, VIBERATION_SPEED);
		}
		if ((IsButtonGroupTriggered(KEY_GROUP::KEY_TAB_RIGHT)) && (change == false) && (!SceneChange) && (!pConfig->cScreenActive))
		{
			Difficulty++;
			change = true;
			if (Difficulty >= DIFFICULTY_NUM)
			{
				Difficulty = DIFFICULTY_EASY;
				PlaySound(g_SE_ChangeB, 0);
			}
			else
			{
				PlaySound(g_SE_ChangeA, 0);
			}
			SetRightVibration(0, VIBERATION_SPEED);
		}

		//シーン:タイトルへ
		if ((IsButtonGroupTriggered(KEY_GROUP::KEY_BUCK)) && (change == false)&&(!SceneChange))
		{
			SceneChange = true;
			PlaySound(g_SE_Back, 0);
			SceneTransition(SCENE_TITLE);
		}

		//パネルが動いているかフラグ
		bool moving = false;
		for (int i = 0; i < STAGE_MAX; i++)
		{
			if (g_StagePanel[i].moving)
			{	moving = true;	}
		}

		//左側に移動
		if ((IsButtonGroupPressing(KEY_GROUP::KEY_LEFT, PRESSING_COUNT)) && (moving == false) && (!SceneChange) && (!pConfig->cScreenActive))
		{
			for (int i = 0; i < STAGE_MAX; i++)
			{
				g_StagePanel[i].moving = true;			//移動中
				g_StagePanel[i].NowLane++;
				g_StagePanel[i].spd = 60.0f;			//スピードを+に
				g_StagePanel[i].direction = D_RIGHT;	//右移動
				alpha = 1.0f;
				ARROW_COL[1] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				arrowSize[1] = 110.0f;
				PlaySound(g_SE_Select, 0);
			}
		}
		//右側に移動
		if ((IsButtonGroupPressing(KEY_GROUP::KEY_RIGHT, PRESSING_COUNT)) && (moving == false) && (!SceneChange) && (!pConfig->cScreenActive))
		{
			for (int i = 0; i < STAGE_MAX; i++)
			{
				g_StagePanel[i].moving = true;			//移動中
				g_StagePanel[i].NowLane--;
				g_StagePanel[i].spd = -60.0f;			//スピードを-に
				g_StagePanel[i].direction = D_LEFT;		//左移動
				alpha = 1.0f;
				ARROW_COL[0] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				arrowSize[0] = 110.0f;
				PlaySound(g_SE_Select, 0);
			}
		}

		for (int i = 0; i < STAGE_MAX; i++)
		{
			//選択移動
			if ((g_StagePanel[i].moving))
			{
				for (int m = 0; m < 16; m++)
				{
					StopSound(g_BGMNo[m]);
				}

				alpha *= 0.99f;
				g_StagePanel[i].pos.x += g_StagePanel[i].spd;	//スピードを足して移動
				g_StagePanel[i].spd *= 0.9f;			//スピード減衰
				if (g_StagePanel[i].NowLane == PLANE_3)
				{
					g_StagePanel[i].size.x += 7.0f;
					g_StagePanel[i].size.y += 7.0f;
				}
				else
				{
					g_StagePanel[i].size.x = 300.0f;
					g_StagePanel[i].size.y = 300.0f;
				}
			}

			//右移動完了
			if ((g_StagePanel[i].spd <= 15.0f) && (g_StagePanel[i].direction == D_RIGHT) && (g_StagePanel[i].moving == true))
			{
				g_StagePanel[i].pos.x = -480.0f * 2 + g_StagePanel[i].NowLane * 480.0f;	//レーンの中心に
				g_StagePanel[i].moving = false;
				//NowSelect++;	//選択ステージ変更
				ARROW_COL[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				arrowSize[1] = 100.0f;
				playing = false;
			}
			//左移動完了
			if ((g_StagePanel[i].spd >= -15.0f) && (g_StagePanel[i].direction == D_LEFT) && (g_StagePanel[i].moving == true))
			{
				g_StagePanel[i].pos.x = -480.0f * 2 + g_StagePanel[i].NowLane * 480.0f;	//レーンの中心に
				g_StagePanel[i].moving = false;
				//NowSelect--;	//選択ステージ変更
				ARROW_COL[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				arrowSize[0] = 100.0f;
				playing = false;
			}

			//選択中のステージパネルの処理
			if (g_StagePanel[i].NowLane == PLANE_3)
			{
				SsBgU[1] = 1.0f * i;
				
				g_StagePanel[i].col = D3DXCOLOR(color, color, color, 1.0f);
				if (g_StagePanel[i].moving == false)
				{
					alpha = 1.0f;
					SsBgU[0] = SsBgU[1];
				}
				NowSelect = i;
			}
			else
			{
				g_StagePanel[i].col = D3DXCOLOR(color, color, color, 0.5f);
			}

			//ステージパネルの折り返し
			if (g_StagePanel[i].NowLane == PLANE_6)
			{
				g_StagePanel[i].NowLane = PLANE_0;
			}
			if (g_StagePanel[i].NowLane == PLANE_MIN)
			{
				g_StagePanel[i].NowLane = PLANE_5;
			}
		}
	}


	//難易度 切り替え中動作
	if (change)
	{	
		for (int m = 0; m < 16; m++)
		{
			StopSound(g_BGMNo[m]);//曲停止
		}

		octRot[1] = OCT_ROT*10.0f;
		color *= 0.8f;
		if (color <= 0.01f)
		{
			ChangeDifficulty(Difficulty);

			if (ura)
			{
				ura = false;
			}
			else
			{
				ura = true;
			}


			change = false;
			playing = false;
			playFst++;
		}
	}
	else if (color < 1.0f)
	{
		color *= 1.3f;
		//octRot[1] = OCT_ROT;
		SsBgV = (float)Difficulty;
		StopVibration(0);
	}
	octRot[0] += octRot[1];

	//裏面かつ、ステージ0じゃない場合、NowSelectに+5
	//if (((ura) && NowSelect > 0)&&(!skillSlc))
	//{
	//	NowSelect += 5;
	//}
	if ((NowSelect > 0)&&(!skillSlc))
	{
		NowSelect += Difficulty * 5;
	}

	//白い四角のエフェクト
	for (int w = 0; w < WHITE_MAX; w++)
	{
		g_White[w].pos.y -= g_White[w].spd*(Difficulty+1);
		g_White[w].rot += 1.0f;
		if (g_White[w].pos.y <= SCREEN_HEIGHT * -1)
		{
			g_White[w].use = false;
		}
	}
	whiteCnt += 1;
	if (whiteCnt == 30)
	{
		SetWhite((frand() * (1920 / 4)) * 4, frand() * 5, frand() * 3);
	}
	if (whiteCnt == 60)
	{
		SetWhite((frand() * (1920 / 4)) * 1, frand() * 5, frand() * 3);
		whiteCnt = 0;
	}

	//スキル選択画面内処理
	if (skillSlc)
	{
		UpdateSkillSelect();

		PlayAlpha += 0.05f;
		if (PlayAlpha >= 1.0f)
		{
			PlayAlpha = 0.0f;
		}

		//シーン遷移
		if (((IsButtonGroupTriggered(KEY_GROUP::KEY_ENTER))) && (!pConfig->cScreenActive) && (pConfig->cFrame >= 30))
		{
			if (!SceneChange)
			{
				PlaySound(g_SE_Start, 0);
			}
			SceneChange = true;
			if (NowSelect == 0)
			{
				SceneTransition(SCENE_TUTO);
			}
			else
			{
				SceneTransition(SCENE_GAME);
			}
		}

		//選択中のステージパネルを拡大
		for (int i = 0; i < STAGE_MAX; i++)
		{
			if (g_StagePanel[i].NowLane == PLANE_3)
			{
				if (g_StagePanel[i].pos.x > CENTER_X - CENTER_X / 2.0f)
				{
					g_StagePanel[i].pos.x -= 50.0f;
					//g_StagePanel[i].pos.y -= 2.0f;
					g_StagePanel[i].size.x += 8.0f;
					g_StagePanel[i].size.y += 8.0f;
					platePosY += 4.0f;
				}
			}
		}

		//スキル選択を閉じる
		if (((IsButtonGroupTriggered(KEY_GROUP::KEY_BUCK)))&&(!SceneChange))
		{
			PlaySound(g_SE_Back, 0);
			skillSlc = false;
			UninitSkillSelect();
			platePosY = CENTER_Y + 170.0f;

			for (int i = 0; i < STAGE_MAX; i++)
			{
				if (g_StagePanel[i].NowLane == PLANE_3)
				{
					g_StagePanel[i].pos.x = CENTER_X;
					g_StagePanel[i].pos.y = STAGE_POS_Y;
					g_StagePanel[i].size = D3DXVECTOR2(390.0f, 390.0f);
					
				}
			}
		}
	}

	//スキルセレクト画面に切り替え
	if ((IsButtonGroupTriggered(KEY_GROUP::KEY_ENTER)) && (!skillSlc) && (!change) && (!g_StagePanel[0].moving) && (color >= 1.0f) && (!SceneChange) && (StageSelectFrame >= 60) && (!pConfig->cScreenActive) && (pConfig->cFrame >= 30))
	{
		skillSlc = true;
		InitSkillSelect();
		PlaySound(g_SE_Stage, 0);
	}

	//uiText Boss 動作
	SetBossNameText();
	STAGE_PANEL nowPanel = GetNowSelect();
	D3DXVECTOR2 uiTextPos = { nowPanel.pos.x, platePosY };
	g_UITextBoss->SetPosition(uiTextPos);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStageSelect(void)
{
	// 背景(1)
	DrawSpriteColor(g_TextureStagePanel, CENTER_X, CENTER_Y, SCREEN_WIDTH * 1.1, SCREEN_WIDTH * 1.1,
		(1.0f / 6.0f) * SsBgU[1], (1.0f / 3.0f) * SsBgV, 1.0f / 6.0f, 1.0f / 3.0f, D3DXCOLOR(color * 0.6f, color * 0.6f, color * 0.6f, 1.0f));
	// 背景(2)
	DrawSpriteColor(g_TextureStagePanel, CENTER_X, CENTER_Y, SCREEN_WIDTH * 1.1, SCREEN_WIDTH * 1.1,
		(1.0f / 6.0f) * SsBgU[0], (1.0f / 3.0f) * SsBgV, 1.0f / 6.0f, 1.0f / 3.0f, D3DXCOLOR(color * 0.6f, color * 0.6f, color * 0.6f, alpha));

	if (color >= 1.0f)
	{
		if (!skillSlc)
		{
			// 矢印(左)
			DrawSpriteColor(g_TextureArrow, CENTER_X - 325.0f, STAGE_POS_Y + 270.0f, arrowSize[1], arrowSize[1],
				0.0f, 0.0f, 0.25f, 1.0f, ARROW_COL[1]);
			// 矢印(右)
			DrawSpriteColor(g_TextureArrow, CENTER_X + 325.0f, STAGE_POS_Y + 270.0f, arrowSize[0], arrowSize[0],
				0.25f, 0.0f, 0.25f, 1.0f, ARROW_COL[0]);
			// PLAYパネル
			DrawSpriteColor(g_TextureUIPlay, CENTER_X, CENTER_Y * 1.6f, 480.0f, 160.0f,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		
			// 操作ガイド
			if (GetCtrl())
			{	
				// ガイド:キー　(PLAY)
				DrawSpriteColor(g_TextureUIKey, CENTER_X + 180.0f, CENTER_Y * 1.6f, buttonSize * 0.7f, buttonSize * 0.7f,
					0.167f * 3.0f, 0.249f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				// ガイド:ボタン(PLAY)
				DrawSpriteColor(g_TextureUIButton, CENTER_X + 180.0f, CENTER_Y * 1.6f, buttonSize * 0.7f, buttonSize * 0.7f,
					0.167f * 2.0f, 0.249f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		// 全ステージパネルの表示
		for (int i = 0; i < STAGE_MAX; i++)
		{
			DrawSpriteColor(g_TextureStagePanel, g_StagePanel[i].pos.x, g_StagePanel[i].pos.y, g_StagePanel[i].size.x, g_StagePanel[i].size.y,
				(1.0f / 6.0f) * i, (1.0f / 3.0f) * Difficulty, 1.0f / 6.0f, 1.0f / 3.0f, g_StagePanel[i].col);
		}
		// スキル選択時 暗転
		if (skillSlc)
		{
			DrawSpriteColor(g_TextureWhite, CENTER_X, CENTER_Y, SCREEN_WIDTH, SCREEN_WIDTH,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f));
		}
	}
	
	if (!skillSlc)
	{
		// 六角形(1)
		GetDeviceContext()->PSSetShaderResources(0, 1,
			GetTexture(g_TextureOct));
		DrawSpriteColorRotation(
			CENTER_X, CENTER_Y, 2200, 2200,octRot[0],
			OCT_COL,
			0.0f,1.0f,1.0f,1);

		// 六角形(2)
		GetDeviceContext()->PSSetShaderResources(0, 1,
			GetTexture(g_TextureOct));
		DrawSpriteColorRotation(
			CENTER_X, CENTER_Y, 2200, 2200,octRot[0] * -1,
			OCT_COL,
			0.0f,1.0f,1.0f,1);
	}
	// 白い四角エフェクト
	for (int i = 0; i < WHITE_MAX; i++)
	{
		if (g_White[i].use == true)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1,
				GetTexture(g_TextureWhite));
			DrawSpriteColorRotation(
				g_White[i].pos.x,
				g_White[i].pos.y,
				(int)g_White[i].size.x, 
				(int)g_White[i].size.y,
				g_White[i].rot,WHITE_COL,
				0.0f,1.0f,1.0f,1);
		}
	}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------- スキルセレクト時 ------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------
	if (skillSlc)
	{
		// スキルセレクト
		DrawSkillSelect();
		
		// BACK(フレーム)
		DrawSpriteColor(g_TextureUIFrame, BACK_POS_X * 3.8f, 100.0f, 300.0f, 150.0f,
			0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// BACK(矢印)
		DrawSpriteColor(g_TextureArrowBack, BACK_POS_X + (buttonSize / 2.0f) - 20.0f, 100.0f, BUTTON_SIZE*1.0f, BUTTON_SIZE*1.0f,
			0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		
		// 操作ガイド
		if (GetCtrl())
		{
			// ガイド:キー　(BACK)
			DrawSpriteColor(g_TextureUIKey, BACK_POS_X + BUTTON_SIZE * 1.8f - 60.0f, 100.0f, buttonSize * 0.8f, buttonSize * 0.8f,
				0.167f * 3.0f, 0.249f*2.01f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// ガイド:ボタン(BACK)
			DrawSpriteColor(g_TextureUIButton, BACK_POS_X + BUTTON_SIZE * 1.8f - 60.0f, 100.0f, buttonSize * 0.8f, buttonSize * 0.8f,
				0.167f * 1.0f, 0.249f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// 選択中
		for (int i = 0; i < STAGE_MAX; i++)
		{
			if ((g_StagePanel[i].NowLane == PLANE_3))
			{
				// PLAYパネル (基本)
				DrawSpriteColor(g_TextureUIPlay, g_StagePanel[i].pos.x, CENTER_Y * 1.69f, g_StagePanel[i].size.x*1.5f, (g_StagePanel[i].size.x/3.0f)*1.5f,
					0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// PLAYパネル (エフェクト)
				DrawSpriteColor(g_TextureUIPlay, g_StagePanel[i].pos.x, CENTER_Y * 1.69f, (g_StagePanel[i].size.x * 1.5f)+(PlayAlpha*30.0f), ((g_StagePanel[i].size.x / 3.0f) * 1.5f)+(PlayAlpha * 30.0f),
					0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(PlayAlpha+0.5f, PlayAlpha+0.5f, PlayAlpha, 1.0f - PlayAlpha));
				
				// 操作ガイド
				if(GetCtrl())
				{
					// ガイド:キー　(PLAY)
					DrawSpriteColor(g_TextureUIKey, g_StagePanel[i].pos.x + 240.0f, CENTER_Y * 1.69f, buttonSize * 0.7f * 1.5f, buttonSize * 0.7f * 1.5f,
						0.167f * 3.0f, 0.249f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{
					// ガイド:ボタン(PLAY)
					DrawSpriteColor(g_TextureUIButton, g_StagePanel[i].pos.x + 240.0f, CENTER_Y * 1.69f, buttonSize * 0.7f * 1.5f, buttonSize * 0.7f * 1.5f,
						0.167f * 2.0f, 0.249f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}

				// 選択中 ステージパネル
				DrawSpriteColor(g_TextureStagePanel, g_StagePanel[i].pos.x, g_StagePanel[i].pos.y, g_StagePanel[i].size.x, g_StagePanel[i].size.y,
					(1.0f / 6.0f)* i, (1.0f / 3.0f) * SsBgV, 1.0f / 6.0f, 1.0f / 3.0f, g_StagePanel[i].col);

			}
		}
	}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------非スキルセレクト時 && 非チェンジ中---------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------

	if (color >= 1.0f)
	{
		if (!skillSlc)
		{
			// BACK(フレーム)
			DrawSpriteColor(g_TextureUIFrame, BACK_POS_X * 3.05f, 100.0f, 260.0f, 130.0f,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			// BACK(矢印)
			DrawSpriteColor(g_TextureArrowBack, BACK_POS_X + (buttonSize / 5.0f)-10.0f, 100.0f, BUTTON_SIZE, BUTTON_SIZE,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			
			// 操作ガイド
			if (GetCtrl())
			{
				// ガイド:キー(BACK)
				DrawSpriteColor(g_TextureUIKey, BACK_POS_X + BUTTON_SIZE * 1.1f - 10.0f, 100.0f, buttonSize * 0.7f, buttonSize * 0.7f,
					0.167f * 3.0f, 0.249f * 2.01f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// ガイド:キー(難易度 下降)
				DrawSpriteColor(g_TextureUIKey, CENTER_X - (HARD_SIZE_X*1.62f), HARD_POS_Y, buttonSize * 0.5f, buttonSize * 0.5f,
					0.167f * 1.0f, 0.249f * 2.01f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// ガイド:キー(難易度 上昇)
				DrawSpriteColor(g_TextureUIKey, CENTER_X + (HARD_SIZE_X*1.62f), HARD_POS_Y, buttonSize * 0.5f, buttonSize * 0.5f,
					0.167f * 2.0f, 0.249f * 2.01f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				// ガイド:ボタン(BACK)
				DrawSpriteColor(g_TextureUIButton, BACK_POS_X + BUTTON_SIZE * 1.1f - 10.0f, 100.0f, buttonSize * 0.7f, buttonSize * 0.7f,
					0.167f * 1.0f, 0.249f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// ガイド:ボタン(難易度 下降)
				DrawSpriteColor(g_TextureUIButton, CENTER_X - (HARD_SIZE_X * 1.62f), HARD_POS_Y, buttonSize * 0.5f, buttonSize * 0.5f,
					0.167f * 1.0f, 0.249f * 2.01f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// ガイド:ボタン(難易度 下降)
				DrawSpriteColor(g_TextureUIButton, CENTER_X + (HARD_SIZE_X * 1.62f), HARD_POS_Y, buttonSize * 0.5f, buttonSize * 0.5f,
					0.167f * 2.0f, 0.249f * 2.01f, 0.167f, 0.249f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------難易度パネル-------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------
			
			// 難易度パネル(フレーム)
			DrawSpriteColor(g_TextureUIHardFrame, CENTER_X - HARD_SIZE_X, HARD_POS_Y, HARD_SIZE_X, HARD_SIZE_Y,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			DrawSpriteColor(g_TextureUIHardFrame, CENTER_X, HARD_POS_Y, HARD_SIZE_X, HARD_SIZE_Y,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			DrawSpriteColor(g_TextureUIHardFrame, CENTER_X + HARD_SIZE_X, HARD_POS_Y, HARD_SIZE_X, HARD_SIZE_Y,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));

			// 難易度パネル(テキスト(選択))
			switch (Difficulty)
			{
			// EASY
			case 0:
				HARD_COL = D3DXCOLOR(0.0f, 1.0f, 0.0f, ((buttonSize / 150.0f) - 0.3f) * 1.1f);
				DrawSpriteColor(g_TextureUIHardFrame, CENTER_X - HARD_SIZE_X, HARD_POS_Y, HARD_SIZE_X, HARD_SIZE_Y,
					0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			// HARD
			case 1:
				HARD_COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, ((buttonSize / 150.0f) - 0.3f) * 1.1f);
				DrawSpriteColor(g_TextureUIHardFrame, CENTER_X, HARD_POS_Y, HARD_SIZE_X, HARD_SIZE_Y,
					0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			// MYTHIC
			case 2:
				HARD_COL = D3DXCOLOR(1.0f, 0.0f, 1.0f, ((buttonSize / 150.0f) - 0.3f) * 1.1f);
				DrawSpriteColor(g_TextureUIHardFrame, CENTER_X + HARD_SIZE_X, HARD_POS_Y, HARD_SIZE_X, HARD_SIZE_Y,
					0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			
			// DEFAULT
			default:
				HARD_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			}

			for (int i = 0; i < DIFFICULTY_NUM; i++)
			{	g_UITextDifficulty[i]->Draw();	}	//UIText Difficulty 描画処理
			
			DrawSpriteColor(g_TextureUIHardFade, CENTER_X + (HARD_SIZE_X * (Difficulty - 1)), HARD_POS_Y, HARD_SIZE_X, HARD_SIZE_Y,
				0.0f, 0.0f, 1.0f, 1.0f, HARD_COL);	// 難易度パネル(選択フェード)

		}
		g_UITextBoss->Draw();	//UIText BossName 描画処理
	}

	
}

//　選択中ステージの値を返す
int GetGemeStageNum(void)
{
	return NowSelect;
}

//　白エフェクトを返す
void SetWhite(float x,float sz,float rt)
{
	for (int i = 0; i < WHITE_MAX; i++)
	{
		if (g_White[i].use == false)
		{
			g_White[i].pos.x = x;
			g_White[i].pos.y = SCREEN_HEIGHT*1.1;
			g_White[i].size.x = sz * 8.0f;
			g_White[i].size.y = sz * 8.0f;
			g_White[i].rot = rt * 45.0f;
			g_White[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_White[i].use = true;
			return;
		}
	}
}

// 旧 裏面かどうかを返す
bool GetUra()
{
	if (ura)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 難易度を返す
int GetDifficulty()
{
	return Difficulty;
}

// シーンチェンジを
bool GetSceneChange()
{
	if (SceneChange)
	{
		return true;
	}
	else
	{
		return false;
	}
}

STAGE_PANEL GetNowSelect()
{
	STAGE_PANEL panel = {};
	// 選択中ネームプレート
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (g_StagePanel[i].NowLane == PLANE_3)
		{
			panel = g_StagePanel[i];
		}
	}
	return panel;
}

void ChangeDifficulty(int difficulty)
{
	for (int i = 0; i < DIFFICULTY_NUM; i++)
	{	g_UITextDifficulty[i]->SetBright(0.25f);	}	//すべての難易度テキストの輝度を下げる
	g_UITextDifficulty[difficulty]->SetBright(1.0f);//選択中の難易度テキストの輝度を上げる
	
	switch (difficulty)
	{
	case DIFFICULTY_EASY:
		WHITE_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		OCT_COL = D3DXCOLOR(0.0f, 0.4f, 1.0f, 0.2f);
		octRot[1] = 0.3f;
		break;
	case DIFFICULTY_HARD:
		WHITE_COL = D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f);
		OCT_COL = D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.2f);
		octRot[1] = 3.0f;
		break;
	case DIFFICULTY_MYTHIC:
		WHITE_COL = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
		OCT_COL = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.2f);
		octRot[1] = 5.0f;
		break;
	default:
		WHITE_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		OCT_COL = D3DXCOLOR(0.0f, 0.4f, 1.0f, 0.2f);
		octRot[1] = 0.3f;
		break;
	}
}

void SetBossNameText()
{
	int nowStage = NowSelect;

	switch (NowSelect)
	{
	case 0:
		g_UITextBoss->SetText((char*)"TUTORIAL");
		break;

	case 1:
		g_UITextBoss->SetText((char*)"ZARI-GANI");
		break;

	case 2:
		g_UITextBoss->SetText((char*)"UNAGI");
		break;

	case 3:
		g_UITextBoss->SetText((char*)"TAKO");
		break;

	case 4:
		g_UITextBoss->SetText((char*)"SAME");
		break;

	case 5:
		g_UITextBoss->SetText((char*)"SHISHAMO");
		break;

	case 6:
		g_UITextBoss->SetText((char*)"CURSE OFnZARI-GANI");
		break;

	case 7:
		g_UITextBoss->SetText((char*)"UNA-JU");
		break;

	case 8:
		g_UITextBoss->SetText((char*)"SHIOKARAnMUSHA");
		break;

	case 9:
		g_UITextBoss->SetText((char*)"JAWGE");
		break;

	case 10:
		g_UITextBoss->SetText((char*)"FULL ARMORnZARI-GANI");
		break;

	case 11:
		g_UITextBoss->SetText((char*)"PROTTYPE");
		break;

	case 12:
		g_UITextBoss->SetText((char*)"UNAGYDRA");
		break;

	case 13:
		g_UITextBoss->SetText((char*)"REVENGEnALLIANCE");
		break;

	case 14:
		g_UITextBoss->SetText((char*)"CURSE OFnJAWGE");
		break;

	case 15:
		g_UITextBoss->SetText((char*)"GANESHAMO");
		break;

	default:
		break;
	}
}