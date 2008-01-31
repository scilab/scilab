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
#include "scicos_block4.h"
#include <math.h>

void extract_bit_32_RB0(scicos_block *block,int flag)
{
   int i,maxim,numb;
   long *y,*u,ref,n;
   int *ipar;
   y=Getint32OutPortPtrs(block,1);
   u=Getint32InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=32;
   ref=0;
   numb=*(ipar+1)-*ipar+1;
   for (i=0;i<numb;i++)
       {n=(long)pow(2,*ipar+i);
        ref=ref+n;}
   *y=(*u)&(ref);
}
