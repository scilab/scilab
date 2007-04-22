/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "stack-def.h"
#include "basout.h"
#include "../../fileio/src/c/diary.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(xscimore)();
extern int C2F(writelunitstring)();
extern int C2F(xscion)();
extern int C2F(xscistring)();
/*-----------------------------------------------------------------------------------*/ 
int C2F(basout)(integer *io,integer *lunit,char *string,long int notused)
{
	static integer iflag;

	static integer ich;

	if (*lunit == C2F(iop).wte)
	{
		/* Sortie sur la sortie standard */
		C2F(xscion)(&iflag);
		*io = 0;
		if (C2F(iop).lct[0] == -1) { return 0; }
		if (C2F(iop).lct[1] > 0) 
		{
			/* Gestion de la pagination */
			if (C2F(iop).lct[0] + 3 > C2F(iop).lct[1])
			{
				/* nombre maxi de ligne atteint,gestion du more */
				C2F(iop).lct[0] = 0;
				if (iflag == 0) 
				{
					int ch;
					/* scilab n'a pas de  fenetre propre */
					sciprint(" more ? ");
					ch = getchar();
					if ( (ch != ' ') && (ch != '\n') && (ch != 'y') ) ich = 1;
				}
				else
				{
					/* scilab a une  fenetre  en propre */
					C2F(xscimore)(&ich);
				}

				if (ich == 1) 
				{
					C2F(iop).lct[0] = -1;
					*io = -1;
					return 0;
				}
			} 
			else
			{
				++C2F(iop).lct[0];
			}
		}

		if (iflag == 0) 
		{
			C2F(writelunitstring)(lunit, string, notused);
		} 
		else 
		{
			C2F(xscistring)(string, &notused, notused);
		}
		/* write to diary file if required */
		C2F(diary)(string, &notused);
	} 
	else
	{
		/* sortie sur fichier */
		if (*lunit == C2F(iop).wio) 
		{
			C2F(diary)(string, &notused);
		}
		else 
		{
			C2F(writelunitstring)(lunit, string,notused);
		}
	}
	return 0;
} 
/*-----------------------------------------------------------------------------------*/ 
