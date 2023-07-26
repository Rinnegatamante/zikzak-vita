/*
 * Project: zikzak
 * Filename: Gfx.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Wrapper for the SDL and OpenGL includes for different platforms
 */

#ifdef WIN32
#include <windows.h>
#endif
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef __vita__
#include <vitaGL.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#endif
#include "SDL.h"
