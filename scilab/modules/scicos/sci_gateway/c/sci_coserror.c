/*  Scicos
*
*  Copyright (C) INRIA - 
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
#include <stdio.h>
#include "gw_scicos.h"
#include "stack-c.h"
#include "Scierror.h"
#include "scicos_block.h"
#include "cvstr.h"
#include "localization.h"
#include "scicos-def.h"
/*--------------------------------------------------------------------------*/
/* Coserror : break the simulation and return a message
*            in the scicos editor
*
* []=Coserror('errmsg')
*
* Input : first rhs is a string
*
*/
/*--------------------------------------------------------------------------*/
/* TO DO : cleaning this */
extern struct {int isrun;} C2F(cosim); 
extern COSERR_struct coserr;  
/*--------------------------------------------------------------------------*/
int sci_coserror(char *fname, unsigned long fname_len)
/* renvoi un message erreur */
{
	/* auxilary variables for dimension and address */
	int m1 = 0,n1 = 0;     /* dimension of input character string               */
	int *il_str = NULL;   /* address of the description of the input parameter */
	int *l_str = NULL;    /* address of the data of the input parameter        */
	int sz_str,ptr_pos = 0;
	int i = 0, j = 0;

	int isrun = C2F(cosim).isrun;

	CheckRhs(1,1);

	if (!isrun) 
	{
		Scierror(999,_("%s: scicosim is not running.\n"),fname);
	}
	else 
	{
		il_str = (int *) GetData(1); /* get ptr of integer header of rsh 1 */
		/* check for a string */
		if(il_str[0] != 10) 
		{
			Scierror(55,_("%s : First argument must be a string.\n"),fname);
			C2F(iop).err = 1;
			return 0;
		}

		m1 = il_str[1]; /* number of row    */
		n1 = il_str[2]; /* number of column */
		sprintf(coserr.buf," ");
		/* Check string matrix  */
		for (j=0; j < m1*n1; j++)
		{
			sz_str = il_str[5+j]-il_str[4+j]; /* store the length of str */
			/* get current position in the istk */
			if (j==0)
			{
				ptr_pos = 5+m1*n1;
			}
			else
			{
				ptr_pos += il_str[5+j-1]-il_str[4+j-1];
			}
			l_str = &il_str[ptr_pos]; /* get ptr of rsh 1 */
			/* codetoascii convertion */
			C2F(cha1).buf[0]=' ';
			C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(i=1,&i),sz_str);
			C2F(cha1).buf[sz_str]='\0';

			sprintf(coserr.buf,"%s\n%s",coserr.buf,C2F(cha1).buf);
		}
		set_block_error(-5);
		LhsVar(1) = 0;
		PutLhsVar();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
