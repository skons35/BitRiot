/*
 * MickSDLRenderer.cpp
 *
 *  Created on: Apr 04, 2022
 *      Author: msaun
 */

#include "MickSDLRenderer.h"
#include "MickLogger.h"
#include "SDL.h"
#include "SDL_render.h"
#include "SDL_video.h"


using namespace std;

static MickSDLRenderer *sInstance = nullptr;


MickSDLRenderer::MickSDLRenderer(WindowMetadata windowMetadata) : MickBaseRenderer<SDL_Window, SDL_Renderer, SDL_Surface, SDL_Texture>()
{
  assert(sInstance == nullptr);
  init(windowMetadata);
  //MickLogger::getInstance()->debug(this, "Created and initialised instance of MickSDLRenderer.");
  sInstance = this;
}

void MickSDLRenderer::init(WindowMetadata windowMetadata)
{
  //VA : adding some logging mesg : 
  //std::string msg = "Entered MickSDLRenderer::init(WindowMetadata windowMetadata) ...";
  //MickLogger::getInstance()->debug(this, msg); // NOT DISPLAYED if DEBUG not set
  //std::cerr << msg.c_str() << std::endl; // OK, displayed
  
  // SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE); // let SDL log lots of messages
	
  // VA change (method  options) : 
  //if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  //if(SDL_InitSubSystem( 0 /* SDL_INIT_EVENTS | SDL_INIT_VIDEO  | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK */ ) < 0)    // OK too // end of VA change (note : same method than SDL_Init() )
  if(SDL_InitSubSystem( SDL_INIT_EVENTS | SDL_INIT_VIDEO  | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK ) < 0) 
  {
    std::string msg = "Failed to SDL_InitSubSystem video!";
    MickLogger::getInstance()->error(this, msg);
    throw new runtime_error(msg);
  }
    
  // VA : hanging before this line when missing some library at link : -noixemul -lSDSL2 --lGL -lc - lm
  //MickLogger::getInstance()->debug(this, "AFTER SDL_InitSubSystem");
  
  // VA : a couple error above: "error writing to datastream" displayed when SDL set to log VERSOSELY (see above) -> not sure of these errors effects
  //MickLogger::getInstance()->debug(this, "Last error was : ");
  //MickLogger::getInstance()->debug(this, SDL_GetError());
  
  m_windowHandle = SDL_CreateWindow(windowMetadata.windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                windowMetadata.windowWidth, windowMetadata.windowHeight, (windowMetadata.initFlags & ASK_FULLSCREEN) ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE );
  if (m_windowHandle == nullptr)
  {
    std::string msg = "Failed to create sdl_window!";
    MickLogger::getInstance()->error(this, msg);
    throw new runtime_error(msg);
  }

  // VA : try settings for MorphOS (below for avoiding full cpu use, set hardware accel and vsync wanted:)
  m_rendererHandle = SDL_CreateRenderer(m_windowHandle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 
  //m_rendererHandle = SDL_CreateRenderer(m_windowHandle, -1, 0); // << original code  
  if (m_rendererHandle == nullptr)
  {
    std::string msg = "Failed to create sdl_renderer!";
    MickLogger::getInstance()->error(this, msg);
    throw new runtime_error(msg);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  //SDL_RenderSetLogicalSize(m_rendererHandle, windowMetadata.windowWidth, windowMetadata.windowHeight);

  Uint32 pixel_format = SDL_GetWindowPixelFormat(m_windowHandle);
  
  // VA tempo debug :
  //const char* pixFormatName = SDL_GetPixelFormatName(pixel_format);
  //std::string pixFormString = pixFormatName;  // returns SDL_PIXELFORMAT_BGRA8888 for MorphOS case
  //MickLogger::getInstance()->debug(this, std::string("Window created with pixel format : ").append(pixFormString) );

  m_textureHandle = SDL_CreateTexture(m_rendererHandle, pixel_format, SDL_TEXTUREACCESS_STREAMING,
                                          windowMetadata.windowWidth, windowMetadata.windowHeight);
  if (m_textureHandle == nullptr)
  {
    std::string msg = "Failed to create primary SDL texture!";
    MickLogger::getInstance()->error(this, msg);
    throw new runtime_error(msg);
  }

  int depth = 0;
  Uint32 rmask = 0;
  Uint32 gmask = 0;
  Uint32 bmask = 0;
  Uint32 amask = 0;
  SDL_PixelFormatEnumToMasks(pixel_format, &depth, &rmask, &gmask, &bmask, &amask);

  m_surfaceBackBufferHandle = SDL_CreateRGBSurface(0, windowMetadata.windowWidth, windowMetadata.windowHeight, windowMetadata.windowBpp,
                                     rmask, gmask, bmask, amask);

  if (m_surfaceBackBufferHandle == nullptr)
  {
    std::string msg = "Failed to create primary SDL surface!";
    MickLogger::getInstance()->error(this, msg);
    throw new runtime_error(msg);
  }
  // update blitting RECT
  SDL_Rect clientArea;
  clientArea.x = 0;
  clientArea.y = 0;
  clientArea.w = windowMetadata.windowWidth -1 ;
  clientArea.h = windowMetadata.windowHeight - 1;

  SDL_SetClipRect(m_surfaceBackBufferHandle, &clientArea);

  // seed random number generator
  srand(SDL_GetTicks());

  initialised = true;
}

void MickSDLRenderer::pushCpuBufferToHardwareBuffer()
{
  SDL_UpdateTexture(getPrimaryTextureHandle(), nullptr, getSurfaceBackBufferHandle()->pixels, getSurfaceBackBufferHandle()->pitch);
  SDL_RenderClear(getRendererHandle());
  SDL_RenderCopy(getRendererHandle(), getPrimaryTextureHandle(), nullptr, nullptr);
}

void MickSDLRenderer::destructVideo()
{
  if(initialised && SDL_WasInit(SDL_INIT_VIDEO))
  {
    if(m_surfaceBackBufferHandle != nullptr)
    {
      SDL_FreeSurface(m_surfaceBackBufferHandle);
      m_surfaceBackBufferHandle = nullptr;
    }

    if(m_rendererHandle != nullptr)
    {
      SDL_DestroyRenderer(m_rendererHandle); // also frees m_textureHandle
      m_textureHandle = nullptr;
      m_rendererHandle = nullptr;
    }

    if(m_windowHandle != nullptr)
    {
      SDL_DestroyWindow(m_windowHandle);
      m_windowHandle = nullptr;
    }

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }
}


MickSDLRenderer::~MickSDLRenderer()
{
  destructVideo();
}

MickSDLRenderer* MickSDLRenderer::getInstance()
{
  assert(sInstance);
  return sInstance;
}
