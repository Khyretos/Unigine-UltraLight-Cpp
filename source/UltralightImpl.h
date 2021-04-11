#pragma once
#include "Ultralight\Ultralight.h"

#include "UltralightImpl.h"
#include <AppCore/Platform.h>
#include <AppCore/Overlay.h>
#include <AppCore/JSHelpers.h>
#include <Ultralight/String.h>
#include <AppCore/CAPI.h>
#include <JavaScriptCore/JavaScript.h>


#include <UnigineLogic.h>
#include <UnigineStreams.h>

#include "UnigineEngine.h"
#include "UnigineGui.h"
#include "UnigineWidgets.h"
#include "UnigineEditor.h"
#include "UnigineTextures.h"
#include "UnigineApp.h"
#include "UnigineMaterials.h"


#include "Ultralight/Ultralight.h"

class UltralightImpl
{

public:
	static void Init();
	static void InitPlatform();
	static void newFrame();
	static void RenderDrawData();
	static void Shutdown();
	static bool CreateRenderer();
	static int CreateView();
	static void RenderOneFrame();
	static void UpdateLogic();
	static void CreateTexture(Unigine::WidgetSpritePtr sprite,void* pixels, uint32_t width, uint32_t height, uint32_t stride);

	static void createHUDWidgetSprite();
	static void shutdown();

private:
	//std::unique_ptr<LoggerDefault> looger;
};