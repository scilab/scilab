/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

#include "math_graphics.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "SetProperty.h"
#include "Interaction.h" /* for callback funtions */
#include "StringMatrix.h"
#include "FigureList.h"
#include "sciprint.h"
#include "InitObjects.h"
#include "ObjectSelection.h"
#include "get_ticks_utils.h"
#include "BuildObjects.h"
#include "HandleManagement.h"
#include "freeArrayOfString.h"
#include "Scierror.h"

//#include "../../../tclsci/includes/GedManagement.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

#include "deleteGraphicObject.h"
#include "CurrentFigure.h"

/*----------------------------------------------------------------------------*/

/**
 * Destroy only the sons of an object.
 * Does not delete the object itself.
 */
int destroyGraphicsSons(sciPointObj * pthis)
{
  /* to destroy only the sons put the while into the switch !*/
  sciSons * toto = NULL;

  toto = sciGetSons (pthis);
  while ((toto != NULL) && (toto->pointobj != NULL))
  {
    destroyGraphicHierarchy((char*)(toto->pointobj));
    toto = sciGetSons(pthis);
  }
  return 0;
}

/********************* modifie le 01/02/2002 ************************
 * On detruit pas la sous fenetre, elle est initialiser avec la figure
 * pour cette version, on considere qu'il y'a 1 seule sous fenetre et
 * elle suit la fenetre principale (voir clf() ), la fenetre n'est pas
 * consideree comme un des fils.
 */
 /**
  * This function destroys childs and pthis
  * @param char * pthis: the entity's identifier
  */
int destroyGraphicHierarchy(char * pthis)
{
    deleteGraphicObject(pthis);

    return 0;
}

/*---------------------------------------------------------------------------*/
/* free the user_data */
void clearUserData( sciPointObj * pObj )
{
  int ** pUserData = NULL;
  int *  pSizeUD = NULL;
  sciGetPointerToUserData( pObj, &pUserData, &pSizeUD ) ;
  if ( pUserData != NULL && *pUserData != NULL )
  {
    FREE( *pUserData ) ;
    *pUserData = NULL ;
    *pSizeUD = 0 ;
  }
}
/*---------------------------------------------------------------------------*/
/**
 * Close the figure with number winNum.
 */
void sciDeleteWindow( int winNum )
{
  //startGraphicDataWriting();
  deleteGraphicObject(getFigureFromIndex(winNum));
  //endGraphicDataWriting();
}
/*--------------------------------------------------------------------------------*/
void AllGraphWinDelete( void )
{

  int num = sciGetNbFigure() ;
  int *ArrayWGraph = NULL ;

  /* sciGetIdFigure (ArrayWGraph,&num,&iflag); */

  if (num > 0)
  {
    int i=0;
    ArrayWGraph=(int*)MALLOC(sizeof(int)*num);

    sciGetFiguresId( ArrayWGraph ) ;

    for (i=0;i<num;i++)
    {
      sciDeleteWindow(ArrayWGraph[i]);
    }
    FREE (ArrayWGraph);
    ArrayWGraph=NULL;
  }
}
