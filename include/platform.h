/*
 * Platform.h
 * This header file contains preprocess-time definitions that choose between SFML and SDL
 * libraries
 */

#ifndef PLATFORM_H
#define PLATFORM_H

// Common for all platforms ------------------------
#define SNDID_EXPLOSION 0
#define SNDID_SHOOT     1
#define SNDID_FSTINV1   2
#define SNDID_FSTINV2   3
#define SNDID_FSTINV3   4
#define SNDID_FSTINV4   5
#define SNDID_INVKLLD   6
#define SNDID_UFOREPT   7
#define SNDID_UFOHIT    8
// -------------------------------------------------

#ifdef LIB_SFML
    #define PLATFORMTYPE Platform_SFML
    #include "platform_sfml.h"
#endif

#ifdef LIB_SDL
    #define PLATFORMTYPE Platform_SDL
    #include "platform_sdl.h"
#endif

#endif

