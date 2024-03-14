#include "UIText.h"

#include <stdio.h>
#include <string.h>

#include "sprite.h"
#include "texture.h"

#define FONT_UV			(1.0f/16.0f)
#define DEFAULT_SIZE	(100.0f)
#define UV_CUT			(0.0005f)

void UIText::Init()
{
	m_Position  = D3DXVECTOR2(CENTER_X, CENTER_Y);
	m_Uv		= D3DXVECTOR2(0.0f, 0.0f);
	m_Size		= D3DXVECTOR2(DEFAULT_SIZE, DEFAULT_SIZE);
	m_Color		= D3DXCOLOR	 (1.0f, 1.0f, 1.0f, 1.0f);
	m_Bright	= 1.0f;
	
	m_FontIndex[FONT_NORMAL]	= LoadTexture((char*)"data/FONT/font01.png");
	m_FontIndex[FONT_GRADATION] = LoadTexture((char*)"data/FONT/font02.png");
	m_FontIndex[FONT_WHITELINE] = LoadTexture((char*)"data/FONT/font03.png");
	
	m_Type		= FONT_NORMAL;
	m_FontNow	= m_FontIndex[m_Type];

	SetText((char*)"START");
}

void UIText::Uninit()
{

}

void UIText::Update()
{

}

void UIText::Draw()
{
	float centerX = m_Position.x - m_Size.x / 2.0f;	
	int   ctX	  = m_TextLong * -1;		//1文字ずつ描画開始位置をずらすためのやつ
	int	  line	  = 1;
	int   ctY	  = 1;
	float centerY = m_Position.y;

	int sNumLine[5] = {};
	int count = 0;

	D3DXCOLOR color = m_Color * m_Bright;
	color.a = m_Color.a;

	for (int i = 0; i < m_TextLong; i++)
	{
		if ((int)m_Text[i] == 110)		//110 = "n" 改行コードとする
		{
			line += 1;					//行数を増やす
			count = 0;					//次行の文字数に追加する数字を初期化
			sNumLine[line] = 0;			
		}
		else
		{
			count += 1;
			sNumLine[line] = count;		//行ごとの文字数を記録
		}
	}

	int addS  = 0;
	int addSS = 0;
	
	ctX = sNumLine[1] * -1;
	for (int l = 0; l < line; l++)
	{
		for (int i = 0; i < sNumLine[l+1]; i++)
		{
			int t = (int)m_Text[i + addSS];
			int x = t % 16;
			int y = t / 16;

			ctX += 2;

			DrawSpriteColor(m_FontNow,
				centerX +	(ctX * (m_Size.x / 2.0f)),
				(centerY -	(line * (m_Size.y / 2.0f)) + (ctY * (m_Size.y / 2.0f))),
				m_Size.x,
				m_Size.y,
				FONT_UV * x + UV_CUT,
				FONT_UV * y + UV_CUT,
				FONT_UV - UV_CUT * 2.0f,
				FONT_UV - UV_CUT * 2.0f,
				color);

			addS++;
		}
		addSS = addS + l + 1;
		ctY += 2;
		ctX = sNumLine[l + 2] * -1;
	}
}

//-----------------------------------------------------
//	位置セット
//-----------------------------------------------------
void UIText::SetPosition(D3DXVECTOR2 pos)
{	m_Position = pos;	}

//-----------------------------------------------------
//	サイズセット
//-----------------------------------------------------
void UIText::SetSize(D3DXVECTOR2 size)
{	m_Size = size;	}

//-----------------------------------------------------
//	色セット
//-----------------------------------------------------
void UIText::SetColor(D3DXCOLOR	 col)
{	m_Color = col;	}

//-----------------------------------------------------
//	色セット
//-----------------------------------------------------
void UIText::SetBright(float bright)
{
	m_Bright = bright;
}

//-----------------------------------------------------
//	テキストセット
//-----------------------------------------------------
void UIText ::SetText(char text[256])
{
	int size = strlen(text);

	for (int i = 0; i < size; i++)
	{
		m_Text[i] = text[i];
	}
	m_TextLong = size;
}

//-----------------------------------------------------
//	フォントセット
//-----------------------------------------------------
void UIText::SetFont(FONT_TYPE type)
{
	if ((type >= 0) && (type < FONT_NUMMAX))
	{
		m_FontNow = m_FontIndex[type];
	}
}

//-----------------------------------------------------
//	全パラメーターセット
//-----------------------------------------------------
void UIText::SetUITextPram(D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXCOLOR col, char text[256], FONT_TYPE type)
{
	SetPosition	(pos);
	SetSize		(size);
	SetColor	(col);
	SetText		(text);
	SetFont		(type);
}