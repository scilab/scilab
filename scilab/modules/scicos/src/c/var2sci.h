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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
/* var2sci function to convert an array of scicos
* blocks to scilab object in the Top+1 position
* in the stack.
*
* Input parameters :
* *x       : void ptr, scicos blocks array to store
*            in the Top+1 position in the stack.
* n        : integer, number of rows.
* m        : integer, number of columns.
* typ_var  : integer, type of scicos data :
*            SCSREAL    : double real
*            SCSCOMPLEX : double complex
*            SCSINT     : int
*            SCSINT8    : int8
*            SCSINT16   : int16
*            SCSINT32   : int32
*            SCSUINT    : uint
*            SCSUINT8   : uint8
*            SCSUINT16  : uint16
*            SCSUINT32  : uint32
*            SCSUNKNOW  : Unknown type
*
* Output parameters : int (<1000), error flag
*                     (0 if no error)
*
* 07/06/06, Alan    : initial version.
*
* 23/06/06, Alan    : moved in intcscicos.c to do
*                     the connection with getscicosvars("blocks")
*
* 09/02/07, Alan    : add unknown type of objects
*
*/

#ifndef __VAR2SCI_H__
#define __VAR2SCI_H__

int var2sci(void *x, int n, int m, int typ_var);

#endif /* __VAR2SCI_H__ */
