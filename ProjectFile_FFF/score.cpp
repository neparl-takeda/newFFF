#include "score.h"
#include "texture.h"
#include "sprite.h"

#include "UIText.h"

#define		SCORETEXT_POS_X			(1600.0f)
#define		SCORETEXT_POS_Y			(25.0f)
#define		SCORETEXT_SIZE_Y		(45)

#define		SCORE_R_TEXT_POS_X		(1395.0f)
#define		SCORE_R_TEXT_POS_Y		(CENTER_Y-180.0f)
#define		SCORE_R_TEXT_SIZE		(40.0f)

#define		SCORENUM_R_TEXT_SIZE	(60.0f)
#define		SCORENUM_R_TEXT_POS_X	(1560.0f)
#define		SCORENUM_R_TEXT_POS_Y	(SCORE_R_TEXT_POS_Y + SCORENUM_R_TEXT_SIZE)
#define		SCOER_R_DIGIT			(8)

#define		SCOREENEMY_R_TEXT_POS_X		(1500.0f)
#define		SCOREENEMY_R_TEXT_POS_Y		(CENTER_Y + 165.0f)
#define		SCOREENEMY_R_TEXT_SIZE		(60.0f)

#define		SCOREENEMY_R_IMAGE_POS_X	(1345.0f)
#define		SCOREENEMY_R_IMAGE_POS_Y	(CENTER_Y + 150.0f)
#define		SCOREENEMY_R_IMAGE_SIZE_X	(100.0f)
#define		SCOREENEMY_R_IMAGE_SIZE_Y	(100.0f)


SCORE	g_Score[SCOER_DIGIT];
ENEMYSCORE g_EnemySocre[3];

float	ScoreTexNo;
float	ScoreMagTexNo;
int		ScoreAdd;

int		Enemys, texnum;

UIText* g_UITextScore		{};
UIText* g_UITextScoreNum	{};
UIText* g_UITextScoreEnemy	{};

void InitScore()
{
	ScoreAdd = 0;

	for (int i = 0; i < SCOER_DIGIT; i++) {
		//g_Score[i].Pos	= D3DXVECTOR2(SCORE_POS_X - (i-4) * (SCORE_SIZE_X), SCORE_POS_Y);
		//g_Score[i].Size = D3DXVECTOR2(SCORE_SIZE_X, SCORE_SIZE_Y);
		g_Score[i].Score = 0;
		g_Score[i].ToResult = 0;
	}

	//UiText "SCORE"
	g_UITextScore = new UIText;
	g_UITextScore->Init();
	D3DXVECTOR2 uiTextPos	= { SCORETEXT_POS_X, SCORETEXT_POS_Y };
	D3DXVECTOR2 uiTextSize	= { 40.0f ,40.0f };
	D3DXCOLOR   uiTextColor = { 1.0f,1.0f,1.0f,1.0f };
	g_UITextScore->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"SCORE", FONT_NORMAL);

	//UiText ÉXÉRÉAêî
	g_UITextScoreNum = new UIText;
	g_UITextScoreNum->Init();
	uiTextPos	= { SCORE_POS_X, SCORE_POS_Y };
	uiTextSize	= { 40.0f ,40.0f };
	uiTextColor = { 1.0f,1.0f,1.0f,1.0f };
	g_UITextScoreNum->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"00000000", FONT_NORMAL);

	//UiText åÇîjêî
	g_UITextScoreEnemy = new UIText;
	g_UITextScoreEnemy->Init();
	uiTextPos	= { SCOREENEMY_R_TEXT_POS_X,	SCOREENEMY_R_TEXT_POS_Y };
	uiTextSize	= { SCOREENEMY_R_TEXT_SIZE ,	SCOREENEMY_R_TEXT_SIZE };
	uiTextColor = { 1.0f,1.0f,1.0f,1.0f };
	g_UITextScoreEnemy->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"*00", FONT_NORMAL);
}

void InitScoreResult(int tex, int num)
{
	for (int i = 0; i < 2; i++) {
		int j, ScoreDigit;
		j = i;
		ScoreDigit = 10;
		while (j > 0)
		{
			ScoreDigit *= 10;
			j--;
		}
		g_EnemySocre[i].Score = (num - 1) % ScoreDigit / (ScoreDigit / 10);
	}

	texnum = tex;

	//UIText
	g_UITextScore	->SetSize		(D3DXVECTOR2(SCORE_R_TEXT_SIZE, SCORE_R_TEXT_SIZE));
	g_UITextScore	->SetPosition	(D3DXVECTOR2(SCORE_R_TEXT_POS_X, SCORE_R_TEXT_POS_Y));
	
	g_UITextScoreNum->SetSize		(D3DXVECTOR2(SCORENUM_R_TEXT_SIZE, SCORENUM_R_TEXT_SIZE));
	g_UITextScoreNum->SetPosition	(D3DXVECTOR2(SCORENUM_R_TEXT_POS_X, SCORENUM_R_TEXT_POS_Y));

}

void UpdateScore()
{
	GetDizit();
}

void UninitScore()
{

	g_UITextScore		= NULL;
	g_UITextScoreNum	= NULL;
	g_UITextScoreEnemy	= NULL;
	
	delete g_UITextScore;
	delete g_UITextScoreNum;
	delete g_UITextScoreEnemy;
}

void DrawScore()
{
	TranslationTextScore();
	g_UITextScoreNum->Draw();	//UIText ÉXÉRÉAêî ï`âÊ
	
	g_UITextScore->Draw();		//UIText "SCORE"  ï`âÊ
}

void DrawEnemyScore()
{
	//ìGÉCÉÅÅ[ÉWï`âÊ
	GetDeviceContext()->PSSetShaderResources(0, 1,
		GetTexture(texnum));
	DrawSpriteColorRotation(
		SCOREENEMY_R_IMAGE_POS_X, SCOREENEMY_R_IMAGE_POS_Y,
		SCOREENEMY_R_IMAGE_SIZE_X, SCOREENEMY_R_IMAGE_SIZE_Y,
		0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f,
		1.0f, 1.0f, 1
	);

	TranslationTextScoreEnemy();
	g_UITextScoreEnemy->Draw();		//UIText åÇîjêî ï`âÊ

}
void ScorePlus(int score)
{
	ScoreAdd += score;
	g_Score[0].ToResult += score;
}

void GetDizit()
{
	for (int i = 0; i < SCOER_DIGIT; i++) {
		int j, ScoreDigit;
		j = i;
		ScoreDigit = 10;
		while (j > 0)
		{
			ScoreDigit *= 10;
			j--;
		}
		g_Score[i].Score = ScoreAdd % ScoreDigit / (ScoreDigit / 10);
	}
}

void SetScore(D3DXVECTOR2 Pos, D3DXVECTOR2 Size)
{
	//for (int i = 0; i < SCOER_DIGIT; i++) {
	//	g_Score[i].Pos = D3DXVECTOR2(Pos.x - (i * Size.x), Pos.y);
	//	g_Score[i].Size = D3DXVECTOR2(Size.x, Size.y);
	//}
}

SCORE* GetScore()
{
	return &g_Score[0];
}

void TranslationTextScore()
{
	char scoreChar[256] = "00000000";
	
	for (int i = 0; i < SCOER_DIGIT; i++)
	{
		scoreChar[SCOER_DIGIT - i] = g_Score[i].Score + ASCII_NUM_0;
	}
	g_UITextScoreNum->SetText(scoreChar);
}

void TranslationTextScoreEnemy()
{
	char scoreChar[256] = "*00";

	for (int i = 0; i < 2; i++)
	{
		scoreChar[2 - i] = g_EnemySocre[i].Score + ASCII_NUM_0;
	}
	g_UITextScoreEnemy->SetText(scoreChar);
}