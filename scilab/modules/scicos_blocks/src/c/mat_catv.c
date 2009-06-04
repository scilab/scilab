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
#include <stdio.h>
# include "scicos_block4.h"
# include "machine.h"



extern void matz_catv();

void mat_catv(scicos_block *block,int flag)
{
	int mu,nu,nin,so,pointerposition,ot,i,j;
	ot=GetOutType(block,1);
	nu =GetInPortCols(block,1);
	if (ot== SCSCOMPLEX_N){
		matz_catv(block,flag);
	} 
	else{
		void *u,*y;
		y=GetOutPortPtrs(block,1);
		nin=GetNin(block);
		if ((flag==1) || (flag==6)) {
			pointerposition=0;
			for (j=0;j<nu;j++) {
				for (i=0;i<nin;i++) { 
					u=GetInPortPtrs(block,i+1);
					mu=GetInPortRows(block,i+1);
					so=GetSizeOfIn(block,i+1);
					memcpy((int*)y+pointerposition,(int*)u+j*mu*so,mu*so);
					pointerposition=pointerposition+mu*so;
				}
			}
		}
	}
}