/*
 * MickSDLAssets.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: msaun
 */

#include "MickSDLAssets.h"

namespace std
{

MickSDLAssets::MickSDLAssets()
{

}

SDL_Surface* MickSDLAssets::Load_BMP(const char* pathfile)
{
  if(! MickUtil::CheckFileExists(pathfile))
  {
    string fname(pathfile);
    RuntimeException e;
    e.setMessage( "MickSDLAssets::Load_BMP-Resource not found: " + fname);
    throw e;
  }

  return SDL_LoadBMP(pathfile);
}

SDL_Texture* MickSDLAssets::LoadTexture_BMP(SDL_Renderer* renderer, const char* pathfile)
{
  SDL_Texture* tex = NULL;
  SDL_Surface* surface = Load_BMP(pathfile);
  if(NULL != surface)
  {
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
  }
  return tex;
}

MickSDLAssets::~MickSDLAssets()
{
}

//VA added: 
// params :  filename,  color key (r,g,b), target optimized format
// returns an optimized surface with alpha channel generated from provided color key applied on surface loaded from BMP file
SDL_Surface* MickSDLAssets::Load_BMP_With_ColorKey(const char * filename, Uint8 r, Uint8 g, Uint8 b, const SDL_PixelFormat * targetPixelFormat)
{
  SDL_Surface* tempSurface = SDL_LoadBMP(filename);

  if (!tempSurface)
	  return NULL;
  
  //Uint32 colorKey = SDL_MapRGB(tempSurface->format, r,g,b);
  Uint32 colorKey = SDL_MapRGBA(tempSurface->format, 0,b,g,r);
  SDL_SetColorKey(tempSurface, SDL_TRUE, colorKey);
  
  SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(tempSurface, targetPixelFormat->format, 0); // targetPixelFormat would be BGRA8888 on MorphOS typically
  
  
  SDL_FreeSurface(tempSurface);
  	
  return optimizedSurface;	
}

} /* namespace std */


