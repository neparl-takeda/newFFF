/*==============================================================================

   �^�C�g����ʏ��� [StageSelect.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "skill_select.h"
#include "stage_select.h"
#include "special.h"
#include "texture.h"
#include "sprite.h"
#include "inputx.h"
#include "sound.h"
#include "fade.h" 
#include "keyboard.h"
#include "main.h"

#include "Config.h"
#include "UIText.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define RING_SIZE  (340)
#define ICON_SIZE  (300)
#define ICON_ZOOM  (1.16f)
#define ICON_SPACE (320.0f)
#define ICON_POS_Y (CENTER_Y-120.0f)
#define PLATE_SIZE (260.0f)
#define PLATE_POS  (CENTER_X*1.46f)

#define SELECT_GRACE (50)

#define BPM_SIZE_MIN (95)
#define BPM_SIZE_MAX (105)
#define BPM_SIZE_ADD (0.7f)

#define PRESSING_COUNT (6)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


static int g_TextureBgSkillSelect;//�w�i�p�e�N�X�`���̎��ʎq
static int g_TextureRing;
static int g_TextureSkillPlate;
static int g_TextureTextArrow;
static int g_TextureSBg;

static int g_SE_SpSelect;
static int g_BGMNo;//BGM�̎��ʎq

int NowSSelect = SKILL_1;

float RingRot;	//�����O�p�x
float RingPosY;	//�����O�ʒu
float TextPosX; //�������ʒu
float TextAlpha;
float BPMSize;
float BPMAdd;
float BackRingrot;

int  changeN;	//�ړ��P�\
bool movingSp;	//�ړ��t���O

D3DXCOLOR BackRingCol;
SKILL_PANEL g_SkillPanel[SKILL_MAX];

UIText* g_UITextBpm{};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSkillSelect(void)
{
	//�e�N�X�`����ǂݍ���Ŏ��ʎq���󂯎��
	g_TextureBgSkillSelect		= LoadTexture((char*)"data/TEXTURE/fade_white.png");
	g_SkillPanel[SKILL_0].texno = LoadTexture((char*)"data/TEXTURE/icon_heal.png");
	g_SkillPanel[SKILL_1].texno = LoadTexture((char*)"data/TEXTURE/icon_damage.png");
	g_SkillPanel[SKILL_2].texno = LoadTexture((char*)"data/TEXTURE/icon_protect.png");
	g_TextureSkillPlate			= LoadTexture((char*)"data/TEXTURE/skill_plate.png");
	g_TextureTextArrow			= LoadTexture((char*)"data/TEXTURE/arrow.png");
	g_TextureRing				= LoadTexture((char*)"data/TEXTURE/icon_ring.png");

	g_TextureSBg				= LoadTexture((char*)"data/TEXTURE/UI_Config.png");	

	char	file_SE_SpSelect[] = "data\\SE\\SE_MenuMove.wav";
	g_SE_SpSelect = LoadSound(file_SE_SpSelect);

	//�\���̂̏�����
	for (int i = 0; i < SKILL_MAX; i++)
	{
		g_SkillPanel[i].pos		= D3DXVECTOR2(CENTER_X*2.25, (CENTER_Y - ICON_SPACE) + i * ICON_SPACE);
		g_SkillPanel[i].size	= D3DXVECTOR2(ICON_SIZE, ICON_SIZE);
		g_SkillPanel[i].col		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_SkillPanel[i].moving	= false;
	}
	BackRingCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	changeN		= 60;			//�ύX�P�\
	RingRot		= 0.0f;			//�����O�p�x
	RingPosY	= 0.0f;			//�����OY���W
	BPMSize		= BPM_SIZE_MIN;	//BPM�e�L�X�g�T�C�Y
	BPMAdd		= BPM_SIZE_ADD;	//BPM�e�L�X�g���Z
	BackRingrot = 0.0f;			//�w�i�����O�p�x
	TextPosX	= PLATE_POS;	//�����e�L�X�gX���W
	TextAlpha	= 0.0f;			//�����e�L�X�g�A���t�@�l
	movingSp	= false;		//�ړ��t���O

	//UiText Bpm
	g_UITextBpm = new UIText;
	g_UITextBpm->Init();
	D3DXVECTOR2 uiTextPos	= { CENTER_X, CENTER_Y, };
	D3DXVECTOR2 uiTextSize	= { 100.0f ,100.0f };
	int sStageNum = GetGemeStageNum();
	if (sStageNum >= 5)
	{
		D3DXCOLOR   uiTextColor = { 1.0f,0.1f,0.1f,1.0f };
		g_UITextBpm->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"BPMn150", FONT_WHITELINE);
	}
	else
	{
		D3DXCOLOR   uiTextColor = { 0.2f, 0.9f, 0.2f, 1.0f };
		g_UITextBpm->SetUITextPram(uiTextPos, uiTextSize, uiTextColor, (char*)"BPMn120", FONT_WHITELINE);
	}
	//BPMSize, BPMSize / 6.0f * 5.0f

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSkillSelect(void)
{
	g_TextureBgSkillSelect	= NULL;
	g_TextureRing			= NULL;
	g_TextureSkillPlate		= NULL;
	g_TextureTextArrow		= NULL;

	g_SE_SpSelect			= NULL;
	g_BGMNo					= NULL;//BGM�̎��ʎq
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSkillSelect(void)
{
	SPECIAL* pSp	 = GetSpecial();
	CONFIG*  pConfig = GetConfig();
	
	SetVolume(g_SE_SpSelect, pConfig->cSoundSEv);

	//����:�X�y�[�X�L�[
	if (Keyboard_IsKeyDown(KK_SPACE))
	{	pSp->type = NowSSelect;		}
	//����:�R���g���[���[B�{�^��
	if (IsButtonTriggered(0, XINPUT_GAMEPAD_B))
	{	pSp->type = NowSSelect;		}
	
	//�����ɑI���ړ�
	if ((IsButtonGroupPressing(KEY_GROUP::KEY_DOWN,PRESSING_COUNT)) && (movingSp == false) && (NowSSelect < 2) && (!pConfig->cScreenActive))
	{
		NowSSelect += 1;
		movingSp = true;
		TextAlpha = 0.0f;
		TextPosX = CENTER_X * 1.5;
		PlaySound(g_SE_SpSelect, 0);
	}
	//�㑤�ɑI���ړ�
	if (((IsButtonGroupPressing(KEY_GROUP::KEY_UP, PRESSING_COUNT))) && (movingSp == false) && (NowSSelect > 0) && (!pConfig->cScreenActive))
	{
		NowSSelect -= 1;
		movingSp = true;
		TextAlpha = 0.0f;
		TextPosX = CENTER_X * 1.5;
		PlaySound(g_SE_SpSelect, 0);
	}

	for (int i = 0; i < SKILL_MAX; i++)
	{
		//�X�L���A�C�R�����X���C�h
		if (g_SkillPanel[i].pos.x > CENTER_X*1.785f)
		{		g_SkillPanel[i].pos.x -= 50.0f;		}

	//�I������Ă���g�� & ����ĂȂ�������k��
		if (i == NowSSelect)
		{
			g_SkillPanel[i].size = D3DXVECTOR2(ICON_SIZE * ICON_ZOOM, ICON_SIZE * ICON_ZOOM);
			g_SkillPanel[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			g_SkillPanel[i].size = D3DXVECTOR2(ICON_SIZE,ICON_SIZE);
			g_SkillPanel[i].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}	

	//�I���ړ��P�\
		if (GetSceneChange())
		{
			movingSp = true;
		}
		if (movingSp==true)
		{
			changeN -= 1;
			if (changeN <= 0)
			{	changeN = SELECT_GRACE;
				movingSp = false;	}
		}

	//�������̃t�F�[�h
		if (TextAlpha < 1.0f)
		{	TextAlpha += 0.025f;	}
		if (TextPosX > PLATE_POS)
		{	TextPosX -= 15.0f;		}
	}

	//�Z���N�g���߂�}����
	if (NowSSelect >= 3)
	{	NowSSelect = 0;		}
	if (NowSSelect <= -1)
	{	NowSSelect = 2;		}

	//�����O�̏���
	RingRot += 0.5f;
	BackRingrot += 0.5f;
	RingPosY = (CENTER_Y - ICON_SPACE) + NowSSelect * ICON_SPACE;

	//�\��BPM�̏���
	BPMSize += BPMAdd;
	if ((BPMSize >= BPM_SIZE_MAX)|| (BPMSize <= BPM_SIZE_MIN))
	{	BPMAdd *= -1;	}
	D3DXVECTOR2 bpmSize = { BPMSize,BPMSize };
	g_UITextBpm->SetSize(bpmSize);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSkillSelect(void)
{
	//BPM �������O�O��
	GetDeviceContext()->PSSetShaderResources(0, 1,
		GetTexture(g_TextureRing));
	DrawSpriteColorRotation(
		CENTER_X,CENTER_Y,
		(int)(RING_SIZE * 1.3f), (int)(RING_SIZE * 1.3f),
		BackRingrot * 0.5f,BackRingCol,
		0.0f,1.0f,1.0f,1
	);
	//BPM �������O����
	GetDeviceContext()->PSSetShaderResources(0, 1,
		GetTexture(g_TextureRing));
	DrawSpriteColorRotation(
		CENTER_X,CENTER_Y,
		(int)(RING_SIZE * 1.5f),
		(int)(RING_SIZE * 1.5f),
		BackRingrot * 0.75f,BackRingCol,
		0.0f,1.0f,1.0f,1
	);
	//BPM�e�L�X�g
	int sStageNum = GetGemeStageNum();
	if (sStageNum>=5)
	{
		BPMSize += BPMAdd;
		BackRingCol = D3DXCOLOR(0.7f, 0.1f, 0.1f, 1.0f);
		BackRingrot += 3.5f;
		//DrawSpriteColor(g_TextureBPM, CENTER_X, CENTER_Y, BPMSize, BPMSize / 6.0f * 5.0f,
		//	0.5f, 0.0f, 0.5f, 1.0f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
	}
	else
	{
		BackRingCol = D3DXCOLOR(0.1f, 0.5f, 0.1f, 1.0f);
		//DrawSpriteColor(g_TextureBPM, CENTER_X, CENTER_Y, BPMSize, BPMSize / 6.0f * 5.0f,
		//	0.0f, 0.0f, 0.5f, 1.0f,D3DXCOLOR(0.2f, 0.9f, 0.2f, 1.0f));
	}
	//BPM �僊���O�O��
	GetDeviceContext()->PSSetShaderResources(0, 1,
		GetTexture(g_TextureRing));
	DrawSpriteColorRotation(
		CENTER_X, CENTER_Y,
		(int)(RING_SIZE * 3.0f), 
		(int)(RING_SIZE * 3.0f), 
		BackRingrot * -1.0f, BackRingCol,
		0.0f, 1.0f, 1.0f, 1
	);
	//BPM �僊���O����
	GetDeviceContext()->PSSetShaderResources(0, 1,
		GetTexture(g_TextureRing));
	DrawSpriteColorRotation(
		CENTER_X, CENTER_Y,
		(int)(RING_SIZE * 2.65f),
		(int)(RING_SIZE * 2.65f),
		BackRingrot * -1.5f, BackRingCol,
		0.0f, 1.0f, 1.0f, 1
	);

	DrawSpriteColor(g_TextureSBg, g_SkillPanel[0].pos.x-170.0f, CENTER_Y, 900.0f*0.85f, 1200.0f * 0.85f,
		0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�����e�L�X�g
	DrawSpriteColor(g_TextureSkillPlate,TextPosX, RingPosY, PLATE_SIZE * 2, PLATE_SIZE,
		0.0f, 1.0f / 3 * NowSSelect - 1, 1.0f, 1.0f / 3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, TextAlpha));

	//�����e�L�X�g�ɖ��o��
	if (NowSSelect < 2)
	{
		DrawSpriteColor(g_TextureTextArrow, TextPosX, RingPosY + (PLATE_SIZE / 2) + 30.0f, 80.0f, 80.0f,
			0.251f * 3.0f, 0.0f, 0.249f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, TextAlpha));
	}
	if (NowSSelect > 0)
	{
		DrawSpriteColor(g_TextureTextArrow, TextPosX, RingPosY - (PLATE_SIZE / 2) - 30.0f, 80.0f, 80.0f,
			0.251f * 2.0f, 0.0f, 0.247f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, TextAlpha));
	}



	//�X�L���A�C�R���̕\��
	for (int i = 0; i < SKILL_MAX; i++)
	{
			DrawSpriteColor(g_SkillPanel[i].texno, g_SkillPanel[i].pos.x, g_SkillPanel[i].pos.y, g_SkillPanel[i].size.x, g_SkillPanel[i].size.y,
				0.0f, 0.0f, 1.0f, 1.0f, g_SkillPanel[i].col);
	}


	//�X�L���A�C�R�������O����
	GetDeviceContext()->PSSetShaderResources(0, 1,
		GetTexture(g_TextureRing));
	DrawSpriteColorRotation(
		g_SkillPanel[0].pos.x, RingPosY,
		RING_SIZE, RING_SIZE,
		RingRot, D3DXCOLOR(0.0f, 0.8f, 0.6f, 1.0f),
		0.0f, 1.0f, 1.0f, 1
	);
	//�X�L���A�C�R�������O�O��
	GetDeviceContext()->PSSetShaderResources(0, 1,
		GetTexture(g_TextureRing));
	DrawSpriteColorRotation(
		g_SkillPanel[0].pos.x, RingPosY,
		(int)(RING_SIZE * 1.15f),
		(int)(RING_SIZE * 1.15f),
		RingRot * -1.5f, D3DXCOLOR(0.0f, 1.0f, 0.8f, 1.0f),
		0.0f, 1.0f, 1.0f, 1
	);

	g_UITextBpm->Draw();

}
