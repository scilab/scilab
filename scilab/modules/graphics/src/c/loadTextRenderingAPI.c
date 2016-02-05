/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

void loadTextRenderingAPI(char const* const* text, int nbRow, int nbCol)
{

    int i = 0;

    /* We already loaded both, don't need to check again */
    if (loadedDepLatex && loadedDepMathML)
    {
        return;
    }


    /* For each element in the array, look if the text starts by:
     * '$' for latex
     * '<' for MathML
     */
    for (i = 0 ; i < nbRow * nbCol ; i++)
    {
        if (text[i][0] == '$' && !loadedDepLatex) /* One of the string starts by a $. This might be a Latex expression */
        {
            loadOnUseClassPath("graphics_latex_textrendering");
            loadedDepLatex = TRUE;
        }

        if (text[i][0] == '<' && !loadedDepMathML) /* One of the string starts by a <. This might be a MathML expression */
        {
            loadOnUseClassPath("graphics_mathml_textrendering");
            loadedDepMathML = TRUE;
        }
    }

}
