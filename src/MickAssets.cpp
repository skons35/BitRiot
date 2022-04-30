/*
 * MickAssets.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: msaun
 */

#include "MickAssets.h"
#include "MickLogger.h"
#include <cassert>
#include <sstream>

namespace std
{

MickAssets::MickAssets()
{
  // TODO Auto-generated constructor stub

}

string MickAssets::getPortraitBitmapFilename(int playerIndex, MickAssets::PlayerType type)
{
  stringstream ss;
  ss << "assets/bitmaps/player" << (playerIndex + 1);
  if(type == MickAssets::PlayerType::MALE)
  {
    ss << "male";
  }
  else if(type == MickAssets::PlayerType::FEMALE)
  {
    ss << "female";
  }
  ss << "face.bmp";
  MickLogger::getInstance()->debug(nullptr, ss.str().c_str());
  assert(MickAssets::checkAssetAvailable(ss.str().c_str()));
  return ss.str();
}

MickAssets::PlayerType MickAssets::getNextPlayerType(PlayerType currentType)
{
  switch(currentType)
  {
    case PlayerType::MALE:
      return PlayerType::FEMALE;

    case PlayerType::FEMALE:
      return PlayerType::MALE;

    default:
      MickLogger::getInstance()->warn(nullptr, "Unknown player type found!");
      return PlayerType::MALE; // start from idx 0
  }
}

//cacheBitmap?
//cacheSound?

bool MickAssets::checkAssetAvailable(const char* pathfile)
{
  return MickUtil::CheckFileExists(pathfile);
}

MickAssets::~MickAssets()
{
  // TODO Auto-generated destructor stub
}

} /* namespace std */
