/*
 * SDLHelper.cpp
 *
 *  Created on: Mar 4, 2012
 *      Author: msaun
 */

#include "SDLHelper.h"

namespace std
{

SDLHelper::SDLHelper()
{
  // TODO Auto-generated constructor stub

}
/* OLD
static Uint32 SDLHelper::GetPixel(SDL_Surface* surface, int x, int y )
{

  return ((Uint32 *)surface->pixels)[ ( y * surface->w ) + x ];
}
*/

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 SDLHelper::GetPixel(SDL_Surface *surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
  {
    case 1:
      return *p;

    case 2:
      return *(Uint16 *)p;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      {
		// VA debug :
		//static bool test = false; if (!test) { test = true; fprintf(stdout,"SDLHelper::GetPixel(..) called, 3 bpp , %d %d %d \n",p[0], p[1], p[2] ); }
		  
        return p[0] << 16 | p[1] << 8 | p[2];
      }
      else
      {
        return p[0] | p[1] << 8 | p[2] << 16;
      }

    case 4:		
	  // VA add Endianness trick
      ///*	
	  if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
       {
	      //static bool test = false; 
		  //if (!test) 
		  //  { test = true; fprintf(stdout,"SDLHelper::GetPixel(..) called, 4 bpp , bytes are: %d %d %d %d\n",p[0], p[1], p[2], p[3]); }	  	  
		  return (p[2] << 24 | p[1] << 16 | p[0] << 8 | 0 /* p[3] */ ); // explicit 0 as we want A to 0
	   }
	  else
	   {
	  		return *(Uint32 *)p;  // same as original code
	   } 
	  //*/
	  // end of VA treat
	  //  ORIGINAL CODE :
      //return *(Uint32 *)p;

    default:
      return 0;       /* shouldn't happen, but avoids warnings */
  }
}

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void SDLHelper::PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
  {
    case 1:
      *p = pixel;
      break;

    case 2:
      *(Uint16 *)p = pixel;
      break;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      }
      else
      {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;

    case 4:
		// VA add Endianness trick
	  if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
		  p[0] = (pixel >> 8) & 0xff;
          p[1] = (pixel >> 16) & 0xff;
          p[2] = (pixel >> 24) & 0xff;
		  p[3] = 255;
		}
	  else 
	   {
	    // original code : 
        *(Uint32 *)p = pixel; 
	   } // end of VA add
      break;
  }
}



SDLHelper::~SDLHelper()
{
  // TODO Auto-generated destructor stub
}

} /* namespace std */
