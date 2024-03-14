//====================================================
//�G�l�~�[
//�쐬�F�����͎�
//		2022/10/26
//====================================================
#include "enemy.h"
#include "collision.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "enemybullet.h"
#include "input.h"
#include "rhythm.h"
//#include "frame.h"
#include "score.h"
#include "combo.h"


ENEMY		Enemy;
ENEMYHP		EnemyHp;
int			EnemyDie;
int			EnemyNum;
int			UnagiPosY;
float		EnemySpYa;
float		EnemySpYt;
float		EnemyAlpha;
bool		BadFlag;
//�e�N�X�`�����̕ۑ��ϐ�
static	ID3D11ShaderResourceView	*g_TextureEnemy;
static  int		g_TextureEnemyID;
static	ID3D11ShaderResourceView	*g_TextureEnemyHp;
static	char	*g_TextureNameEnemyHpA = ENEMY_HP_TEX_A;//�e�N�X�`����
static	char	*g_TextureNameEnemyHpB = ENEMY_HP_TEX_B;//�e�N�X�`����
static  int		g_TextureEnemyIcon;

//===================================================
//�@������
//===================================================
HRESULT InitEnemy(int StageNum)
{
	UnagiPosY = 0;

	//�e�N�X�`���̃��[�h
	switch (StageNum)
	{
	default:
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_zarigani_dummy.png");
			Enemy.size = D3DXVECTOR2(ENEMY00_SIZE_X, ENEMY00_SIZE_Y);
			EnemySpYa = 0.925f;
			EnemySpYt = 25.0f;
		break;

		case 0:
			// �� �`���[�g���A�� �U���K�j
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_zarigani_dummy.png");
			Enemy.size = D3DXVECTOR2(ENEMY00_SIZE_X, ENEMY00_SIZE_Y);
			EnemySpYa = 0.95275f;
			EnemySpYt = 25.0f;
			break;
		case 1:
			// �� �U���K�j
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_zarigani.png");
			Enemy.size = D3DXVECTOR2(ENEMY01_SIZE_X, ENEMY01_SIZE_Y);
			EnemySpYa = 0.95275f;
			EnemySpYt = 25.0f;
			break;
		case 2:
			// �� �E�i�M
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_unagi.png");
			Enemy.size = D3DXVECTOR2(ENEMY02_SIZE_X, ENEMY02_SIZE_Y);
			UnagiPosY = 200;
			EnemySpYa = 0.9625f;
			EnemySpYt = 12.5f;
			break;
		case 3:
			// �� �^�R
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_octopus.png");
			Enemy.size = D3DXVECTOR2(ENEMY03_SIZE_X, ENEMY03_SIZE_Y-300.0f);
			UnagiPosY = -80;
			EnemySpYa = 0.9575f;
			EnemySpYt = 25.0f;
			break;
		case 4:
			// �� �T��
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_shark.png");
			Enemy.size = D3DXVECTOR2(ENEMY04_SIZE_X, ENEMY04_SIZE_Y);
			UnagiPosY = 80;
			EnemySpYa = 0.976f;
			EnemySpYt = 12.0f;
			break;
		case 5:
			// �� �������
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_shishamo muscle.png");
			Enemy.size = D3DXVECTOR2(ENEMY05_SIZE_X, ENEMY05_SIZE_Y);
			EnemySpYa = 0.95275f;
			EnemySpYt = 25.0f;
			break;
		case 6:
			// �� �J�[�X�E�I�u�E�U���K�j
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_zarigani_shirokuro.png");
			Enemy.size = D3DXVECTOR2(ENEMY06_SIZE_X, ENEMY06_SIZE_Y);
			EnemySpYa = 0.95275f;
			EnemySpYt = 25.0f;
			break;
		case 7:
			// �� ���ȏd
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_unagidon_yankee.png");
			Enemy.size = D3DXVECTOR2(ENEMY07_SIZE_X, ENEMY07_SIZE_Y);
			UnagiPosY = -100;
			EnemySpYa = 0.96f;
			EnemySpYt = 25.0f;
			break;
		case 8:
			// �� ���h����
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_shiokara_musha.png");
			Enemy.size = D3DXVECTOR2(ENEMY08_SIZE_X, ENEMY08_SIZE_Y);
			UnagiPosY = -100;
			EnemySpYa = 0.96f;
			EnemySpYt = 25.0f;
			break;
		case 9:
			// �� �W���[�W
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_shark_yankee.png");
			Enemy.size = D3DXVECTOR2(ENEMY09_SIZE_X, ENEMY09_SIZE_Y);
			UnagiPosY = 100;
			EnemySpYa = 0.9765f;
			EnemySpYt = 12.0f;
			break;
		case 10:
			// �� �t���A�[�}�[�E�U���K�j
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_mm meka(combinated).png");
			Enemy.size = D3DXVECTOR2(ENEMY10_SIZE_X, ENEMY10_SIZE_Y);
			EnemySpYa = 0.95275f;
			EnemySpYt = 25.0f;
			break;
		case 11:
			// �� �v���g�^�C�v�E�U���K�j
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_prototype.png");
			Enemy.size = D3DXVECTOR2(ENEMY11_SIZE_X, ENEMY11_SIZE_Y);
			EnemySpYa = 0.95275f;
			EnemySpYt = 25.0f;
			break;
		case 12:
			// �� �E�i�M�h��
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_unagydra.png");
			Enemy.size = D3DXVECTOR2(ENEMY12_SIZE_X, ENEMY12_SIZE_Y);
			UnagiPosY = 200;
			EnemySpYa = 0.9625f;
			EnemySpYt = 12.5f;
			break;
		case 13:
			// �� ���Q����
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_alliance.png");
			Enemy.size = D3DXVECTOR2(ENEMY13_SIZE_X, ENEMY13_SIZE_Y);
			EnemySpYa = 0.95275f;
			EnemySpYt = 25.0f;
			break;
		case 14:
			// �� �J�[�X�E�I�u�E�W���[�W
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_jawge_curse.png");
			Enemy.size = D3DXVECTOR2(ENEMY14_SIZE_X, ENEMY14_SIZE_Y);
			UnagiPosY = 80;
			EnemySpYa = 0.9765f;
			EnemySpYt = 12.0f;
			break;
		case 15:
			// �� �K�l�[�V����
			g_TextureEnemyID = LoadTexture((char*)"data/TEXTURE/ENEMY/chara_ganeshamo.png");
			Enemy.size = D3DXVECTOR2(ENEMY15_SIZE_X, ENEMY15_SIZE_Y);
			EnemySpYa = 0.95275f;
			EnemySpYt = 25.0f;
			break;
	}
	g_TextureEnemyIcon = LoadTexture((char*)"data/TEXTURE/HP_Icon.png");
	
	//Enemy.pos = D3DXVECTOR2(ENEMY_SPAWN_POS_X, ENEMY_SPAWN_POS_Y - UnagiPosY*2.5f);
	Enemy.pos = D3DXVECTOR2(ENEMY_SPAWN_POS_X, ENEMY_SPAWN_POS_Y - 475.0f);
	Enemy.sp = D3DXVECTOR2(ENEMY_SP, EnemySpYt);
	Enemy.rot = 0.0f;
	Enemy.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Enemy.hp = ENEMY_HP;
	Enemy.use = false;

	EnemyDie   = -2;
	EnemyAlpha = 1.0f;

	if (Enemy.texno == -1)
	{//���[�h�G���[
		exit(999);
	}

	EnemyHp.size = D3DXVECTOR2(ENEMY_HP_SIZE_X, ENEMY_HP_SIZE_Y);
	EnemyHp.pos = D3DXVECTOR2(ENEMY_HP_POS_X , ENEMY_HP_POS_Y);
	EnemyHp.rot = 0.0f;
	EnemyHp.col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	//�e�N�X�`���̃��[�h
	EnemyHp.texnoA = (float)LoadTexture(g_TextureNameEnemyHpA);
	if (EnemyHp.texnoA == -1)
	{//���[�h�G���[
		exit(999);
	}
	EnemyHp.texnoB = (float)LoadTexture(g_TextureNameEnemyHpB);
	if (EnemyHp.texnoB == -1)
	{//���[�h�G���[
		exit(999);
	}


	BadFlag = false;
	EnemyNum = 1;
	return S_OK;
}

//===================================================
//�@�I������
//===================================================
void UninitEnemy()
{

}

//===================================================
//�@�X�V����
//===================================================
void UpdateEnemy()
{
	if (Enemy.use) 
	{
		Enemy.pos.x += Enemy.sp.x;		//�G�l�~�[���ړ�

		//�G�l�~�[�����[���[�ɐG�ꂽ�甽�]
		if (Enemy.pos.x - Enemy.size.x/2 <= SCREEN_WIDTH/2 - LANE_SIZE_X*2 - LANE_SIZE_X/2) 
		{
			Enemy.sp.x *= -1;
		}
		else if (Enemy.pos.x + Enemy.size.x/2 >= SCREEN_WIDTH / 2 + LANE_SIZE_X * 2 + LANE_SIZE_X / 2)
		{
			Enemy.sp.x *= -1;
		}

		//�̗�0�ŃG�l�~�[��
		if (Enemy.hp <= 0) 
		{
			Enemy.use = false;
			//Enemy.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, ENEMY_SPAWN_POS_Y - 475.0f);
 			ScorePlus((int)((500 * EnemyNum)*GetComboScoreUp()));
			EnemyNum++;
			Enemy.sp.y = EnemySpYt;
			EnemyDie = 50;
		}
	}

	//new�G�l�~�[�a��
	if(!Enemy.use)
	{
		if (EnemyDie>-2)
		{
			EnemyDie--;
			EnemyAlpha *= 0.9f;
		}
		if (EnemyDie==0)
		{
			Enemy.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, ENEMY_SPAWN_POS_Y - 475.0f);
		}
		if (EnemyDie == -2)
		{
			EnemyAlpha = 1.0f;
			Enemy.pos.y += Enemy.sp.y;
			Enemy.sp.y *= EnemySpYa;
			if (Enemy.pos.y >= ENEMY_SPAWN_POS_Y - UnagiPosY)
			{
				Enemy.pos.y = ENEMY_SPAWN_POS_Y - UnagiPosY;
				Enemy.hp = ENEMY_HP;
				Enemy.use = true;
			}
		}
	}
}

//===================================================
//�@�`�揈��
//===================================================
void DrawEnemy()
{		
	if (EnemyDie % 5 == 0)
	{

	}
	else
	{
		DrawSpriteColor(g_TextureEnemyID, Enemy.pos.x, Enemy.pos.y, Enemy.size.x, Enemy.size.y,
			0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, EnemyAlpha));
	}
		//�c�\��
		DrawSpriteColor((int)EnemyHp.texnoA, EnemyHp.pos.x, EnemyHp.pos.y + ((ENEMY_HP - Enemy.hp) / 1.11f), EnemyHp.size.x, EnemyHp.size.y - (ENEMY_HP - Enemy.hp) * 1.8f,
			0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	
		DrawSpriteColor((int)EnemyHp.texnoB, EnemyHp.pos.x, EnemyHp.pos.y, EnemyHp.size.x + 10.0f, EnemyHp.size.y + 10.0f,
			0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//HP�A�C�R��
		DrawSpriteColor(g_TextureEnemyIcon, EnemyHp.pos.x, PLAYER_HP_POS_Y - 490.0f, PLAYER_HP_SIZE_X + 15.0f, PLAYER_HP_SIZE_X + 15.0f,
			0.0f, 0.0f, 0.5f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

ENEMY * GetEnemy()
{
	return &Enemy;
}

int	GetEnemyNum()
{
	return EnemyNum;
}
int GetEnemyTex() 
{
	return g_TextureEnemyID;
}
bool BadEnd()
{
	if (!BadFlag) {
		Enemy.sp.x = 0;
		Enemy.pos.y -= Enemy.sp.y * 4.2f;
		if (Enemy.pos.y + Enemy.size.y < 0) {
			BadFlag = true;
		}
	}
	else {
		Enemy.size.x = Enemy.size.x * 1.05f;
		Enemy.size.y = Enemy.size.y * 1.05f;

		Enemy.pos.x = ENEMY_SPAWN_POS_X;
		Enemy.pos.y -= Enemy.sp.y * 2.0f;

		if (Enemy.pos.y < 1080 - 200) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;	//
}