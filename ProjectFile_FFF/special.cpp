/*==============================================================================

   �X�y�V�����̐��� [special.cpp]
														 Author :�@�ēc �S��
														 Date   :�@11/19
--------------------------------------------------------------------------------
*/

#include "special.h""
#include "combo.h"
#include "Config.h"
#include "main.h"

//�X�y�V�����̃e�N�X�`��
static int g_SpecialIconTexture;
static int g_TextureGoRing;
static int g_TextureCharge;
static int g_TextureSpButton;
static int g_TextureSpKey;
static int g_TextureScreenEffect[3];

//�X�y�V������SE
static int g_SE_Special;		

//�^�C�}�[
unsigned int timer = 0;

//�^�C�}�[�̃X�^�[�g�t���O
bool start_timer = false;

//�X�y�V�����Q�[�W�̃J���[
float colorR;
float colorG;
float colorB;
float colorRGB;
float Calpha;
float ScreenAlpha;
float IconcolorR;
float IconcolorG;
float IconcolorB;
float SpButtonSize;
float SpButtonAdd;

int chargeUv;

float GoRingrot;
float AdRingrot;

SPECIAL sp;

void InitSpecial()
{
	//�e�N�X�`���̃��[�h
	g_SpecialIconTexture= LoadTexture((char*)"data/TEXTURE/fade_white.png");
	g_TextureGoRing		= LoadTexture((char*)"data/TEXTURE/icon_ring.png");
	g_TextureCharge		= LoadTexture((char*)"data/TEXTURE/skill_charge.png");
	g_TextureSpButton	= LoadTexture((char*)"data/TEXTURE/UI_Buttons_B.png");
	g_TextureSpKey		= LoadTexture((char*)"data/TEXTURE/UI_Buttons_A.png");

	g_TextureScreenEffect[0] = LoadTexture((char*)"data/TEXTURE/Screen_Effect_Heal.png");
	g_TextureScreenEffect[1] = LoadTexture((char*)"data/TEXTURE/Screen_Effect_Damage.png");
	g_TextureScreenEffect[2] = LoadTexture((char*)"data/TEXTURE/Screen_Effect_Shield.png");

	//SE�̃��[�h
	char	file_SE_Special[] = "data\\SE\\SE_Special.wav";
	g_SE_Special = LoadSound(file_SE_Special);

	//������
	sp.IconPos.x= 0;
	sp.IconPos.y= 0;
	sp.w		= 0;
	sp.h		= 0;
	
	sp.heal_count = 1;

	GoRingrot	= 0.0f;
	AdRingrot	= 1.0f;

	timer		= 0;
	colorR		= 0.8f;
	colorG		= 0.8f;
	colorB		= 0.8f;
	colorRGB	= 0.5f;
	Calpha		= 0.5f;
	IconcolorR	= 1.0f;
	IconcolorG	= 1.0f;
	IconcolorB	= 1.0f;
	ScreenAlpha = 0.0f;

	chargeUv	= 0;
	//sp.type = 0;  //0�񕜁A1�^�_�������A�Q��_���ጸ

	sp.charge	 = 0;		//�X�y�V�����̏����l�B��o���ɂ�0�ɂ��Ă��������B
	sp.UseOk 	 = false;
	sp.heal_up	= false;
	sp.damage_up = false;
	start_timer  = false;
	sp.get_damage_down = false;
	SpButtonAdd  = 2;
	SpButtonSize = 100.0f;

	switch (sp.type) {
	case SP_TYPE::DAMAGE_UP: //�^����_���[�W����
		g_SpecialIconTexture = LoadTexture((char*)"data/TEXTURE/icon_damage.png");
		break;

	case SP_TYPE::HEAL:		//�v���C���[�̗͉̑�
		g_SpecialIconTexture = LoadTexture((char*)"data/TEXTURE/icon_heal.png");
		break;

	case SP_TYPE::GET_DAMAGE_DOWN:	//�v���C���[�̔�_���[�W�y��
		g_SpecialIconTexture = LoadTexture((char*)"data/TEXTURE/icon_protect.png");
		break;
	}
}

void UninitSpecial()
{
}

void UpdateSpecial()
{
	PLAYER* player = GetPlayer();
	CONFIG* pConfig = GetConfig();

	SetVolume(g_SE_Special, pConfig->cSoundGSEv);

	//�X�y�V�����Q�[�W�����܂肫�����甭���\
	if (sp.charge >= SPECIAL_MAX)
	{
		colorB = 0.0f;
		sp.UseOk = true;
		Calpha = 1.0f;
		colorRGB = 1.0f;
	}
	//Y�{�^���ŃX�y�V��������
	if ((((IsButtonTriggered(0, XINPUT_GAMEPAD_RIGHT_SHOULDER)) || (Keyboard_IsKeyDown(KK_S))) && sp.UseOk == true)&&(!start_timer))
	{
		PlaySound(g_SE_Special, 0);

		switch (sp.type) {
		case SP_TYPE::DAMAGE_UP: //�^����_���[�W����
			sp.damage_up = true;
			start_timer = true;
			break;

		case SP_TYPE::HEAL:		//�v���C���[�̗͉̑�
			//player->hp += 75.0f;
			//if (player->hp >= PLAYER_HP_DEFAULT)
			//{
			//	player->hp = PLAYER_HP_DEFAULT;
			//}
			sp.heal_up = true;
			//sp.charge = 0;
			//sp.UseOk = false;
			start_timer = true;
			//timer = 280;
			//AdRingrot = 10.0f;
			break;

		case SP_TYPE::GET_DAMAGE_DOWN:	//�v���C���[�̔�_���[�W�y��
			sp.get_damage_down = true;
			start_timer = true;
			break;
		}
		ScreenAlpha = 0.01f;
	}

	//�X�y�V�����̌o�ߎ��Ԃ̃^�C�}�[
	if (start_timer == true)
	{
		if (timer == 1)
		{	AdRingrot = 8.0f;	}
		timer++;
		colorB = 1.0f;
		colorG = 0.4f;
		colorR = 1.0f;
		GoRingrot += AdRingrot;
		Calpha = 0.7f;
		ScreenAlpha *= 1.20f;
	}
	else
	{
		ScreenAlpha -= 0.1f;
	}

	if (ScreenAlpha >= 0.4f)
	{
		ScreenAlpha = 0.4f;
	}

	//5�b�ŏI��
	if (timer >= 300)
	{
		//�X�y�V������ǉ�����ꍇ�͂����ɂ��̃X�y�V�����̏I��������ǉ�
		sp.get_damage_down	= false;
		sp.damage_up		= false;
		sp.heal_up			= false;
		sp.heal_count = 1;
		//�^�C�}�[�ƃX�y�V�����̃��Z�b�g
		start_timer = false;
		timer = 0;
		sp.charge = 0;
		sp.UseOk = false;
		colorR = 0.8f;
		colorG = 0.8f;
		colorB = 0.8f;
		Calpha = 0.5f;
		AdRingrot = 1.0f;

	}

	if (sp.UseOk == true)
	{
		GoRingrot += AdRingrot;
		if (!start_timer)
		{		SpButtonSize += SpButtonAdd;	}
	}
	else
		{		SpButtonSize = 100.0f;			}

	if ((SpButtonSize <= 90.0f) || (SpButtonSize >= 110.0f))
		{		SpButtonAdd *= -1;				}

	if ((sp.charge >= 15) && (start_timer == false))
	{	chargeUv = 1;	}
	else
	{	if (!start_timer)
	{	chargeUv = 0;	}	
	}

	if ((start_timer))
	{	
		sp.charge = (300 - timer)/10;
	if (timer > 150)
	{	chargeUv = 0;	}
	}
}

void DrawSpecial()
{
	//�A�C�R�� �`���[�W
	DrawSpriteColor(g_TextureCharge,
		SKILL_ICON_POS_X, SKILL_ICON_POS_Y,
		SKILL_ICON_SIZE * 0.85, SKILL_ICON_SIZE * 0.85,
		1.0f / 15 * sp.charge, 0.5f * chargeUv,
		1.0f / 15, 0.5f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�A�C�R��
	DrawSpriteColor(g_SpecialIconTexture,
		SKILL_ICON_POS_X, SKILL_ICON_POS_Y,
		SKILL_ICON_SIZE, SKILL_ICON_SIZE,
		0.0f, 0.0f,
		1.0f, 1.0f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, Calpha));

	//�����O
		GetDeviceContext()->PSSetShaderResources(0, 1,
			GetTexture(g_TextureGoRing));
		DrawSpriteColorRotation(
			SKILL_ICON_POS_X, SKILL_ICON_POS_Y,
			(int)(SKILL_ICON_SIZE * 1.1f), (int)(SKILL_ICON_SIZE * 1.1f),
			GoRingrot, D3DXCOLOR(colorR, colorG, colorB, 1.0f),
			0.0f, 1.0f, 1.0f, 1
		);

		//�����O
		GetDeviceContext()->PSSetShaderResources(0, 1,
			GetTexture(g_TextureGoRing));
		DrawSpriteColorRotation(
			SKILL_ICON_POS_X, SKILL_ICON_POS_Y,
			(int)(SKILL_ICON_SIZE * 0.95f), (int)(SKILL_ICON_SIZE * 0.95f),
			GoRingrot * 0.8f, D3DXCOLOR(colorR * 0.8f, colorG * 0.8f, colorB * 0.8f, 1.0f),
			0.0f, 1.0f, 1.0f, 1
		);
		//�{�^��
		if (GetCtrl())
		{
			DrawSpriteColor(g_TextureSpKey, SKILL_ICON_POS_X + 100.0f, SKILL_ICON_POS_Y + 100.0f, SpButtonSize, SpButtonSize,
				0.167f * 4.0f, 0.25f * 1.0f, 0.167f, 0.249f, D3DXCOLOR(Calpha, Calpha, Calpha, 1.0f));
		}
		else
		{
			DrawSpriteColor(g_TextureSpButton, SKILL_ICON_POS_X + 100.0f, SKILL_ICON_POS_Y + 100.0f, SpButtonSize, SpButtonSize,
				0.167f * 2.0f, 0.25f * 2.0f, 0.167f, 0.249f, D3DXCOLOR(Calpha, Calpha, Calpha, 1.0f));
		}

		switch (sp.type) {
		case SP_TYPE::DAMAGE_UP: //�^����_���[�W����
			DrawSpriteColor(g_TextureScreenEffect[1], CENTER_X, CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, ScreenAlpha));
			break;

		case SP_TYPE::HEAL:		//�v���C���[�̗͉̑�
			DrawSpriteColor(g_TextureScreenEffect[0], CENTER_X, CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, ScreenAlpha));
			break;

		case SP_TYPE::GET_DAMAGE_DOWN:	//�v���C���[�̔�_���[�W�y��
			DrawSpriteColor(g_TextureScreenEffect[2], CENTER_X, CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT,
				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, ScreenAlpha));
			break;
		}
}

//�X�y�V�����̑���
void SpecialPlus()
{
	if ((sp.charge < SPECIAL_MAX)&&(start_timer==false))
	{	sp.charge += 1 + (int)(GetComboScoreUp() / 2);	}
}

//�Q�b�^�[
SPECIAL* GetSpecial(void)
{
	return &sp;
}