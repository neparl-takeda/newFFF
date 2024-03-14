#include "combo.h"
#include "texture.h"
#include "sprite.h"

#include "UIText.h"

#define		COMBO_DIGIT				(3)

#define		COMBO_TEXT_POS_Y		(CENTER_Y - 130.0f)
#define		COMBO_TEXT_SIZE			(60.0f)

#define		NUM_TEXT_SIZE			(135.0f)

#define		COMBOMAG_TEXT_SIZE		(70.0f)
#define		COMBOMAG_POS_X			(CENTER_X)
#define		COMBOMAG_POS_Y			(CENTER_Y + 130.0f)

#define		COMBO_TEXT_R_SIZE		(40.0f)
#define		COMBO_TEXT_R_POS_X		(1475.0f)
#define		COMBO_TEXT_R_POS_Y		(CENTER_Y - 30.0f)

#define		COMBONUM_TEXT_R_SIZE	(60.0f)
#define		COMBONUM_TEXT_R_POSX	(1380.0f)
#define		COMBONUM_TEXT_R_POSY	(COMBO_TEXT_R_POS_Y + COMBONUM_TEXT_R_SIZE)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
COMBO	g_Combo[COMBO_DIGIT],g_ComboMag[4];

D3DXCOLOR	MagColor[5]{
	D3DXCOLOR(0.6f,0.6f,0.6f,0.2f),
	D3DXCOLOR(1.0f,1.0f,1.0f,0.2f),
	D3DXCOLOR(0.0f,1.0f,0.0f,0.2f),
	D3DXCOLOR(1.0f,1.0f,0.0f,0.2f),
	D3DXCOLOR(1.0f,0.0f,0.0f,0.2f),
};

int		ComboAdd;
int		ComboMagNum;
int		ComboMax;

//UIText* g_UITextMax		{};
UIText* g_UITextCombo	{};
UIText* g_UITextComboNum{};
UIText* g_UITextComboMag{};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCombo()
{
	for (int i = 0; i < 4; i++)
	{	g_ComboMag[i].combonum = 0;			}//コンボ倍率初期化	//？コンボの倍率一桁抽出？　

	g_ComboMag[1].combonum = 18;
	g_ComboMag[2].combonum = 1;
	g_ComboMag[3].combonum = 16;

	for (int i = 0; i < COMBO_DIGIT; i++)
	{	g_Combo[i].combonum = 0;		}	//デフォルトコンボ数

	ComboAdd	= 0;
	ComboMax	= 0;
	ComboMagNum = 0;
	
	//UiText Combo
	g_UITextCombo = new UIText;
	g_UITextCombo->Init();
	D3DXVECTOR2	uiTextPos	= { CENTER_X, COMBO_TEXT_POS_Y };
	D3DXVECTOR2	uiTextSize	= { COMBO_TEXT_SIZE ,COMBO_TEXT_SIZE };
	D3DXCOLOR	uiTextColor = { 1.0f,1.0f,1.0f,0.2f };
	g_UITextCombo->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"COMBO", FONT_NORMAL);

	//UiText ComboNum 000
	g_UITextComboNum = new UIText;
	g_UITextComboNum->Init();
	uiTextPos	= { CENTER_X, CENTER_Y };
	uiTextSize	= { NUM_TEXT_SIZE ,NUM_TEXT_SIZE };
	uiTextColor = { 1.0f,1.0f,1.0f,0.2f };
	g_UITextComboNum->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"000", FONT_NORMAL);

	//UiText ComboMag *0.0
	g_UITextComboMag = new UIText;
	g_UITextComboMag->Init();
	uiTextPos	= { CENTER_X, COMBOMAG_POS_Y };
	uiTextSize	= { COMBOMAG_TEXT_SIZE ,COMBOMAG_TEXT_SIZE };
	uiTextColor = { 1.0f,1.0f,1.0f,0.2f };
	g_UITextComboMag->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"*0.0", FONT_NORMAL);

	return S_OK;
}

void UninitCombo()
{
	g_UITextCombo	= NULL;
	g_UITextComboNum= NULL;
	g_UITextComboMag= NULL;

	delete g_UITextCombo	;
	delete g_UITextComboNum	;
	delete g_UITextComboMag	;
}

void UpdateCombo()
{
	GetComboDizit();
}

void DrawCombo()
{
	TranslationTextCombo();
	g_UITextComboNum->Draw();	//コンボ数描画

	TranslationTextComboMag();	
	g_UITextComboMag->Draw();	//スコア倍率描画

	g_UITextCombo->Draw();		//"COMBO"描画
}

void DrawMaxCombo()
{
	D3DXVECTOR2 uiTextPos	= { COMBO_TEXT_R_POS_X,	COMBO_TEXT_R_POS_Y };
	D3DXVECTOR2 uiTextSize	= { COMBO_TEXT_R_SIZE,COMBO_TEXT_R_SIZE };
	D3DXCOLOR	uiTextColor = { 1.0f,1.0f,1.0f,1.0f };
	g_UITextCombo->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"MAX COMBO", FONT_NORMAL);
	g_UITextCombo->Draw();

	uiTextPos	= { COMBONUM_TEXT_R_POSX,	COMBONUM_TEXT_R_POSY };
	uiTextSize	= { COMBONUM_TEXT_R_SIZE,	COMBONUM_TEXT_R_SIZE };
	uiTextColor = { 1.0f,1.0f,1.0f,1.0f };
	g_UITextComboNum->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"000", FONT_NORMAL);
	TranslationTextMaxCombo();
	g_UITextComboNum->Draw();
}

void ComboPlus(int combo)
{
	ComboAdd += combo;
	if (ComboAdd % 10 == 0) {
		ComboMagNum = ComboAdd * 2 / 10;
		ComboMagUp();
	}
	if (ComboMax < ComboAdd) {
		ComboMax = ComboAdd;
	}
}

void GetComboDizit()
{
	for (int i = 0; i < COMBO_DIGIT; i++) {
		int j, ComboDigit;
		j = i;
		ComboDigit = 10;
		while (j > 0)
		{
			ComboDigit *= 10;
			j--;
		}
		g_Combo[i].combonum = ComboAdd % ComboDigit / (ComboDigit / 10);
	}
}

void ComboMagUp()
{
	if (ComboMagNum <= 30) {
		g_ComboMag[0].combonum = ComboMagNum % 10;
		if (ComboMagNum < 10)
		{
			g_ComboMag[2].combonum = 1;
		}
		else if (ComboMagNum % 10 == 0)
		{
			g_ComboMag[2].combonum++;
			g_ComboMag[0].combonum = 0;
		}
	}
	else {
		ComboMagNum = 30;
	}
}
void ResetCombo()
{
	ComboAdd = 0;
	ComboMagNum = 0;
	ComboMagUp();
}

float GetComboScoreUp()
{
	float Up = 1.0f;
	if (ComboAdd >= 10)
	{
		Up += ComboMagNum / 10.0f;
	}
	return Up;
}

void TranslationTextCombo()
{
	char comboNumChar[256] = "000";

	for (int i = 0; i < COMBO_DIGIT; i++)
	{
		comboNumChar[COMBO_DIGIT - i - 1] = g_Combo[i].combonum + ASCII_NUM_0;
	}
	g_UITextComboNum->SetText(comboNumChar);
}

void TranslationTextComboMag()
{
	char comboMagChar[256] = "*0.0";

	comboMagChar[1] = g_ComboMag[2].combonum + ASCII_NUM_0;
	comboMagChar[3] = g_ComboMag[0].combonum + ASCII_NUM_0;
	
	g_UITextComboMag->SetText(comboMagChar);
	g_UITextComboMag->SetColor(MagColor[g_ComboMag[2].combonum]);
}

void TranslationTextMaxCombo()
{
	COMBO maxCombo[COMBO_DIGIT];

	for (int i = 0; i < COMBO_DIGIT; i++)
	{
		int j, ComboDigit;
		j = i;
		ComboDigit = 10;
		while (j > 0)
		{
			ComboDigit *= 10;
			j--;
		}
		maxCombo[i].combonum = ComboMax % ComboDigit / (ComboDigit / 10);
	}

	char maxComboChar[256] = "000";
	for (int i = 0; i < COMBO_DIGIT; i++)
	{
		maxComboChar[COMBO_DIGIT - i - 1] = maxCombo[i].combonum + ASCII_NUM_0;
	}
	g_UITextComboNum->SetText(maxComboChar);
}
	
