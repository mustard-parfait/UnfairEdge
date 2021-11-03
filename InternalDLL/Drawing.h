#pragma once

#define ABS(x) ((x < 0) ? (-x) : (x))

void DrawLine(int x1, int x2, int y1, int y2, int thickness, D3DCOLOR color);
void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color);
void DrawESPBox(Vec2 top, Vec2 bottom, int thickness, D3DCOLOR color);