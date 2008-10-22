/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
	#include <Windows.h>
#endif
#include "vect_and.h"
/*--------------------------------------------------------------------------*/
void vect_and(int *v,int m, int n,int *r,int opt)
{
	int k,l,i;

	if (opt==0) {
		r[0]=1;
		for (k=0;k<m*n;k++)
			if (!v[k]) {
				r[0]=0;
				break;
			}
	}
	else if (opt==1) {

		for (k=0;k<n;k++) {
			r[k]=1;i=k*m;
			for (l=0;l<m;l++)
				if (!v[i++]) {
					r[k]=0;
					break;
				}
		}
	}
	else if (opt==2) {
		for (l=0;l<m;l++) {
			r[l]=1;i=l;
			for (k=0;k<n;k++) {
				if (!v[i]) {
					r[l]=0;
					break;
				}
				i+=m;
			}
		}
	}
}
/*--------------------------------------------------------------------------*/
