#include "UltralightImpl.h"
#include <AppCore/Platform.h>

#include <UnigineMeshDynamic.h>
#include <UnigineRender.h>
#include <UnigineWidgets.h>

using namespace Unigine;
using namespace ultralight;

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

//WidgetSpritePtr hud;
//WidgetLabelPtr widget_label;

void UltralightImpl::Init() {
	Config config;

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
          body {
            margin: 0;
            padding: 0;
            overflow: hidden;
            color: black;
            font-family: Arial;
            background: linear-gradient(-45deg, #acb4ff, #f5d4e2);
            display: flex;
            justify-content: center;
            align-items: center;
          }
          div {
            width: 350px;
            height: 350px;
            text-align: center;
            border-radius: 25px;
            background: linear-gradient(-45deg, #e5eaf9, #f9eaf6);
            box-shadow: 0 7px 18px -6px #8f8ae1;
          }
          h1 {
            padding: 1em;
          }
          p {
            background: white;
            padding: 2em;
            margin: 40px;
            border-radius: 25px;
          }
        </style>
      </head>
      <body>
        <div>
          <h1>Hello World!</h1>
          <p>Welcome to Ultralight!</p>
        </div>
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
	Platform::instance().set_file_system(GetPlatformFileSystem("."));

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
	view = renderer->CreateView(500, 500, false, nullptr);

	///
	/// Load a raw string of HTML.
	///
	view->LoadHTML(htmlString());

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
	hud->setWidth(500);
	hud->setHeight(500);
	hud->setLayerBlendFunc(0, Gui::BLEND_ONE, Gui::BLEND_ONE_MINUS_SRC_ALPHA);

	gui->addChild(hud, Gui::ALIGN_OVERLAP);
}

int UltralightImpl::SetWidgetSpriteTexture(Unigine::WidgetSpritePtr sprite)
{
	my_texture = Texture::create();

	const int width = 500;//int(view->GetWidth());
	const int height = 500;//int(view->GetHeight());
	int flags = Unigine::Texture::FILTER_LINEAR | Unigine::Texture::USAGE_RENDER;
	my_texture->create2D(width, height, Texture::FORMAT_RGBA8, Texture::DEFAULT_FLAGS);

	sprite->setRender(my_texture);

	return 1;
}
