#include "pch.h"
#include "Include.h"

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
{
	ID3DXLine* LineL;
	D3DXCreateLine(lpDevice, &LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	LineL->SetWidth(thickness);
	LineL->Draw(Line, 2, color);
	LineL->Release();
}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color)
{
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawESPBox(Vec2 top, Vec2 bottom, int thickness, D3DCOLOR color)
{
	int height = ABS(top.y - bottom.y);
	
	Vec2 TopL, TopR;
	TopL.x = top.x - height / 4;
	TopR.x = top.x + height / 4;
	TopL.y = TopR.y = top.y;

	Vec2 BotL, BotR;
	BotL.x = bottom.x - height / 4;
	BotR.x = bottom.x + height / 4;
	BotL.y = BotR.y = bottom.y;

	DrawLine(TopL, TopR, thickness, color);
	DrawLine(TopL, BotL, thickness, color);
	DrawLine(TopR, BotR, thickness, color);
	DrawLine(BotL, BotR, thickness, color);
}