/*------------------------------------------------------------------------*/
/* file: set_user_data_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the user_data field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int set_user_data_property( sciPointObj * pobj, int stackPointer,  int valueType, int nbRow, int nbCol )
{

  /* set pobj->user_data*/
  int *  size_ptr                             ;
  int ** user_data_ptr                        ;
  int    data_size     = GetDataSize( 3 ) * 2 ; /* why 2 ?????? Jb.Silvy */
  int *  data_ptr      = GetData( 3 )         ;

  /* no type check here user_data can be anything */

  /* retrieve current user data matrix */
  sciGetPointerToUserData( pobj, &user_data_ptr, &size_ptr ) ;

  if ( nbRow * nbCol == 0 )
  {
    FREE( *user_data_ptr ) ;
    *user_data_ptr = NULL ;
    *size_ptr = 0 ;
    return SET_PROPERTY_SUCCEED ;
  }

  if( user_data_ptr == NULL )
  {
    *user_data_ptr = createIntArrayCopy( data_ptr, data_size ) ;
    *size_ptr      = data_size ;
  }
  else if( *size_ptr == data_size )
  {
    intArrayCopy( *user_data_ptr, data_ptr, data_size ) ;
  }
  else
  {
    FREE( *user_data_ptr ) ;
    *user_data_ptr = createIntArrayCopy( data_ptr, data_size ) ;
    *size_ptr      = data_size ;
  }

  if ( *user_data_ptr == NULL )
  {
    sciprint(_("%s: No more memory.\n"),"set_user_data_property") ;
    *size_ptr = 0 ;
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;

  /*if ( isParameterDoubleMatrix( 3 ) )
  { 
    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&numrow3,&numcol3,&l3);
    if (numrow3==0||numcol3==0) delete_user_data=1;
  }
  else if (VarType(3) == 0) delete_user_data=1;

  sciGetPointerToUserData (pobj,&user_data_ptr, &size_ptr);*/

}
/*------------------------------------------------------------------------*/
