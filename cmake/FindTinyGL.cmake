FIND_PATH(TINYGL_HEADERS tgl/gl.h  # find tgl/gl.h and set it in TINYGL_HEADERS
  HINTS
  PATH_SUFFIXES os-include  # postfix the path 'os-include' when searching the paths below for 'tgl/gl.h'
  PATHS
  /gg                       # /System/Development/gg   : usual default parent location for dev added SDKs on MorphOS
  #$ENV{RMLUI}  # Use environment variable RMLUI and see if $RMLUI/include/RmlUi/Core.h exists
  #${RMLUI_DIR}/RmlUi
  #/Library/Frameworks
  #/usr/local
)

# TinyGL :  There is a catch : two variant of libGL.a provided ( surely for noixemul use or not)
# one as single libGL.a (in lib/libb32/libinix) , another one along libglut.a (in lib/libnix folder)

# First, find the single first one.
FIND_LIBRARY(TINYGL_LIB GL
  HINTS
  #PATH_SUFFIXES lib lib64 libs/Win32 libs/Win64
  PATH_SUFFIXES ppc-morphos/lib/libb32/libnix
  PATHS
  /gg                       # /System/Development/gg    :  usual default parent location for dev added SDKs on MorphOS
  #$ENV{RMLUI}  # Use environment variable RMLUI and see if $RMLUI/include/RmlUi/Core.h exists
  #${RMLUI_STATIC_DIR}
  #/Library/Frameworks
  #/usr/local
  #/usr
  #/opt/local
  #/opt
)

# Second, find the other variant one..
FIND_LIBRARY(TINYGL_GL_LIB GL
  HINTS
  #PATH_SUFFIXES lib lib64 libs/Win32 libs/Win64
  PATH_SUFFIXES ppc-morphos/lib/libnix
  PATHS
  /gg                       # /System/Development/gg    :  usual default parent location for dev added SDKs on MorphOS
  #$ENV{RMLUI}  # Use environment variable RMLUI and see if $RMLUI/include/RmlUi/Core.h exists
  #/usr/local
  #/usr
)
# ..and its companino lib freeglut :
FIND_LIBRARY(TINYGL_GLUT_LIB glut
  HINTS
  #PATH_SUFFIXES lib lib64 libs/Win32 libs/Win64
  PATH_SUFFIXES ppc-morphos/lib/libnix
  PATHS
  /gg                       # /System/Development/gg    :  usual default parent location for dev added SDKs on MorphOS
  #$ENV{RMLUI}  # Use environment variable RMLUI and see if $RMLUI/include/RmlUi/Core.h exists
  #/usr/local
  #/usr
)

SET(TINGYGL_FOUND "NO")
IF(TINYGL_LIB AND TINYGL_GL_LIB AND TINYGL_GLUT_LIB AND TINYGL_HEADERS)
  SET(TINYGL_FOUND "YES")
  SET(TINYGL_LIBRARIES ${TINYGL_LIB})                            # << use this for linking, OR :
  SET(TINYGL_GL_GLUT_LIBRARIES ${TINYGL_GL_LIB} ${TINYGL_GLUT_LIB})  # << theses ones if noixemul issue with previous (TBC)
ENDIF()

FIND_PACKAGE_HANDLE_STANDARD_ARGS(RmlUi
	REQUIRED_VARS TINYGL_LIBRARIES TINYGL_GL_GLUT_LIBRARIES TINYGL_HEADERS)
