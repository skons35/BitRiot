/*
 * MickSDLAssets.h
 *
 *  Created on: Nov 18, 2012
 *      Author: msaun
 */

#ifndef MICKSDLASSETS_H_
#define MICKSDLASSETS_H_

#include "MickAssets.h"
#include <SDL2/SDL.h>


namespace std
{

class MickSDLAssets: public std::MickAssets
{
  public:
    //static SDL_Surface* Load_BMP(string pathfile);
    static SDL_Surface* Load_BMP(const char* pathfile);
    static SDL_Texture* LoadTexture_BMP(SDL_Renderer* renderer, const char* pathfile);
    virtual ~MickSDLAssets();
	
	//VA added:
	static SDL_Surface* Load_BMP_With_ColorKey(const char * filename, Uint8 r, Uint8 g, Uint8 b, const SDL_PixelFormat * targetPixelFormat);

  private:
    MickSDLAssets();
};

} /* namespace std */
#endif /* MICKSDLASSETS_H_ */
