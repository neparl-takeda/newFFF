#pragma once
#ifndef COMBO_H_
#include "main.h"
struct COMBO 
{
	int			combonum;
};


HRESULT		InitCombo();
void		UninitCombo();
void		UpdateCombo();
void		DrawCombo();
void		DrawMaxCombo();
void		ComboPlus(int combo);
void		GetComboDizit();
void		ComboMagUp();
void		ResetCombo();
float		GetComboScoreUp();

void TranslationTextCombo();
void TranslationTextComboMag();
void TranslationTextMaxCombo();

#endif // !COMBO_H_
