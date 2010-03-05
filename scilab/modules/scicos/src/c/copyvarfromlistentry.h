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
/*--------------------------------------------------------------------------*/ 
#ifndef __COPYVARFROMLISTENTRY_H__
#define __COPYVARFROMLISTENTRY_H__

/* CopyVarFromlistentry
*    Copy a Scilab object in a list to the variable position  lw
*
* Calling sequence :
*  int CopyVarFromlistentry(int lw, int *header, int i)
*
* Input parameters : lw : integer, the free position
*                    header : int pointer, a pointer of a list.
*                    i : integer, give the number of the element to copy
*
* Output : FALSE if failed, TRUE else.
*
* Examples of use 
*
* 1 -  put the third element of a list given in position lw=1 
*      to position lw=2 :
*
*  int *il_list;
*  il_list = (int *) Getdata(1);
*  CopyVarFromlistentry(2, il_list, 3)
*
* 2 - put the second element of a list stored in the fourth element 
*     of a list in position lw=1 to position lw=3 :
*
*  int *il_list;
*  il_list = (int *) Getdata(1);
*  int *il_sublist;
*  il_sublist = (int *) listentry(il_list,4);
*  CopyVarFromlistentry(3, il_sublist, 2)
*/
int CopyVarFromlistentry(int lw, int *header, int i);

#endif /* __COPYVARFROMLISTENTRY_H__ */
/*--------------------------------------------------------------------------*/ 
