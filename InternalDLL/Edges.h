#pragma once

void* Aimbot(SDK* sdk, uintptr_t* currentFocusPlayer, bool* hooked);
void Triggerbot(SDK* sdk);
void RocketJump(SDK* sdk, int jump);
void GlowObject(SDK* sdk);
void BunnyHop(SDK* sdk);
void* ESP(SDK* sdk);
void PatchDirectX(void* hook);