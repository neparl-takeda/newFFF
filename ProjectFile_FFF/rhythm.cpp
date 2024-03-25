//====================================================
//リズム
//作成：勝沼力樹
//		2022/10/26
//====================================================
#include "rhythm.h"
#include "sound.h"
#include "game.h"
#include "texture.h"
#include "sprite.h"
#include "main.h"
#include "keyboard.h"
#include "enemybullet.h"
#include "effect.h"

#include "Config.h"

#include "inputx.h"
#include "UIText.h"

//曲のBPMのパターン
#define		BPM1				(150)
#define		BPM2				(120)
#define		BPM3				(90)
#define		BPM4				(180)
#define		BPM5				(165)

int			NowBPM;
//float		NowBPM;
int			GameSoundNo;

int			NotesNum = 1;
int			Frame;

float		sp;
float		NotesT = 0.0f;
int			errors = 0;
int			BGMError;
bool		Rhythmflg = false;
float		Gradationalf;
int			alfnum;
float		tutorialerror;
NOTES		Notes[NOTES_MAX];
NOTESLANE	NotesLane;

UIText* g_UITextDebugBPM{};

//テクスチャ情報の保存変数
static	ID3D11ShaderResourceView	*g_TextureNotes;
static	char	*g_TextureNameNotesRight = NOTES_TEX_RIGHT;//テクスチャ名
static	char	*g_TextureNameNotesLeft  = NOTES_TEX_LEFT;//テクスチャ名
static	char	*g_TextureNameNotesCenter= NOTES_TEX_CENTER;//テクスチャ名
static	ID3D11ShaderResourceView	*g_TextureNotesLane;
static	char	*g_TextureNameNotesLane = NOTESLANE_TEX;//テクスチャ名
static	int		g_TextureNameStageProg;
static	int		g_TextureNameProgFlag;
static	int		g_TextureNameRunPlayer;
static	int		g_TextureNameGradation;

int Notestipindex1, Notestipindex2,indexNum;
int Notestip[10][10]
{//15フレームごと
	{1,0,1,0,1,0,1,0},
	{1,0,1,0,1,0,1,0},
	{1,0,1,0,1,0,1,0},
};
//BPM90は24フレームで一個
HRESULT InitRhythm(int stagenum)
{
	char	filename0[]  = "data\\BGM\\00_Tutorial_120.wav";
	char	filename1[]  = "data\\BGM\\01_Zarigani_120.wav";
	char	filename2[]  = "data\\BGM\\02_Unagi_120.wav";
	char	filename3[]  = "data\\BGM\\03_Tako_120.wav";
	char	filename4[]  = "data\\BGM\\04_Shark_120.wav";
	char	filename5[]  = "data\\BGM\\05_Shishamo_150.wav";
	char	filename6[]  = "data\\BGM\\06_Zarigani_150.wav";
	char	filename7[]  = "data\\BGM\\07_Unadon_150.wav";
	char	filename8[]  = "data\\BGM\\08_Shiokara_150.wav";
	char	filename9[]  = "data\\BGM\\09_Jawge_150.wav";
	char	filename10[] = "data\\BGM\\10_FullArmor_150.wav";
	char	filename11[] = "data\\BGM\\11_Prottype_150.wav";
	char	filename12[] = "data\\BGM\\12_Unagydra_150.wav";
	char	filename13[] = "data\\BGM\\13_Revenge_150.wav";
	char	filename14[] = "data\\BGM\\14_Jawge_150.wav";
	char	filename15[] = "data\\BGM\\15_Ganeshamo_150.wav";
	//char	filename15[] = "data\\BGM\\test.wav";

	tutorialerror = 0;
	switch (stagenum)
	{
	case 0://チュートリアル

		GameSoundNo = LoadSound(filename0);

		Notestipindex1 = 1;
		indexNum = 8;
		errors = 0;
		sp = 10.8f;
		NowBPM = BPM2;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = -2400;
		tutorialerror = 0.01f;
		break;
	case 1://ザリガニ

		GameSoundNo = LoadSound(filename1);

		Notestipindex1 = 1;
		indexNum = 8;
		errors = 0;
		sp = 10.8f;
		NowBPM = BPM2;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = -60;
		break;
	case 2://ウナギ

		GameSoundNo = LoadSound(filename2);

		Notestipindex1 = 1;
		indexNum = 8;
		errors = 2;
		sp = 10.8f;
		NowBPM = BPM2;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = 0;
		break;
	case 3://タコ

		GameSoundNo = LoadSound(filename3);

		Notestipindex1 = 1;
		indexNum = 8;
		errors = -4;
		sp = 10.8f;
		NowBPM = BPM2;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = 430;
		break;
	case 4://サメ


		GameSoundNo = LoadSound(filename4);

		Notestipindex1 = 1;
		indexNum = 8;
		errors = 0;
		sp = 10.8f;
		NowBPM = BPM2;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = 0;
		break;

	case 5://シシャモ

		GameSoundNo = LoadSound(filename5);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = 0;
		sp = 12.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = -20;
		break;
	case 6://ザリガニ裏	//抜け殻

		GameSoundNo = LoadSound(filename6);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = 0;
		sp = 12.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = 10;
		break;
	case 7://ウナギ裏	//うな重

		GameSoundNo = LoadSound(filename7);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = 0;
		sp = 12.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = -10;
		break;
	case 8://タコ裏		//シオカラ武者

		GameSoundNo = LoadSound(filename8);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = 0;
		sp = 12.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = -150;
		break;
	case 9://サメ裏		//ジョージ

		GameSoundNo = LoadSound(filename9);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = -1;
		sp = 12.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = -170;
		break;
	case 10://シシャモ裏 //フルアーマーザリガニ

		GameSoundNo = LoadSound(filename10);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = -1;
		sp = 12.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = 250;
		break;
	case 11://MYTHIC //プロトタイプザリガニ

		GameSoundNo = LoadSound(filename11);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = 0;
		sp = 12.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = 500;
		break;

	case 12://MYTHIC //ウナギドラ

		GameSoundNo = LoadSound(filename12);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = -1;	//現行
		sp = 12.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = 120;
		break;

	case 13://MYTHIC //復讐

		GameSoundNo = LoadSound(filename13);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = -1;
		sp = 12.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = 120;
		break;

	case 14://MYTHIC //カース ジョージ

		GameSoundNo = LoadSound(filename14);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = 0;
		sp = 10.8f;
		NowBPM = BPM4;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = -755;
		break;

	case 15://MYTHIC //ガネーシャモ
		
		GameSoundNo = LoadSound(filename15);

		//Notestipindex1 = 2;
		//indexNum = 8;
		//errors = 0;
		//sp = 12.0f;
		//NowBPM = BPM1;
		//NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		//BGMError = -20;
		Notestipindex1 = 2;
		indexNum = 8;
		errors = 0;
		sp = 10.8f;
		NowBPM = BPM4;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = -525;
		break;

	default:

		GameSoundNo = LoadSound(filename11);

		Notestipindex1 = 2;
		indexNum = 8;
		errors = -1;
		sp = 13.0f;
		NowBPM = BPM1;
		NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		BGMError = 250;
		break;
	}

	for (int i = 0; i < NOTES_MAX; i += 2) 
	{
		//ノーツ左の初期化
			Notes[i].pos = D3DXVECTOR2(NOTES_POS_X_1, NOTES_POS_Y);
			Notes[i].size = D3DXVECTOR2(NOTES_SIZE_X, NOTES_SIZE_Y);
			Notes[i].sp = D3DXVECTOR2(sp, 0.0f);
			Notes[i].rot = 0.0f;
			Notes[i].alpha = 1.0f;
			Notes[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - Notes[i].alpha);
			Notes[i].use = false;
			Notes[i].num = -1;


			//テクスチャのロード
			Notes[i].texno = (float)LoadTexture(g_TextureNameNotesLeft);
			if (Notes[i].texno == -1)
			{//ロードエラー
				exit(999);
			}
	
		//ノーツ右の初期化
			Notes[i + 1].pos = D3DXVECTOR2(NOTES_POS_X_2, NOTES_POS_Y);
			Notes[i + 1].size = D3DXVECTOR2(NOTES_SIZE_X, NOTES_SIZE_Y);
			Notes[i + 1].sp = D3DXVECTOR2(-sp, 0.0f);
			Notes[i + 1].rot = 0.0f;
			Notes[i + 1].alpha = 1.0f;
			Notes[i + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - Notes[i + 1].alpha);
			Notes[i + 1].use = false;
			Notes[i + 1].num = -1;

			//テクスチャのロード
			Notes[i+1].texno = (float)LoadTexture(g_TextureNameNotesRight);
			if (Notes[i+1].texno == -1)
			{//ロードエラー
				exit(999);
			}		
	}
	//ノーツレーンの初期化
	NotesLane.pos = D3DXVECTOR2(NOTESLANE_POS_X, NOTESLANE_POS_Y);
	NotesLane.size = D3DXVECTOR2(NOTESLANE_SIZE_X, NOTESLANE_SIZE_Y);
	NotesLane.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャのロード
	NotesLane.texno = (float)LoadTexture(g_TextureNameNotesLane);
	if (NotesLane.texno == -1)
	{//ロードエラー
		exit(999);
	}
	NotesLane.texcent = (float)LoadTexture(g_TextureNameNotesCenter);
	if (NotesLane.texcent == -1)
	{//ロードエラー
		exit(999);
	}

	g_TextureNameRunPlayer = (int)LoadTexture((char*)"data/TEXTURE/player.png");
	if (g_TextureNameRunPlayer == -1)
	{
		exit(999);
	}
	g_TextureNameStageProg = (int)LoadTexture((char*)"data/TEXTURE/UI_StageProg.png");
	if (g_TextureNameStageProg == -1)
	{
		exit(999);
	}
	g_TextureNameProgFlag = (int)LoadTexture((char*)"data/TEXTURE/UI_ProgFlag_B.png");
	if (g_TextureNameProgFlag == -1)
	{
		exit(999);
	}
	g_TextureNameGradation = (int)LoadTexture((char*)"data/TEXTURE/gradation.jpg");
	if (g_TextureNameGradation == -1)
	{
		exit(999);
	}
	
	alfnum = 1;
	Gradationalf = 0.0f;
	Frame = 0;
	
	Notestipindex2 = 0;

	if (GetDebugFlag())
	{
		//UiText "DebugBPM"
		g_UITextDebugBPM = new UIText;
		g_UITextDebugBPM->Init();
		D3DXVECTOR2 uiTextPos	= { 300.0f, 800.0f };
		D3DXVECTOR2 uiTextSize	= { 100.0f ,100.0f };
		D3DXCOLOR   uiTextColor = { 1.0f,0.0f,0.0f,1.0f };
		g_UITextDebugBPM->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"000", FONT_NORMAL);
	}
	return	S_OK;
}

void UpdateRhythm()
{
	if (GetDebugFlag())
	{
		//デバッグ用
		if (IsButtonGroupTriggered(KEY_GROUP::KEY_UP))
		{
			NowBPM++;
			NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		}
		//デバッグ用
		if (IsButtonGroupTriggered(KEY_GROUP::KEY_DOWN))
		{
			NowBPM--;
			NotesT = (60.0f / (NowBPM / 60.0f)) / 2.0f;
		}
	}



	CONFIG* pConfig = GetConfig();

	if (Frame == 120)
	{
		PlaySound(GameSoundNo, 0);
		SetVolume(GameSoundNo, pConfig->cSoundBGMv);
	}
		SetVolume(GameSoundNo, pConfig->cSoundBGMv);

	Frame++;
	if (Frame >= 120)
	{
		if (((Frame - errors) % (int)NotesT) == 0.0f)
		{
			if (Notestip[Notestipindex1][Notestipindex2 % indexNum] == 1)
			{
				Rhythmflg = true;
				Gradationalf = 0.0f;
				alfnum = 1;
				SetNotes();
			}
			if ((Frame - errors) % ((int)NotesT * 4)  == 0.0f) {
				SETBULLET();
			}
			Notestipindex2++;
		}

		for (int i = 0; i < NOTES_MAX; i++)
		{
			if (!Notes[i].use) continue;

			Notes[i].pos.x += Notes[i].sp.x;
			Notes[i].alpha -= 0.01f * NOTES_DIST;
			if (i % 2 == 0)
			{
				if (Notes[i].pos.x + NOTES_SIZE_X / 2 >= SCREEN_WIDTH / 2 - NOTES_SIZE_X / 2) {
 					Notes[i].alpha += 0.065f * NOTES_DIST;
					Notes[i+1].alpha += 0.065f * NOTES_DIST;
				}
				//ノーツ左が真ん中に来た時両方消える
				if (Notes[i].pos.x + NOTES_SIZE_X / 2 >= SCREEN_WIDTH / 2 + 25.0f)
				{
					Notes[i].use = false;
					Notes[i + 1].use = false;
				}
			}
		}
	}
	if (Rhythmflg) 
	{
		if (Gradationalf > 15.0f) 
		{
			alfnum *= -1;
		}
		Gradationalf += 1.5f * alfnum;
		if (Gradationalf < 0.0f) 
		{
			Rhythmflg = false;
		}
	}

}

void UninitRhythm()
{
	if (g_TextureNotes)
	{
		g_TextureNotes->Release();
		g_TextureNotes = NULL;
	}
	if (g_TextureNotesLane)
	{
		g_TextureNotesLane->Release();
		g_TextureNotesLane = NULL;
	}

	StopSound(GameSoundNo);
}

void DrawRhythm()
{
	//ノーツレーンの表示
	DrawSpriteColor((int)(NotesLane.texno), NotesLane.pos.x, NotesLane.pos.y, NotesLane.size.x, NotesLane.size.y,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//ノーツ 中心
	DrawSpriteColor((int)(NotesLane.texcent), NotesLane.pos.x, NotesLane.pos.y, NOTES_SIZE_X * 3 * 1.01f, NOTES_SIZE_Y * 1.01f,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f));
	for (int i = 0; i < NOTES_MAX; i++)
	{
		if (!Notes[i].use) continue;

		DrawSpriteColor((int)Notes[i].texno, Notes[i].pos.x, Notes[i].pos.y, Notes[i].size.x, Notes[i].size.y,
			0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - Notes[i].alpha));
	}
	DrawSpriteColor(g_TextureNameStageProg, 1700.0f, 470.0f, 40.0f, 440.0f,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DrawSpriteColor(g_TextureNameProgFlag, 1710.0f, 240.0f, 90.0f, 90.0f,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(0.3f, 0.9f, 0.3f, 1.0f));
	DrawSpriteColor(g_TextureNameRunPlayer, 1700.0f, 700.0f - ((Frame - 120) * (0.06f - ((float)BGMError / 120000)+ tutorialerror)), 20.0f, 60.0f,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (GetDebugFlag())
	{
		TranslationTextDebugBPM();
		g_UITextDebugBPM->Draw();		//UIText "DEBUG BPM"  描画
	}
}

void SetNotes()
{
	for (int n = 0; n < NOTES_MAX; n += 2)
	{
		if (!Notes[n].use)
		{
			Notes[n].pos = D3DXVECTOR2(NOTES_POS_X_1, NOTES_POS_Y);
			Notes[n].use = true;
			Notes[n].num = NotesNum;
			Notes[n].alpha = 1.0f;

			Notes[n + 1].pos = D3DXVECTOR2(NOTES_POS_X_2, NOTES_POS_Y);
			Notes[n + 1].use = true;
			Notes[n + 1].alpha = 1.0f;
			Notes[n + 1].num = NotesNum;
			NotesNum++;

			return;
		}
	}
}

bool GetRhythm()
{//リズムに合っているかの判定
	//ノーツの座標で判定版
	for (int i = 0; i < NOTES_MAX; i += 2) {
		if (Notes[i].use) {
			if (Notes[i].pos.x < (880 + (Notes[i].sp.x * 5.0f)) && Notes[i].pos.x >(880 - (Notes[i].sp.x * 3.0f))) {
				return true;
			}
		}
	}
	return	false;
}

NOTES * GetNotes()
{
	return Notes;
}

int  GetFreame()
{
	return Frame;
}

void ReleaseNotes()
{//一番真ん中に近いノーツを消す関数

	for (int i = 0; i < NOTES_MAX; i += 2) {
		if (!Notes[i].use) continue;
		if (Notes[i].pos.x < (880 + (Notes[i].sp.x * 6.0f)) && Notes[i].pos.x >(880 - (Notes[i].sp.x * 6.0f))) {
			SetEffect(EFFECT_4, Notes[i].pos, Notes[i].size+ D3DXVECTOR2(130.0f,130.0f));
			SetEffect(EFFECT_4, Notes[i].pos, Notes[i].size + D3DXVECTOR2(130.0f, 130.0f));
			SetEffect(EFFECT_4, Notes[i].pos, Notes[i].size + D3DXVECTOR2(130.0f, 130.0f));
			SetEffect(EFFECT_3, Notes[i + 1].pos, Notes[i + 1].size + D3DXVECTOR2(130.0f, 130.0f));
			SetEffect(EFFECT_3, Notes[i + 1].pos, Notes[i + 1].size + D3DXVECTOR2(130.0f, 130.0f));
			SetEffect(EFFECT_3, Notes[i + 1].pos, Notes[i + 1].size + D3DXVECTOR2(130.0f, 130.0f));
			Notes[i].use = false;
			Notes[i + 1].use = false;
			break;
		}
	}
}

bool	MusicEnd()
{
	if ((Frame - 120) < 120 * 60 + BGMError) {
		return false;
	}
	else {
		return true;
	}
}

int BGM_RE()
{
	return GameSoundNo;
}
float GetGradation()
{
	return Gradationalf;
}

void TranslationTextDebugBPM()
{
	char scoreChar[256] = "---";

	int bpm  = NowBPM;
	int bpm2 = NowBPM;

	int a[3] = { bpm / 100,(bpm - 100) / 10 , bpm};

	a[0] = bpm / 100;
	bpm -= a[0] * 100;
	a[1] = bpm / 10;
	bpm -= a[1] * 10;
	a[2] = bpm;
	//a[0] = (int)((bpm / 100) * 100);
	//bpm - a[0];
	//a[1] = (int)((bpm / 10) * 10);
	//bpm - a[1];
	//a[2] = (int)((bpm / 1) * 1);
	//bpm - a[2];

	for (int i = 0; i < 3; i++)
	{
		scoreChar[i] = a[i] + ASCII_NUM_0;
	}
	g_UITextDebugBPM->SetText(scoreChar);
}