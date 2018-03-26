#include "CDebug.h"

void CDebug::CreateFont(LPDIRECT3DDEVICE9 lpdev) {
	D3DXCreateFont(lpdev,						// デバイスオブジェクト
		0,							// 文字の高さ
		10,							// 文字の幅
		FW_REGULAR,					// 文字の太さ
		NULL,						// ミップマップレベルの数
		FALSE,						// イタリック体か否か true:イタリック体
		SHIFTJIS_CHARSET,			// 文字セット
		OUT_DEFAULT_PRECIS,			// 常にＴＲＵＥＴＹＰＥのフォントを使う
		PROOF_QUALITY,				// 出力品質
		FIXED_PITCH | FF_MODERN,	// ピッチとファミリーインデックス
		"ＭＳＰゴシック",					// フォント名
		&m_pFont);					// 生成されたフォント
}

void CDebug::DrawText(int x, int y, char* str) {
	RECT	rect = { x,y,0,0 };
	// 文字列のサイズを計算
	m_pFont->DrawText(NULL, str, -1, &rect, DT_CALCRECT, NULL);
	// 文字列表示（赤色で表示）
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT | DT_BOTTOM, 0xffff0000);
}