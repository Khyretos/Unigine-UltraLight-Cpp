#include "UltralightImpl.h"
#include <AppCore/Platform.h>
#include <AppCore/Overlay.h>
#include <AppCore/JSHelpers.h>

#include <UnigineMeshDynamic.h>
#include <UnigineRender.h>
#include <UnigineWidgets.h>
#include "UnigineApp.h"
#include <Ultralight/String.h>

using namespace Unigine;
using namespace ultralight;
using namespace KeyCodes;

static TexturePtr font_texture;
static MeshDynamicPtr Ultralight_mesh;
static MaterialPtr Ultralight_material;

RefPtr<ultralight::Renderer> renderer;
RefPtr<View> view;

RefPtr<Bitmap> bitmap;

static TexturePtr texture;
static MeshDynamicPtr ultralight_mesh;
static MaterialPtr ultralight_material;

WidgetLabelPtr widget_label;
TexturePtr my_texture;
TexturePtr my_texture_ds;

WidgetSpritePtr hud;
GuiPtr gui;

const int KeyToInt(unsigned int key)
{
	int ConvertedKey = 0;

	switch (key)
	{
	case App::KEY_ESC:
		ConvertedKey = 9;
		break;
	case App::KEY_TAB:
		ConvertedKey = GK_TAB;
		break;
	case App::KEY_BACKSPACE:
		ConvertedKey = GK_BACK;
		break;
	case App::KEY_RETURN:
		ConvertedKey = GK_RETURN;
		break;
	case App::KEY_DELETE:
		ConvertedKey = GK_DELETE;
		break;
	case App::KEY_INSERT:
		ConvertedKey = GK_INSERT;
		break;
	case App::KEY_HOME:
		ConvertedKey = GK_HOME;
		break;
	case App::KEY_END:
		ConvertedKey = GK_END;
		break;
	case App::KEY_PGUP:
		ConvertedKey = GK_PRIOR;
		break;
	case App::KEY_PGDOWN:
		ConvertedKey = GK_NEXT;
		break;
	case App::KEY_LEFT:
		ConvertedKey = GK_LEFT;
		break;
	case App::KEY_RIGHT:
		ConvertedKey = GK_RIGHT;
		break;
	case App::KEY_UP:
		ConvertedKey = GK_UP;
		break;
	case App::KEY_DOWN:
		ConvertedKey = GK_DOWN;
		break;
	case App::KEY_SHIFT:
		ConvertedKey = GK_SHIFT;
		break;
	case App::KEY_CTRL:
		ConvertedKey = GK_CONTROL;
		break;
	case App::KEY_CMD:
		ConvertedKey = GK_LWIN;
		break;
	case App::KEY_SCROLL:
		ConvertedKey = GK_SCROLL;
		break;
	case App::KEY_CAPS:
		ConvertedKey = GK_CAPITAL;
		break;
	case App::KEY_NUM:
		ConvertedKey = GK_NUMLOCK;
		break;
	case App::KEY_F1:
		ConvertedKey = GK_F1;
		break;
	case App::KEY_F2:
		ConvertedKey = GK_F2;
		break;
	case App::KEY_F3:
		ConvertedKey = GK_F3;
		break;
	case App::KEY_F4:
		ConvertedKey = GK_F4;
		break;
	case App::KEY_F5:
		ConvertedKey = GK_F5;
		break;
	case App::KEY_F6:
		ConvertedKey = GK_F6;
		break;
	case App::KEY_F7:
		ConvertedKey = GK_F7;
		break;
	case App::KEY_F8:
		ConvertedKey = GK_F8;
		break;
	case App::KEY_F9:
		ConvertedKey = GK_F9;
		break;
	case App::KEY_F10:
		ConvertedKey = GK_F10;
	case App::KEY_F11:
		ConvertedKey = GK_F11;
	case App::KEY_F12:
		ConvertedKey = GK_F12;
	case App::NUM_KEYS:
		ConvertedKey = GK_NUMPAD0;
		break;
	}

	return ConvertedKey;
}


void UltralightImpl::HandleMouse()
{
	MouseEvent evt;
	evt.type = MouseEvent::kType_MouseMoved;
	evt.x = App::getMouseX();
	evt.y = App::getMouseY();
	evt.button = MouseEvent::kButton_None;

	view->FireMouseEvent(evt);
}

static int on_button_pressed(int button)
{
	MouseEvent evt;
	evt.type = MouseEvent::kType_MouseDown;
	evt.x = App::getMouseX();
	evt.y = App::getMouseY();

	switch (button)
	{
	case App::BUTTON_LEFT:
		evt.button = MouseEvent::kButton_Left;
		break;
	case App::BUTTON_RIGHT:
		evt.button = MouseEvent::kButton_Left;
		break;
	case App::BUTTON_MIDDLE:
		evt.button = MouseEvent::kButton_Left;
		break;
	}

	view->FireMouseEvent(evt);

	return 0;
}

static int on_button_released(int button)
{
	MouseEvent evt;
	evt.type = MouseEvent::kType_MouseUp;
	evt.x = App::getMouseX();
	evt.y = App::getMouseY();

	switch (button)
	{
	case App::BUTTON_LEFT:
		evt.button = MouseEvent::kButton_Left;
		break;
	case App::BUTTON_RIGHT:
		evt.button = MouseEvent::kButton_Left;
		break;
	case App::BUTTON_MIDDLE:
		evt.button = MouseEvent::kButton_Left;
		break;
	}

	view->FireMouseEvent(evt);

	return 0;
}

static int on_key_pressed(unsigned int key)
{
	// Synthesize a key press event for the 'Right Arrow' key
	KeyEvent evt;
	evt.type = KeyEvent::kType_RawKeyDown;
	evt.virtual_key_code = KeyToInt(key);
	evt.native_key_code = KeyToInt(key);
	evt.modifiers = 0;

	// You'll need to generate a key identifier from the virtual key code
	// when synthesizing events. This function is provided in KeyEvent.h
	GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);

	view->FireKeyEvent(evt);

	return 0;
}

static int on_key_released(unsigned int key)
{
	// Synthesize a key press event for the 'Right Arrow' key
	KeyEvent evt;
	evt.type = KeyEvent::kType_KeyUp;
	evt.virtual_key_code = KeyToInt(key);//KeyCodes::GK_TAB;
	evt.native_key_code = KeyToInt(key);
	evt.modifiers = 0;

	// You'll need to generate a key identifier from the virtual key code
	// when synthesizing events. This function is provided in KeyEvent.h
	GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);

	view->FireKeyEvent(evt);

	return 0;
}

static int on_unicode_key_pressed(unsigned int key)
{
	if (view->HasInputFocus()) {
		///
		/// The View has an input element with visible keyboard focus (blinking caret).
		/// Dispatch the keyboard event to the view and consume it.
		///
		//view->Focus();
	}


	if (key < App::KEY_ESC || key >= App::NUM_KEYS) 
	{
		char ch = static_cast<char>(key);
		ultralight::String ConvertedKey(&ch, 1);

		// Synthesize an  event for text generated from pressing the 'A' key
		KeyEvent evt;
		evt.type = KeyEvent::kType_Char;
		evt.text = ConvertedKey;
		evt.unmodified_text = ConvertedKey; // If not available, set to same as evt.text

		//GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);

		view->FireKeyEvent(evt);


	}
	return 0;
}

void UltralightImpl::Init() {
	Config config;

	App::setKeyPressFunc(on_key_pressed);
	App::setKeyReleaseFunc(on_key_released);
	App::setButtonPressFunc(on_button_pressed);
	App::setButtonReleaseFunc(on_button_released);
	App::setKeyPressUnicodeFunc(on_unicode_key_pressed);

	InitPlatform();
	CreateRenderer();
	CreateView();

	///
	/// We need to tell config where our resources are so it can 
	/// load our bundled SSL certificates to make HTTPS requests.
	///
	config.resource_path = "./resources/";

	///
	/// The GPU renderer should be disabled to render Views to a 
	/// pixel-buffer (Surface).
	///
	config.use_gpu_renderer = false;

	///
	/// You can set a custom DPI scale here. Default is 1.0 (100%)
	///
	config.device_scale = 1.0;

	///
	/// Pass our configuration to the Platform singleton so that
	/// the library can use it.
	///
	Platform::instance().set_config(config);


	gui = Gui::get();

	createHUDWidgetSprite();

	create_Ultralight_mesh();
	create_Ultralight_material();

	SetWidgetSpriteTexture(hud);
}

const char* htmlString() {
	return R"(
<html>
  <head>
    <style type="text/css">
      * { -webkit-user-select: none; }
      body { 
        font-family: -apple-system, 'Segoe UI', Ubuntu, Arial, sans-serif; 
        text-align: center;
        background: linear-gradient(#FFF, #DDD);
        padding: 2em;
      }
      body.rainbow {
        background: linear-gradient(90deg, #ff2363, #fff175, #68ff9d, 
                                           #45dce0, #6c6eff, #9e23ff, #ff3091);
        background-size: 1000% 1000%;
        animation: ScrollGradient 10s ease infinite;
      }
      @keyframes ScrollGradient {
        0%   { background-position:0% 50%; }
        50%  { background-position:100% 50%; }
        100% { background-position:0% 50%; }
      }
      #message {
        padding-top: 2em;
        color: white;
        font-weight: bold;
        font-size: 24px;
        text-shadow: 1px 1px rgba(0, 0, 0, 0.4);
      }
    </style>
    <script type="text/javascript">
    function HandleButton(evt) {
      // Call our C++ callback 'GetMessage'
      var message = GetMessage();
      
      // Display the result in our 'message' div element and apply the
      // rainbow effect to our document's body.
      document.getElementById('message').innerHTML = message;
      document.body.classList.add('rainbow');
    }
    </script>
  </head>
  <body>
    <button onclick="HandleButton(event);">Get the Secret Message!</button>
    <div id="message"></div>
  </body>
</html>
    )";
}

void UltralightImpl::InitPlatform() {
	/// 
	/// Use the OS's native font loader
	///
	Platform::instance().set_font_loader(GetPlatformFontLoader());

	///
	/// Use the OS's native file loader, with a base directory of "."
	/// All file:/// URLs will load relative to this base directory.
	///
	Platform::instance().set_file_system(GetPlatformFileSystem("./resources/"));

	///
	/// Use the default logger (writes to a log file)
	///
	Platform::instance().set_logger(GetDefaultLogger("ultralight.log"));
}

void UltralightImpl::CreateRenderer() {
	///
	/// Create our Renderer (call this only once per application).
	/// 
	/// The Renderer singleton maintains the lifetime of the library
	/// and is required before creating any Views.
	///
	/// You should set up the Platform handlers before this.
	///
	renderer = ultralight::Renderer::Create();
}

void UltralightImpl::CreateView() {
	///
	/// Create an HTML view, 500 by 500 pixels large.
	///
	view = renderer->CreateView(900, 600, false, nullptr);

	///
	/// Load a raw string of HTML.
	///
	//view->LoadHTML(htmlString());

	view->LoadURL("file:///app.html");
	///
	/// Notify the View it has input focus (updates appearance).
	///
	view->Focus();

	///
/// Get the pixel-buffer Surface for a View.
///
	Surface* surface = view->surface();

	///
	/// Cast it to a BitmapSurface.
	///
	BitmapSurface* bitmap_surface = (BitmapSurface*)surface;

	///
	/// Get the underlying bitmap.
	///
	RefPtr<Bitmap> bitmap = bitmap_surface->bitmap();
}

void UltralightImpl::UpdateLogic() {
	///
	/// Give the library a chance to handle any pending tasks and timers.
	///
	///
	renderer->Update();
}

void CopyBitmapToTexture(RefPtr<Bitmap> bitmap) {

	bitmap->SwapRedBlueChannels();

	///
	/// Lock the Bitmap to retrieve the raw pixels.
	/// The format is BGRA, 8-bpp, premultiplied alpha.
	///
	void* pixels = bitmap->LockPixels();

	///
	/// Get the bitmap dimensions.
	///
	uint32_t width = bitmap->width();
	uint32_t height = bitmap->height();
	uint32_t stride = bitmap->row_bytes();

	///
	/// Psuedo-code to upload our pixels to a GPU texture.
	///
	UltralightImpl::CreateTexture(hud, pixels, width, height, stride);

	///
	/// Unlock the Bitmap when we are done.
	///
	bitmap->UnlockPixels();
	bitmap->SwapRedBlueChannels();
}

void UltralightImpl::RenderOneFrame() {
	///
	/// Render all active Views (this updates the Surface for each View).
	///
	renderer->Render();

	///
	/// Psuedo-code to loop through all active Views.
	///
	//for (auto view : view_list) {
		///
		/// Get the Surface as a BitmapSurface (the default implementation).
		///
		BitmapSurface* surface = (BitmapSurface*)(view->surface());

		HandleMouse();

		///
		/// Check if our Surface is dirty (pixels have changed).
		///
		if (!surface->dirty_bounds().IsEmpty()) {
			///
			/// Psuedo-code to upload Surface's bitmap to GPU texture.
			///
			CopyBitmapToTexture(surface->bitmap());

			///
			/// Clear the dirty bounds.
			///
			surface->ClearDirtyBounds();
		}
	//}
}

void UltralightImpl::CreateTexture(Unigine::WidgetSpritePtr sprite,void* pixels, uint32_t width, uint32_t height, uint32_t stride)
{
	texture = Texture::create();
	texture->create2D(width, height, Texture::FORMAT_RGBA8, Texture::DEFAULT_FLAGS);

	auto blob = Blob::create();
	auto kees = static_cast<unsigned char*>(pixels);
	blob->setData(kees,stride);
	texture->setBlob(blob);
	blob->setData(nullptr, 0);

	sprite->setRender(texture);
}

void UltralightImpl::create_Ultralight_mesh()
{
	ultralight_mesh = MeshDynamic::create(MeshDynamic::DYNAMIC_ALL);

	MeshDynamic::Attribute attributes[3]{};
	attributes[0].offset = 0;
	attributes[0].size = 2;
	attributes[0].type = MeshDynamic::TYPE_FLOAT;
	attributes[1].offset = 8;
	attributes[1].size = 2;
	attributes[1].type = MeshDynamic::TYPE_FLOAT;
	attributes[2].offset = 16;
	attributes[2].size = 4;
	attributes[2].type = MeshDynamic::TYPE_UCHAR;
	ultralight_mesh->setVertexFormat(attributes, 3);

	//assert(ultralight_mesh->getVertexSize() == sizeof(ImDrawVert) && "Vertex size of MeshDynamic is not equal to size of ImDrawVert");
}

void UltralightImpl::create_Ultralight_material()
{
	ultralight_material = Materials::findMaterial("ultralight")->inherit();
}

void UltralightImpl::createHUDWidgetSprite()
{
	GuiPtr gui = Gui::get();
	hud = WidgetSprite::create(gui);
	hud->setPosition(0, 0);
	hud->setWidth(900);
	hud->setHeight(600);
	hud->setLayerBlendFunc(0, Gui::BLEND_ONE, Gui::BLEND_ONE_MINUS_SRC_ALPHA);

	gui->addChild(hud, Gui::ALIGN_OVERLAP);
}

int UltralightImpl::SetWidgetSpriteTexture(Unigine::WidgetSpritePtr sprite)
{
	my_texture = Texture::create();

	const int width = int(view->width());
	const int height = int(view->height());
	int flags = Unigine::Texture::FILTER_LINEAR | Unigine::Texture::USAGE_RENDER;
	my_texture->create2D(width, height, Texture::FORMAT_RGBA8, Texture::DEFAULT_FLAGS);

	sprite->setRender(my_texture);

	return 1;
}