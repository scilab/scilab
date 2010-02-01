/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_data_property.c                                              */
/* desc : function to retrieve in Scilab the data field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "get_data_property.h"
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnPropertyList.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "SetPropertyStatus.h"

/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the grayplot data is now given as a tlist (like for surface and champ objects) */
int getgrayplotdata(sciPointObj *pobj)
{
  char * variable_tlist[] = {"grayplotdata","x","y","z"};

  /* F.Leray debug*/
  sciGrayplot * ppgrayplot = pGRAYPLOT_FEATURE (pobj);

  /* Add 'variable' tlist items to stack */
  returnedList * tList = createReturnedList( 3, variable_tlist ) ;

  if ( tList == NULL )
  {
    return -1 ;
  }

  addColVectorToReturnedList( tList, ppgrayplot->pvecx, ppgrayplot->nx ) ;

  addColVectorToReturnedList( tList, ppgrayplot->pvecy, ppgrayplot->ny ) ;

  addMatrixToReturnedList( tList, ppgrayplot->pvecz, ppgrayplot->nx, ppgrayplot->ny ) ;

  destroyReturnedList( tList ) ;

  return 0;
}
/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the champ data is now given as a tlist (like for surface objects) */
int getchampdata(sciPointObj *pobj)
{
  char * variable_tlist[] = {"champdata","x","y","fx","fy"};

  /* F.Leray debug*/
  sciSegs * ppsegs = pSEGS_FEATURE (pobj);

  /* Add 'variable' tlist items to stack */

  returnedList * tList = createReturnedList( 4, variable_tlist ) ;

  if ( tList == NULL )
  {
    return -1 ;
  }

  addColVectorToReturnedList( tList, ppsegs->vx, ppsegs->Nbr1 ) ;

  addColVectorToReturnedList( tList, ppsegs->vy, ppsegs->Nbr2 ) ;

  addMatrixToReturnedList( tList, ppsegs->vfx, ppsegs->Nbr1, ppsegs->Nbr2 ) ;

  addMatrixToReturnedList( tList, ppsegs->vfy, ppsegs->Nbr1, ppsegs->Nbr2 ) ;

  destroyReturnedList( tList ) ;

  return 0;
}
/*--------------------------------------------------------------------------*/
int get3ddata(sciPointObj *pobj)
{
  char *variable_tlist_color[] = {"3d","x","y","z","color"};
  char *variable_tlist[] = {"3d","x","y","z"};

  returnedList * tList = NULL ;

  /* tests a faire sur presence et taille color */
  if ( pSURFACE_FEATURE (pobj)->m3n != 0 )
  {

    /* Add 'variable' tlist items to stack */
    tList = createReturnedList( 4, variable_tlist_color ) ;

    if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D)
    {
      int nbRow = pSURFACE_FEATURE (pobj)->m1 ;
      int nbCol = pSURFACE_FEATURE (pobj)->n1 ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE (pobj)->pvecx, nbRow, nbCol ) ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE (pobj)->pvecy, nbRow, nbCol ) ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE (pobj)->pvecz, nbRow, nbCol ) ;
      
      addMatrixToReturnedList( tList, pSURFACE_FEATURE (pobj)->inputCMoV, pSURFACE_FEATURE (pobj)->m3n, pSURFACE_FEATURE (pobj)->n3n ) ;

      
    }
    else if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D)
    {
      
      addMatrixToReturnedList( tList, pSURFACE_FEATURE(pobj)->pvecx, pSURFACE_FEATURE(pobj)->m1, pSURFACE_FEATURE(pobj)->n1 ) ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE(pobj)->pvecy, pSURFACE_FEATURE(pobj)->m2, pSURFACE_FEATURE(pobj)->n2 ) ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE(pobj)->pvecz, pSURFACE_FEATURE(pobj)->m3, pSURFACE_FEATURE(pobj)->n3 ) ;

      addMatrixToReturnedList( tList, pSURFACE_FEATURE(pobj)->inputCMoV, pSURFACE_FEATURE (pobj)->m3n, pSURFACE_FEATURE (pobj)->n3n ) ;
    }
    destroyReturnedList( tList ) ;
  }
  else /* no color provided in input*/
  {

    /* Add 'variable' tlist items to stack */
    tList = createReturnedList( 3, variable_tlist ) ;

    if( pSURFACE_FEATURE(pobj)->typeof3d == SCI_FAC3D )
    {
      int nbRow = pSURFACE_FEATURE (pobj)->m1 ;
      int nbCol = pSURFACE_FEATURE (pobj)->n1 ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE (pobj)->pvecx, nbRow, nbCol ) ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE (pobj)->pvecy, nbRow, nbCol ) ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE (pobj)->pvecz, nbRow, nbCol ) ;
    }
    else if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D)
    {
      addMatrixToReturnedList( tList, pSURFACE_FEATURE(pobj)->pvecx, pSURFACE_FEATURE(pobj)->m1, pSURFACE_FEATURE(pobj)->n1 ) ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE(pobj)->pvecy, pSURFACE_FEATURE(pobj)->m2, pSURFACE_FEATURE(pobj)->n2 ) ;
      addMatrixToReturnedList( tList, pSURFACE_FEATURE(pobj)->pvecz, pSURFACE_FEATURE(pobj)->m3, pSURFACE_FEATURE(pobj)->n3 ) ;
    }
    destroyReturnedList( tList ) ;
  }


  return 0;
}
/*------------------------------------------------------------------------*/
int get_data_property( sciPointObj * pobj )
{

  if ( sciGetEntityType( pobj ) == SCI_SURFACE )
  {
    return get3ddata( pobj ) ;
  }
  else if ( (sciGetEntityType(pobj) == SCI_SEGS) && (pSEGS_FEATURE(pobj)->ptype == 1) )
  {
    return getchampdata( pobj ) ;
  }
  else if ( (sciGetEntityType(pobj) == SCI_GRAYPLOT)  && (pGRAYPLOT_FEATURE(pobj)->type == 0) ) /* case 0: real grayplot */
  {
    return getgrayplotdata( pobj ) ;
  }
  else /* F.Leray 02.05.05 : "data" case for others (using sciGetPoint routine inside GetProperty.c) */
  {
    int nbRow  =  0 ;
    int nbCol  =  0 ;
    int status = SET_PROPERTY_ERROR ;
    /* Warning the following function allocate data */
    double * data = sciGetPoint( pobj, &nbRow, &nbCol ) ;

    if (data == NULL && nbRow == 0 && nbCol == 0)
    {
      /* Empty data */
      sciReturnEmptyMatrix();
      status = SET_PROPERTY_SUCCEED;
    }
    else if (data == NULL && nbRow == -1 && nbCol == -1)
    {
      /* data allocation failed */
      Scierror(999, _("%s: No more memory."), "get_data_property") ;
      status = SET_PROPERTY_ERROR;
    }
		else if (data == NULL)
		{
			Scierror(999, _("'%s' property does not exist for this handle.\n"),"data");
			return -1;
		}
    else
    {
      status = sciReturnMatrix( data, nbRow, nbCol ) ;
      FREE( data ) ;
    }

    return status ;
  }
  
}
/*------------------------------------------------------------------------*/
