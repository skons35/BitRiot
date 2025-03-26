/*
 * MenuRenderer.h
 *
 *  Created on: Apr 2, 2022
 *      Author: msaun
 */

#ifndef MENUSDLRENDERER_H_
#define MENUSDLRENDERER_H_

// VA : before these types are used (template) : SCNu64, SCNd64, PRId64, PRu64
//      need to provide a def => the one in /gg/include/inttypes.h
#define __PRI_64_LENGTH_MODIFIER__ "ll"
#define PRId64 __PRI_64_LENGTH_MODIFIER__ "d"
#define PRIu64 __PRI_64_LENGTH_MODIFIER__ "u"
#define __SCN_64_LENGTH_MODIFIER__ "ll"
#define SCNd64 __SCN_64_LENGTH_MODIFIER__ "d"
#define SCNu64 __SCN_64_LENGTH_MODIFIER__ "u"
// End of VA

#include <RmlUi/Core/ElementDocument.h>
#include <SDL2/SDL_render.h>
#include "SDL_keycode.h"
#include "SDL_pixels.h"
#include "RuntimeException.h"

#include <SDL2/SDL.h>

#include <RmlUi/Core.h>
#include <RmlUi/Core/Input.h>
#ifndef NDEBUG
#include <RmlUi/Debugger/Debugger.h>
#endif

#include "RmlUI/glue/SystemInterfaceSDL2.h"
#include "RmlUI/glue/RenderInterfaceSDL2.h"
#include "RmlUI/glue/ShellFileInterface.h"


#include "MickLogger.h"
#include "PlayerCharacterEntity.h"
#include "GameSettings.h"
#include "RmlUI/MickRmlUIElementUtil.h"
#include "RmlUI/events/EventInstancer.h"
#include "RmlUI/events/EventManager.h"
#include "RmlUI/events/EventHandlerOptions.h"
#include "RmlUI/ScoreBoardBinder.h"
#include <iostream>
#include <stdexcept>
#include <string>


class MenuSDLRenderer
{
  public:
    MenuSDLRenderer(SDL_Renderer *renderer, SDL_Window *screen);
    void loadScoreBoard(shared_ptr<PlayerCharacterEntity> winner, int gameTotalLength);
    void clearScoreBoard();
    bool showMenu();
    virtual ~MenuSDLRenderer();

  private:
    void handleWindowResize();
    void init(SDL_Renderer* renderer, SDL_Window *screen);
    int getTabIndex(Rml::Element* node);
    Rml::Element* getChildElementWithTabIndex(Rml::Element* parentNode, int tabIndex);
    Rml::Context* m_context;
    Rml::ElementDocument* loadMenu(std::string menuRmlFile, bool loadAndShow = true);
    SDL_Window *m_screen;
    SDL_Renderer *m_renderer;
    ScoreBoardBinder *m_scoreBinder = nullptr;
};

#endif /* MENUSDLRENDERER_H_ */
