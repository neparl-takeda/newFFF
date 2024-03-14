#pragma once
#ifndef SCORE_H_
#include "main.h"

#define		SCORE_UV_MAX	(10)

#define		SCORE_UV_W		(0.2)
#define		SCORE_UV_H		(0.2)


typedef struct {
	int			Score;
	int			ToResult;

}SCORE;
typedef struct {
	int			Score;
	int			ToResult;

}ENEMYSCORE;

void InitScore();
void InitScoreResult(int tex, int num);
void UpdateScore();
void UninitScore();
void DrawScore();
void ScorePlus(int score);
void GetDizit();
SCORE* GetScore(void);
void DrawEnemyScore();
void SetScore(D3DXVECTOR2 Pos, D3DXVECTOR2 Size);

void TranslationTextScore();
void TranslationTextScoreEnemy();
#endif // !SCORE_H_

