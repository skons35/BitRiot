/*
 * MickRmlUIElementUtil.h
 *
 *  Created on: 25 May 2017
 *      Author: msaun
 */

#ifndef MICKROCKETELEMENTUTIL_H_
#define MICKROCKETELEMENTUTIL_H_

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
#include <string>
#include "RuntimeException.h"

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include <RmlUi/Core.h>

class MickRmlUIElementUtil
{
  public:
    MickRmlUIElementUtil();

    static int getTabIndex(Rml::Element* element);
    static Rml::Element* getChildElementWithTabIndex(Rml::Element* parentNode, int tabIndex);
    static void appendTextToTextNode(Rml::Element* element, std::string toAppend);
    static void replaceEndStringInTextNode(Rml::Element* element, std::string toReplace);
    static Rml::Element* getFirstElementWithAttribute(Rml::Element *searchFrom, std::string attributeName);

    virtual ~MickRmlUIElementUtil();
};

#endif /* MICKROCKETELEMENTUTIL_H_ */
