/*
 * Platform.h
 * This header file contains preprocess-time definitions that choose between SFML and SDL
 * libraries
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef LIB_SFML
    #define PLATFORMTYPE Platform_SFML
    #include "platform_sfml.h"
#endif

#ifdef LIB_SDL
    #define PLATFORMTYPE Platform_SDL
    #include "platform_sdl.h"
#endif

#endif

