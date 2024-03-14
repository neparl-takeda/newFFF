//====================================================
//バレット
//作成：町田裕哉
//		2022/10/26
//====================================================
#include "bullet.h"
#include "enemy.h"
#include "texture.h"
#include "sprite.h"
#include "special.h"
#include "effect.h"
#include "collision.h"

//================================
//グローバル変数
//================================
BULLET g_Bullet[BULLET_MAX];
//================================
//初期化
//================================

HRESULT InitBullet()
{
	//テクスチャのロード
	int texno = LoadTexture((char*)"data\\TEXTURE\\bullet_Player.png");
	//構造体の初期化
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_Bullet[i].use		= false;
		g_Bullet[i].w		= BULLET_SIZE_W;
		g_Bullet[i].h		= BULLET_SIZE_H;
		g_Bullet[i].pos		= D3DXVECTOR2(0, 0);
		g_Bullet[i].texNo	= texno;
		g_Bullet[i].mov		= D3DXVECTOR2(0, -BULLET_SPEED);
		g_Bullet[i].type	= TYPE_BULLET_NORMAL;
	}
	return S_OK;
}

//終了処理
void UninitBullet()
{
}

//更新処理
void UpdateBullet()
{
	ENEMY* pEnemy = GetEnemy();

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == true)
		{
			g_Bullet[i].oldpos = g_Bullet[i].pos;	//当たり判定用の座標コピー

			g_Bullet[i].pos += g_Bullet[i].mov;	//移動

			if (g_Bullet[i].pos.y < 0 - g_Bullet[i].h / 2)
			{
				g_Bullet[i].use = false;
			}

			if (pEnemy->use)
			{
				if(CollisionBB(g_Bullet[i].pos,pEnemy->pos,D3DXVECTOR2(g_Bullet[i].w,g_Bullet[i].h),pEnemy->size))
				{
					pEnemy->hp -= Damage(g_Bullet[i].type);
					switch (g_Bullet->type)
					{
					default:
						SetEffect(EFFECT_2, D3DXVECTOR2(g_Bullet[i].pos.x, g_Bullet[i].pos.y - 30.0f), D3DXVECTOR2(200.0f, 200.0f));
						break;
					case TYPE_BULLET_NORMAL:
						SetEffect(EFFECT_2, D3DXVECTOR2(g_Bullet[i].pos.x, g_Bullet[i].pos.y - 30.0f), D3DXVECTOR2(200.0f, 200.0f));
						break;
					case TYPE_BULLET_STRONG:
						SetEffect(EFFECT_5, D3DXVECTOR2(g_Bullet[i].pos.x, g_Bullet[i].pos.y - 60.0f), D3DXVECTOR2(300.0f, 300.0f));
						break;
					}
					
					g_Bullet[i].use = false;
				}
			}
		}
	}
}

void DrawBullet()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use)
		{
			DrawSpriteColor(g_Bullet[i].texNo, g_Bullet[i].pos.x, g_Bullet[i].pos.y, g_Bullet[i].w, g_Bullet[i].h,
				0.25f*g_Bullet[i].type, 0.0f, 0.25f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

	}
}

BULLET * GetBullet()
{
	return &g_Bullet[0];
}

void SetBullet(D3DXVECTOR2 pos)
{
	SPECIAL* sp = GetSpecial();

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == false)
		{
			if (sp->damage_up == true)
			{
				g_Bullet[i].type = TYPE_BULLET_STRONG;
				g_Bullet[i].pos  = pos;
				g_Bullet[i].use  = true;
				g_Bullet[i].w = BULLET_SIZE_W * 1.5f;
				g_Bullet[i].h = BULLET_SIZE_H * 1.5f;
			}
			else
			{
				g_Bullet[i].type = TYPE_BULLET_NORMAL;
				g_Bullet[i].pos  = pos;
				g_Bullet[i].use  = true;
				g_Bullet[i].w = BULLET_SIZE_W;
				g_Bullet[i].h = BULLET_SIZE_H;
			}
			return;
		}
	}
}

//ダメージの計算　returnでダメージの数値を返す
int Damage(int type)
{
	SPECIAL* sp = GetSpecial();

	int damage = 0;

	//当該special発動中のダメージの増加
	if (type == TYPE_BULLET_STRONG)
	{
		float m = 1, n = (float)GetEnemyNum();
		for (int i = 0; i < n; i++) {
			m *= 0.9f;
		}
		damage = (int)(250 * m);
		return damage;
	}
	//非発動時のデフォルト数値
	if (type == TYPE_BULLET_NORMAL)
	{
		float m = 1, n = (float)GetEnemyNum();
		for (int i = 0; i < n; i++) {
			m *= 0.9f;
		}
		damage = (int)(90 * m);
		return damage;
	}
	return damage;
}