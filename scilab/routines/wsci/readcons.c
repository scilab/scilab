/* Copyright (C) 1998-2002 Chancelier Jean-Philippe */

/**************************************
 * performs I/O in a console window 
 * (see wtextc.h for the definition of the used io functions
 * (1997) : Jean-Philippe Chancelier 
 * 
 **************************************/

/* with cygwin the two solutions should work 
   (i.e with _Windows or without)
   with __MINGWIN32__ and other windows compilers 
   #define _Windows is needed

   when  _Windows is activated see 
   wtextc.h and in winmain.c WriteStr and WriteKey 
   file add #define UseWIN

 */

#ifndef  __CYGWIN32__
#define _Windows
#endif

#define  USE_CONSOLE
#include "readline.c"
