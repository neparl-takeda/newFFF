/*==============================================================================

   UIテキスト[UIText.h]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#define UITEXT_MAX_LINE (5)

#include "main.h"
#include "renderer.h"

enum FONT_TYPE
{
	FONT_NORMAL = 0,
	FONT_GRADATION,
	FONT_WHITELINE,
	FONT_NUMMAX
};

class UIText
{
private:

	D3DXVECTOR2		m_Position;	//位置
	D3DXVECTOR2     m_Uv;		//UV値
	D3DXVECTOR2		m_Size;	//サイズ
	D3DXCOLOR		m_Color;
	float m_Bright  = 1.0f;

	int m_FontNow				 = {};
	int m_FontIndex[FONT_NUMMAX] = {};

	FONT_TYPE m_Type = FONT_NORMAL;

	int m_TextLong	 = 0;
	char m_Text[256] = {};
	

public:

	void	Init		();
	void	Uninit		();
	void	Update		();
	void	Draw		();

	void	SetPosition (D3DXVECTOR2 pos);
	void	SetSize		(D3DXVECTOR2 size);
	void	SetColor	(D3DXCOLOR	 col);
	void	SetText		(char text[256]);
	void	SetFont		(FONT_TYPE type);
	void	SetBright	(float bright);

	void	SetUITextPram(D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXCOLOR col, char text[256], FONT_TYPE type);

};



