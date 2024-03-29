/*==============================================================================

   タイトル画面処理 [Over.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "Over.h"
#include "texture.h"
#include "sprite.h"
#include "inputx.h"
#include "sound.h"
#include "fade.h" 
#include "keyboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitOver(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitOver(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateOver(void)
{
	//エンターキーが押されたらSCENE_GAMEへ移行する
	if (Keyboard_IsKeyDown(KK_ENTER))
	{
		SceneTransition(SCENE_TITLE);
	}
	//コントローラーBボタン押したらSCENE_GAMEへ移行
	if (IsButtonTriggered(0, XINPUT_GAMEPAD_B))
	{
		SceneTransition(SCENE_TITLE);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawOver(void)
{
	
}
