/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __MATSCICOS__
#define __MATSCICOS__

#ifdef WIN32
  #include <windows.h>
  #include <stdio.h>
#endif
#include <string.h>
#include "machine.h"
#include "../stack-c.h"


typedef int (*des_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  des_interf f;    /** function **/
  char *name;      /** its name **/
} intcscicosTable;

#endif /*  __MATSCICOS__ */
/*-----------------------------------------------------------------------------------*/
