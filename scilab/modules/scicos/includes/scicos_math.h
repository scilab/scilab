/*  Scicos
*
*  Copyright (C) DIGITEO - 2009 - Allan CORNET
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

#ifndef __SCICOS_MATH_H__
#define __SCICOS_MATH_H__

/*--------------------------------------------------------------------------*/ 
#ifdef abs
	#undef abs
#endif
#define abs(x) ((x) >= 0 ? (x) : -(x))
/*--------------------------------------------------------------------------*/ 
#ifdef max
	#undef max
#endif
#define max(a,b) ((a) >= (b) ? (a) : (b))

#ifdef min
	#undef min
#endif
#define min(a,b) ((a) <= (b) ? (a) : (b))
/*--------------------------------------------------------------------------*/ 
#endif /* __SCICOS_MATH_H__ */
