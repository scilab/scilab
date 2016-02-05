/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010-2012 - Scilab Enterprises - Bruno JOFRET
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

/*------------------------------------------------------------------------
 *    Graphic library
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to BUILD new objects :
 - break the binding between the deleted object and its parent in the
 existing hierarchy
 - freeing memory
 --------------------------------------------------------------------------*/

#include "DestroyObjects.h"
#include "sci_malloc.h" /* MALLOC */
#include "FigureList.h"
#include "deleteGraphicObject.h"

/*---------------------------------------------------------------------------*/
/**
 * Close the figure with number winNum.
 */
void sciDeleteWindow(int winNum)
{
    deleteGraphicObject(getFigureFromIndex(winNum));
}
/*--------------------------------------------------------------------------------*/
void AllGraphWinDelete(void)
{

    int num = sciGetNbFigure();
    int *ArrayWGraph = NULL;

    /* sciGetIdFigure (ArrayWGraph,&num,&iflag); */

    if (num > 0)
    {
        int i = 0;
        ArrayWGraph = (int*)MALLOC(sizeof(int) * num);

        sciGetFiguresId(ArrayWGraph);

        for (i = 0; i < num; i++)
        {
            sciDeleteWindow(ArrayWGraph[i]);
        }
        FREE (ArrayWGraph);
        ArrayWGraph = NULL;
    }
}
