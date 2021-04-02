#pragma once
#include "Ultralight\Ultralight.h"

class UltralightImpl
{
public:
	static void Init();
	static void InitPlatform();
	static void newFrame();
	static void RenderDrawData();
	static void Shutdown();
	static void CreateRenderer();
	static void CreateView();
	static void RenderOneFrame();
	static void UpdateLogic();
	static void CreateTexture(void* pixels, uint32_t width, uint32_t height, uint32_t stride);
	static void create_Ultralight_mesh();
	static void create_Ultralight_material();
};