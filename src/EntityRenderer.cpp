// implementation of EntityRenderer class

#include "EntityRenderer.h"

// following used to change the colour of entities to their team colours
//static const unsigned int TRANSPARENT_COLOR = (255 << 8) + (255 << 0);

// VA test reversing color order on PPC (at constructor time): (part 1/2)
///*
static unsigned int TEAM_ORIGINAL_HIGHLIGHT_COLOR = 0;
static unsigned int TEAM_ORIGINAL_BASE_COLOR =     0;
static unsigned int TEAM_ORIGINAL_SHADOW_COLOR = 0; 
// original code : (no architecture specifics) :
/*
 static const unsigned int TEAM_ORIGINAL_HIGHLIGHT_COLOR = (238 << 16) + (166 << 8) + 247;
 static const unsigned int TEAM_ORIGINAL_BASE_COLOR = (200 << 16) + (23 << 8) + 221;
 static const unsigned int TEAM_ORIGINAL_SHADOW_COLOR = (142 << 16) + (16 << 8) + 158;
*/
// end of original code 

static bool teamColoursInitialised = false;

Uint32 EntityRenderer::TEAM_HIGHLIGHT_COLOR[] = {0,0,0,0,0};
Uint32 EntityRenderer::TEAM_SHADOW_COLOR[] = {0,0,0,0,0};
Uint32 EntityRenderer::TEAM_BASE_COLOR[] = {0,0,0,0,0};



EntityRenderer::EntityRenderer(SDL_Surface* sdl_primary, const char * filename)
{ 
  // VA tempo debug : // SDL_PIXELFORMAT_BGRA8888 on MorphOS PPC (but RGB888 on Linux x86)
  //fprintf(stdout,"EntityRenderer::EntityRenderer(..) > SDL_Surface format: %s \n", SDL_GetPixelFormatName(sdl_primary->format->format));
	
	
  if (! teamColoursInitialised)
  {
	  // VA test reversing color order on PPC (at constructor time): (part 2/2)
 	 if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	 {
	   TEAM_ORIGINAL_HIGHLIGHT_COLOR = (238 << 24) + (166 << 16) + (247 << 8);
	   TEAM_ORIGINAL_BASE_COLOR =      (200 << 24) + ( 23 << 16) + (221 << 8);
	   TEAM_ORIGINAL_SHADOW_COLOR =    (142 << 24) + ( 16 << 16) + (158 << 8); 
	   //fprintf(stdout,"EntityRenderer() : Team colors initialized for BIG ENDIAN architecture\n");
	 }
	else 
	 {
       TEAM_ORIGINAL_HIGHLIGHT_COLOR = (238 << 16) + (166 << 8) + 247;
	   TEAM_ORIGINAL_BASE_COLOR = (200 << 16) + (23 << 8) + 221;
   	   TEAM_ORIGINAL_SHADOW_COLOR = (142 << 16) + (16 << 8) + 158;
   	   //fprintf(stdout,"EntityRenderer() : Team colors initialized for LITTLE ENDIAN architecture\n");
 	 } // end of VA reversing bytes for BIG ENDIAN case (PPC) (part 2/2)
 
	// VA test reversing color order + using BGRA 32bpp not RGB 24 bpp:
	///*
    TEAM_HIGHLIGHT_COLOR[0] = SDL_MapRGBA(sdl_primary->format, 247, 166, 238, 0); // team 0 - computer team highlight
    TEAM_HIGHLIGHT_COLOR[1] = SDL_MapRGBA(sdl_primary->format, 166, 166, 247, 0); // team 1 - red team
    TEAM_HIGHLIGHT_COLOR[2] = SDL_MapRGBA(sdl_primary->format, 247, 185, 166, 0); // team 2 - blue team
    TEAM_HIGHLIGHT_COLOR[3] = SDL_MapRGBA(sdl_primary->format, 192, 247, 186, 0); // team 3 - green team
    TEAM_HIGHLIGHT_COLOR[4] = SDL_MapRGBA(sdl_primary->format, 166, 237, 241, 0); // team 4 - yellow team
	
    TEAM_BASE_COLOR[0] = SDL_MapRGBA(sdl_primary->format, 221, 23, 200, 0); // team 0 - computer team base colour
    TEAM_BASE_COLOR[1] = SDL_MapRGBA(sdl_primary->format, 23, 23, 221, 0); // team 1 - red team
    TEAM_BASE_COLOR[2] = SDL_MapRGBA(sdl_primary->format, 221, 69, 23, 0); // team 2 - blue team
    TEAM_BASE_COLOR[3] = SDL_MapRGBA(sdl_primary->format, 86, 221, 72, 0); // team 3 - green team
    TEAM_BASE_COLOR[4] = SDL_MapRGBA(sdl_primary->format, 23, 196, 208, 0); // team 4 - yellow team

    TEAM_SHADOW_COLOR[0] = SDL_MapRGBA(sdl_primary->format, 158, 16, 142, 0); // team 0 - computer team shadow colour
    TEAM_SHADOW_COLOR[1] = SDL_MapRGBA(sdl_primary->format, 16, 16, 158, 0); // team 1 - red team
    TEAM_SHADOW_COLOR[2] = SDL_MapRGBA(sdl_primary->format, 158, 49, 16, 0); // team 2 - blue team
    TEAM_SHADOW_COLOR[3] = SDL_MapRGBA(sdl_primary->format, 61, 158, 51, 0); // team 3 - green team
    TEAM_SHADOW_COLOR[4] = SDL_MapRGBA(sdl_primary->format, 16, 140, 148, 0); // team 4 - yellow team
    //*/ // end of VA test reversing color order
	// VA test reversing color order + using BGR 24 bpp but 32bpp stored):
	/*
    TEAM_HIGHLIGHT_COLOR[0] = SDL_MapRGB(sdl_primary->format, 247, 166, 238); // team 0 - computer team highlight
    TEAM_HIGHLIGHT_COLOR[1] = SDL_MapRGB(sdl_primary->format, 166, 166, 247); // team 1 - red team
    TEAM_HIGHLIGHT_COLOR[2] = SDL_MapRGB(sdl_primary->format, 247, 185, 166); // team 2 - blue team
    TEAM_HIGHLIGHT_COLOR[3] = SDL_MapRGB(sdl_primary->format, 192, 247, 186); // team 3 - green team
    TEAM_HIGHLIGHT_COLOR[4] = SDL_MapRGB(sdl_primary->format, 166, 237, 241); // team 4 - yellow team
	
    TEAM_BASE_COLOR[0] = SDL_MapRGB(sdl_primary->format, 221, 23, 200); // team 0 - computer team base colour
    TEAM_BASE_COLOR[1] = SDL_MapRGB(sdl_primary->format, 23, 23, 221); // team 1 - red team
    TEAM_BASE_COLOR[2] = SDL_MapRGB(sdl_primary->format, 221, 69, 23); // team 2 - blue team
    TEAM_BASE_COLOR[3] = SDL_MapRGB(sdl_primary->format, 86, 221, 72); // team 3 - green team
    TEAM_BASE_COLOR[4] = SDL_MapRGB(sdl_primary->format, 23, 196, 208); // team 4 - yellow team

    TEAM_SHADOW_COLOR[0] = SDL_MapRGB(sdl_primary->format, 158, 16, 142); // team 0 - computer team shadow colour
    TEAM_SHADOW_COLOR[1] = SDL_MapRGB(sdl_primary->format, 16, 16, 158); // team 1 - red team
    TEAM_SHADOW_COLOR[2] = SDL_MapRGB(sdl_primary->format, 158, 49, 16); // team 2 - blue team
    TEAM_SHADOW_COLOR[3] = SDL_MapRGB(sdl_primary->format, 61, 158, 51); // team 3 - green team
    TEAM_SHADOW_COLOR[4] = SDL_MapRGB(sdl_primary->format, 16, 140, 148); // team 4 - yellow team
    */ // end of VA test reversing color order
    // original code : 
	/*
    TEAM_HIGHLIGHT_COLOR[0] = SDL_MapRGB(sdl_primary->format, 238, 166, 247); // team 0 - computer team highlight
    TEAM_HIGHLIGHT_COLOR[1] = SDL_MapRGB(sdl_primary->format, 247, 166, 166); // team 1 - red team
    TEAM_HIGHLIGHT_COLOR[2] = SDL_MapRGB(sdl_primary->format, 166, 185, 247); // team 2 - blue team
    TEAM_HIGHLIGHT_COLOR[3] = SDL_MapRGB(sdl_primary->format, 186, 247, 192); // team 3 - green team
    TEAM_HIGHLIGHT_COLOR[4] = SDL_MapRGB(sdl_primary->format, 241, 237, 166); // team 4 - yellow team
	
    TEAM_BASE_COLOR[0] = SDL_MapRGB(sdl_primary->format, 200, 23, 221); // team 0 - computer team base colour
    TEAM_BASE_COLOR[1] = SDL_MapRGB(sdl_primary->format, 221, 23, 23); // team 1 - red team
    TEAM_BASE_COLOR[2] = SDL_MapRGB(sdl_primary->format, 23, 69, 221); // team 2 - blue team
    TEAM_BASE_COLOR[3] = SDL_MapRGB(sdl_primary->format, 72, 221, 86); // team 3 - green team
    TEAM_BASE_COLOR[4] = SDL_MapRGB(sdl_primary->format, 208, 196, 23); // team 4 - yellow team

    TEAM_SHADOW_COLOR[0] = SDL_MapRGB(sdl_primary->format, 142, 16, 158); // team 0 - computer team shadow colour
    TEAM_SHADOW_COLOR[1] = SDL_MapRGB(sdl_primary->format, 158, 16, 16); // team 1 - red team
    TEAM_SHADOW_COLOR[2] = SDL_MapRGB(sdl_primary->format, 16, 49, 158); // team 2 - blue team
    TEAM_SHADOW_COLOR[3] = SDL_MapRGB(sdl_primary->format, 51, 158, 61); // team 3 - green team
    TEAM_SHADOW_COLOR[4] = SDL_MapRGB(sdl_primary->format, 148, 140, 16); // team 4 - yellow team
	*/
	// end of original code
	
	  
    teamColoursInitialised = true;

  }
  
  // VA NOTE : below is called for many BMP with CYAN aka rgb(0,255, 255) as Color Key : 
  //           block.bmp, egg1.bmp, ... , en.bmp, iq.bmp, ..., damage.bmp 
  
  // VA :try using our own BMP loader with color key :
  m_animSurface[0] = std::MickSDLAssets::Load_BMP_With_ColorKey(filename, 0, 255, 255, sdl_primary->format);
  
  /*  // originl code: 
  SDL_Surface* tempSurface = SDL_LoadBMP(filename);
  
  if(tempSurface)
  { 
    // VA tempo debug : // SDL_PIXELFORMAT_RGBA8888 first hen SDL_PIXELFORMAT_BGR24 on both MorphOS PPC and Linux x86 :
    //fprintf(stdout,"EntityRenderer(..) > From SDL_LoadBMP, SDL_Surface format: %s \n", SDL_GetPixelFormatName(tempSurface->format->format)); 	  
	  
    //VA test loading non converted :
	//m_animSurface[0] = tempSurface;
	  
	// original code :	
	m_animSurface[0] = SDL_ConvertSurface(tempSurface, sdl_primary->format, 0); 
	
	// VA tempo debug : // SDL_PIXELFORMAT_BGRA8888 on MorphOS PPC versus of RGB888 Linux x86 :
	//fprintf(stdout,"EntityRenderer(..) > converted m_animSurface[0], SDL_Surface format: %s \n", SDL_GetPixelFormatName(m_animSurface[0]->format->format)); 	  

    SDL_FreeSurface(tempSurface);
    tempSurface = NULL;	  
  } 
 */ // original code

  // set number of frames based on width
  for (int i = 0; i < NUM_TEAMS; ++i)
  {
    m_numFrames[i] = m_animSurface[0]->w / Map::TILE_WIDTH;
  }

  SDL_Rect sourceRect, destRect;
  sourceRect.y = destRect.y = 0;
  sourceRect.x = destRect.x = 0;
  sourceRect.h = destRect.h = m_animSurface[0]->h;
  sourceRect.w = destRect.w = m_animSurface[0]->w;

  for (int i = 1; i < NUM_TEAMS; ++i)
  {
	// VA HACK for Morphos BIG ENDIAN case (ARGB format below, not RGBA) :
    //	test 1: (OK but inefficient globally)  seems recover colors to allow painting pixels for each team created entities (but colors channel still incorrect ?) :
	//tempSurface = SDL_CreateRGBSurface(0, m_animSurface[0]->w, m_animSurface[0]->h, m_animSurface[0]->format->BitsPerPixel, 0,m_animSurface[0]->format->Bmask, m_animSurface[0]->format->Gmask, m_animSurface[0]->format->Rmask);
	// VA : global BGRA 88888 approach :
    //tempSurface = SDL_CreateRGBSurface(0, m_animSurface[0]->w, m_animSurface[0]->h, m_animSurface[0]->format->BitsPerPixel, m_animSurface[0]->format->Bmask, m_animSurface[0]->format->Gmask, m_animSurface[0]->format->Rmask, 0);
    // Test 2 : Below is identical to original code, except it re use the m_animSurface format :
	//tempSurface = SDL_CreateRGBSurfaceWithFormat(0, m_animSurface[0]->w, m_animSurface[0]->h, m_animSurface[0]->format->BitsPerPixel, m_animSurface[0]->format->format);	  
	SDL_Surface *tempSurface = SDL_CreateRGBSurfaceWithFormat(0, m_animSurface[0]->w, m_animSurface[0]->h, m_animSurface[0]->format->BitsPerPixel, m_animSurface[0]->format->format);	  
	// original code : 
	//tempSurface = SDL_CreateRGBSurface(0, m_animSurface[0]->w, m_animSurface[0]->h, m_animSurface[0]->format->BitsPerPixel, m_animSurface[0]->format->Rmask, m_animSurface[0]->format->Gmask, m_animSurface[0]->format->Bmask, 0);

    // copy to this surface
    SDL_BlitSurface(m_animSurface[0], &sourceRect, tempSurface, &destRect);
    m_animSurface[i] = tempSurface;
  }

  // change to team colours and apply color key
  for (int i = 0; i < NUM_TEAMS; ++i)
  {	
    changeToTeamColour(m_animSurface[i], i);
		
	//	VA :   Fixing the original CYAN color (0, 255,255) , HIDDEN , as  used as color key :  
	Uint32 colorKey = SDL_MapRGBA(m_animSurface[i]->format, 0, 255, 255, 0);	
	// tempo debug display : 
	//Uint8 *ck = (Uint8 *) &colorKey;
	//fprintf(stdout,"EntityRenderer::EntityRenderer(..) colorKey seen as %08x: %d %d %d %d \n",colorKey, ck[0], ck[1], ck[2], ck[3] ); 
	//SDL_SetColorKey( m_animSurface[i], SDL_TRUE, SDL_MapRGB(m_animSurface[i]->format, 255, 255, 0) ); // VA test : assuming BGR order  , but 24 bpp globally inefficent versus 32 bpp manips
	SDL_SetColorKey( m_animSurface[i], SDL_TRUE, colorKey ); // VA test : assuming ?? order  
	
	// VA NOTE : the color key just means ignore that color, it does not create a real alpha channel (you need SDL_ConvertSurfaceFormat() to create a surface with matching Alpha channel )
	  
	// original code (RGB?) :
	//SDL_SetColorKey( m_animSurface[i], SDL_TRUE, SDL_MapRGB(m_animSurface[i]->format, 0, 255, 255) ); 
	
  }

}


EntityRenderer::~EntityRenderer()
{
  // if pointer is valid, release the surfaces
  for (int i = 0; i < NUM_TEAMS; ++i)
    if (m_animSurface[i])
    {
      SDL_FreeSurface(m_animSurface[i]);
      m_animSurface[i] = NULL;
      //m_animSurface[i]->Release();
    }

}

bool EntityRenderer::render(SDL_Surface* dest, int destX, int destY, int state, int animFrame, int team)
{
  if (state < 0 || state >= MAX_ANIM_STATES)
  {
    return false;
  }
  if (animFrame < 0 || animFrame >= m_numFrames[team])
  {
    return false;
  }
  if (team < 0 || team >= NUM_TEAMS)
  {
    return false;
  }

  //RECT sourceRect, destRect;
  SDL_Rect sourceRect, destRect;

  sourceRect.x = animFrame * Map::TILE_WIDTH;
  //sourceRect.w = (animFrame + 1) * Map::TILE_WIDTH;
  sourceRect.w = Map::TILE_WIDTH;
  sourceRect.y = state * Map::TILE_HEIGHT;
  //sourceRect.h = (state + 1) * Map::TILE_HEIGHT;
  sourceRect.h = Map::TILE_HEIGHT;

  destRect.x = destX;
  destRect.w = destX + Map::TILE_WIDTH;
  destRect.y = destY;
  destRect.h = destY + Map::TILE_HEIGHT;

  //HRESULT result = dest->Blt(&destRect, m_animSurface[team], &sourceRect, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
  //printf("EntityRenderer::render - m_animSurface[%d]-w: %d, sourceRect.w=%d\n", team, m_animSurface[team]->w, sourceRect.w );
  if (SDL_BlitSurface(m_animSurface[team], &sourceRect, dest, &destRect) != 0)
  {
    printf("EntityRenderer::render - SDL_BlitSurface failed: %s\n", SDL_GetError());
    return false;
  }

  return true;

}

void EntityRenderer::changeToTeamColour(SDL_Surface* surface, int team)
{
	
  //VA Debug :
  /*	
  static bool test =false;
  if (!test) 
   {
	 test = true;
	 fprintf(stdout,"EntityRenderer::changeToTeamColour(..) SDL_Surface format: %s \n", SDL_GetPixelFormatName(surface->format->format)); 
	 int bpp = surface->format->BytesPerPixel;
	 fprintf(stdout,"EntityRenderer::changeToTeamColour(..) surface bpp : %d \n",bpp); 
	 Uint32 val1 = TEAM_ORIGINAL_HIGHLIGHT_COLOR;
	 Uint8 *p = (Uint8 *) &val1;
	 fprintf(stdout,"EntityRenderer::changeToTeamColour(..) TEAM_ORIGINAL_HIGHLIGHT_COLOR seen as %08x: %d %d %d %d \n",val1, p[0], p[1], p[2], p[3] ); 
	 Uint32 val2 = TEAM_ORIGINAL_BASE_COLOR;
	 p = (Uint8 *) &val2;
	 fprintf(stdout,"EntityRenderer::changeToTeamColour(..) TEAM_ORIGINAL_BASE_COLOR seen as %08x: %d %d %d %d \n", val2, p[0], p[1], p[2], p[3] ); 
	 Uint32 val3 = TEAM_ORIGINAL_SHADOW_COLOR;
	 p = (Uint8 *) &val3;
	 fprintf(stdout,"EntityRenderer::changeToTeamColour(..) TEAM_ORIGINAL_SHADOW_COLOR seen as %08x: %d %d %d %d \n",val3, p[0], p[1], p[2], p[3] ); 	 
	 // check also a RGBA mapped color as example : 
	 Uint32 val4 = TEAM_HIGHLIGHT_COLOR[0];
	 p = (Uint8 *) &val4;
	 fprintf(stdout,"EntityRenderer::changeToTeamColour(..) TEAM_HIGHLIGHT_COLOR[0] seen as %08x: %d %d %d %d \n",val4, p[0], p[1], p[2], p[3] ); 	 
   }
   */	
   // end of VA Debug
  	
	
  //DDSURFACEDESC2 ddsd;
  //memset(&ddsd,0,sizeof(ddsd));
  //ddsd.dwSize = sizeof(ddsd);

  // lock surface for manipulation
  //surface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
  if(SDL_MUSTLOCK(surface))
  {
    SDL_LockSurface(surface);
  }
  //surface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

  //unsigned int * buffer = (unsigned int *)ddsd.lpSurface;
  //Uint32* buffer = (Uint32* )surface->pixels;

  // loop through all of image
  for (int x = 0; x < surface->w; ++x)
  {
    for (int y = 0; y < surface->h; ++y)
    {
      Uint32 pixel = SDLHelper::GetPixel(surface, x, y);
      //Uint32 pixel = buffer[x + y*(ddsd.lPitch >> 2)];

	  //VA Debug 2:
	  /*	
  	  static int debCount = 0;
      if (debCount < 32) // display 30 pixels values for info (32 is a width of a tile)
   	   {
         debCount++;		   
	     Uint8 *pix = (Uint8 *) &pixel;
	 	 fprintf(stdout,"EntityRenderer::changeToTeamColour(..) query pixel seen as %08x: %d %d %d %d \n",pixel, pix[0], pix[1], pix[2], pix[3] ); 
	   }
	  */	
	  // End of VA Debug 2	
		
      if (pixel == TEAM_ORIGINAL_HIGHLIGHT_COLOR)
      {
        SDLHelper::PutPixel(surface, x, y, TEAM_HIGHLIGHT_COLOR[team]);
        //buffer[x + y*(ddsd.lPitch >> 2)] = TEAM_HIGHLIGHT_COLOR[team];
      }
      else
      {
        if (pixel == TEAM_ORIGINAL_BASE_COLOR)
        {
          SDLHelper::PutPixel(surface, x, y, TEAM_BASE_COLOR[team]);
          //buffer[x + y*(ddsd.lPitch >> 2)] = TEAM_BASE_COLOR[team];
        }
        else if (pixel == TEAM_ORIGINAL_SHADOW_COLOR)
        {
          SDLHelper::PutPixel(surface, x, y, TEAM_SHADOW_COLOR[team]);
          //buffer[x + y*(ddsd.lPitch >> 2)] = TEAM_SHADOW_COLOR[team];
        }
      }
    }
  }

  if(SDL_MUSTLOCK(surface))
  {
    SDL_UnlockSurface(surface);
  }

  /*
  // loop through all of image
  for (unsigned int x = 0; x < ddsd.dwWidth; ++x)
  {
    for (unsigned int y = 0; y < ddsd.dwHeight; ++y)
    {
      unsigned int pixel = buffer[x + y*(ddsd.lPitch >> 2)];
      if (pixel == TEAM_ORIGINAL_HIGHLIGHT_COLOR)
      {
        buffer[x + y*(ddsd.lPitch >> 2)] = TEAM_HIGHLIGHT_COLOR[team];
      } else {
        if (pixel == TEAM_ORIGINAL_BASE_COLOR)
        {
          buffer[x + y*(ddsd.lPitch >> 2)] = TEAM_BASE_COLOR[team];
        } else {
          if (pixel == TEAM_ORIGINAL_SHADOW_COLOR)
          {
            buffer[x + y*(ddsd.lPitch >> 2)] = TEAM_SHADOW_COLOR[team];
          }
        }
      }
    }
  }
  */
  //surface->Unlock(NULL);

}

bool EntityRenderer::replaceTeamSurfaceWithImage(SDL_Surface* sdl_primary, const char * filename, int team)
{
	
	// VA test 3 : use my optimized BMP load method
	//SDL_Surface* optimSurface = loadBMPWithColorKey(filename, 0, 255, 255, sdl_primary->format);
	SDL_Surface* optimSurface = std::MickSDLAssets::Load_BMP_With_ColorKey(filename, 0, 255, 255, sdl_primary->format);  // changed method type & class
	if (NULL == optimSurface)
		return false;
	
	if (m_animSurface[team])
    {      
      SDL_FreeSurface(m_animSurface[team]);
    }
	m_animSurface[team] = optimSurface;	  
	
	optimSurface = NULL;
	
	//printf("Not sure about this - CHECK M2S surface width of BMP: %d", m_animSurface[team]->w);
    m_numFrames[team] = m_animSurface[team]->w / Map::TILE_WIDTH;
	
	return true;
	// End of VA test 3

/* VA
  // load anim surface from file using ddutil.h
  //LPDIRECTDRAWSURFACE7 tempSurface = DDLoadBitmap(lpdd, filename, 0, 0);
  SDL_Surface* tempSurface = SDL_LoadBMP(filename);

  if (tempSurface)
  {
	 
	// VA tempo debug : // SDL_PIXELFORMAT_BGR24 , used to load player1male.bmp, ....  (used for sprites of moving computer/human caracter )
    //fprintf(stdout,"EntityRenderer::replaceTeamSurfaceWithImage(..) > From SDL_LoadBMP on %s, SDL_Surface format: %s \n", filename, SDL_GetPixelFormatName(tempSurface->format->format)); 	  
	  
	// VA : try to load bmp with no converting 
	//
	//if (m_animSurface[team])
    //{      
    //  SDL_FreeSurface(m_animSurface[team]);
    //}
	//m_animSurface[team] = tempSurface;	  
	//tempSurface = NULL;
	//
	// End of VA : try to load bmp with no converting
	  
	// VA test 2 : do exact same approach as in above loading BMP call (that work fine), using Blitting
    //
	//SDL_Rect sourceRect, destRect;
    //sourceRect.y = destRect.y = 0;
    //sourceRect.x = destRect.x = 0;
    //sourceRect.h = destRect.h = tempSurface->h;
    //sourceRect.w = destRect.w = tempSurface->w;
	
	////SDL_Surface* optSurface = SDL_CreateRGBSurface(0, tempSurface->w, tempSurface->h, tempSurface->format->BitsPerPixel, 0,tempSurface->format->Bmask, tempSurface->format->Gmask, tempSurface->format->Rmask);
	//SDL_Surface* optSurface = SDL_CreateRGBSurface(0, tempSurface->w, tempSurface->h, tempSurface->format->BitsPerPixel, tempSurface->format->Rmask,tempSurface->format->Gmask, tempSurface->format->Bmask, tempSurface->format->Amask);
	// copy to this surface
    //SDL_BlitSurface(tempSurface, &sourceRect, optSurface, &destRect);
	//SDL_FreeSurface(tempSurface);
    //tempSurface = NULL;	
	//
	// end of VA test 2	  
	  
	//original code :
    //SDL_Surface* optSurface = SDL_ConvertSurface(tempSurface, sdl_primary->format, 0);
	  
    // release current surface
    if (m_animSurface[team])
    {
      //m_animSurface[team]->Release();
      SDL_FreeSurface(m_animSurface[team]);
    }
    // set surface from temp
    m_animSurface[team] = optSurface;
	// end of original code 
	
	// VA tempo debug : // SDL_PIXELFORMAT_BGRA8888 on MorphOS :
	//fprintf(stdout,"EntityRenderer::replaceTeamSurfaceWithImage(..) > converted m_animSurface[team], SDL_Surface format: %s \n", SDL_GetPixelFormatName(m_animSurface[team]->format->format)); 	    

    // set number of frames
    //DDSURFACEDESC2 ddsd;
    //memset(&ddsd,0,sizeof(ddsd));
    //ddsd.dwSize = sizeof(ddsd);
    //m_animSurface[team]->GetSurfaceDesc(&ddsd);

    //printf("Not sure about this - CHECK M2S surface width of BMP: %d", m_animSurface[team]->w);
    m_numFrames[team] = m_animSurface[team]->w / Map::TILE_WIDTH;

    //DDCOLORKEY key;
    //key.dwColorSpaceLowValue = TRANSPARENT_COLOR;
    //key.dwColorSpaceHighValue = TRANSPARENT_COLOR;
    //m_animSurface[team]->SetColorKey(DDCKEY_SRCBLT, &key);

	// VA : Fixing the original PINK color (0, 255,255) , HIDDEN , as  used as color key :
    //SDL_SetColorKey(m_animSurface[team], SDL_TRUE, SDL_MapRGB(m_animSurface[team]->format, 0, 255, 255)); // << original code	
	SDL_SetColorKey(m_animSurface[team], SDL_TRUE, SDL_MapRGBA(m_animSurface[team]->format, 0, 255, 255, 0) ); // VA test : assuming BGRA order 

    if(tempSurface)
    {
      SDL_FreeSurface(tempSurface);
      tempSurface = NULL;
    }

    return true;
  }
  else
  {
    return false;
  }
 */ //VA
}
