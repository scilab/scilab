/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "loadTextRenderingAPI.h"
#include "BOOL.h"
#include "loadOnUseClassPath.h"

/* Variable to store if you have already loaded or not the Latex 
 * dependencies */
static BOOL loadedDepLatex = FALSE;
/* Variable to store if you have already loaded or not the MathML
 * dependencies */
static BOOL loadedDepMathML = FALSE;

void loadTextRenderingAPI(char **text, int nbRow, int nbCol)
{

	int i;

	/* We already loaded both, don't need to check again */
	if (loadedDepLatex && loadedDepMathML) 
	{
		return; 
	}


	/* For each element in the array, look if the text starts by:
	 * '$' for latex
	 * '<' for MathML
	 */
	for ( i = 0 ; i < nbRow * nbCol ; i++ )
	{
		if (text[i][0]=='$' && !loadedDepLatex) /* One of the string starts by a $. This might be a Latex expression */
		{
			loadOnUseClassPath("graphics_latex_textrendering");
			loadedDepLatex=TRUE;
		}

		if (text[i][0]=='<' && !loadedDepMathML) /* One of the string starts by a <. This might be a MathML expression */
		{
			loadOnUseClassPath("graphics_mathml_textrendering");
			loadedDepMathML=TRUE;
		}
	}

}
