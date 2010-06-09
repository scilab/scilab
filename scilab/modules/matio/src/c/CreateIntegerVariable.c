/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * Copyright (C) 2010 - INRIA - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateMatlabVariable.h"
#include "stack-c.h"
#include "api_scilab.h"

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

int CreateIntegerVariable(int iVar, int integerType, matvar_t *matVariable, int * parent, int item_position)
{
  int nbRow, nbCol, i;
  SciErr _SciErr;
  char * tmp_int8 = NULL;
  short * tmp_int16 = NULL;
  int * tmp_int32 = NULL;
  unsigned char * tmp_uint8 = NULL;
  unsigned short * tmp_uint16 = NULL;
  unsigned int * tmp_uint32 = NULL;
#ifdef __SCILAB_INT64__
  long long * tmp_int64 = NULL;
  unsigned long long * tmp_uint64 = NULL;
#endif

  // Matrix dimensions
  nbRow = matVariable->dims[0];
  nbCol = matVariable->dims[1];

  if (matVariable->rank==2) /* 2-D array */
    {
      switch(integerType)
	{
	case I_CHAR:
	  tmp_int8 = (char *)MALLOC(nbRow*nbCol*sizeof(char));
	  if (tmp_int8 == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
              return FALSE;
            }
	  for(i=0;i<nbRow*nbCol; i++) tmp_int8[i] = ((char *)matVariable->data)[i];

	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfInteger8(pvApiCtx, iVar, nbRow, nbCol, tmp_int8); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfInteger8InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_int8); MATIO_ERROR;
	    }

	  FREE(tmp_int8);
	  break;
	case I_INT16:
	  tmp_int16 = (short *)MALLOC(nbRow*nbCol*sizeof(short));
	  if (tmp_int16 == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
              return FALSE;
            }
	  for(i=0;i<nbRow*nbCol; i++) tmp_int16[i] = ((short *)matVariable->data)[i];

	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfInteger16(pvApiCtx, iVar, nbRow, nbCol, tmp_int16); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfInteger16InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_int16); MATIO_ERROR;
	    }

	  FREE(tmp_int16);
	  break;
	case I_INT32:
	  tmp_int32 = (int *)MALLOC(nbRow*nbCol*sizeof(int));
	  if (tmp_int32 == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
              return FALSE;
            }
	  for(i=0;i<nbRow*nbCol; i++) tmp_int32[i] = ((int *)matVariable->data)[i];

	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfInteger32(pvApiCtx, iVar, nbRow, nbCol, tmp_int32); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfInteger32InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_int32); MATIO_ERROR;
	    }

	  FREE(tmp_int32);
	  break;
#ifdef __SCILAB_INT64__
	case I_INT64:
	  tmp_int64 = (long long *)MALLOC(nbRow*nbCol*sizeof(long long));
	  if (tmp_int64 == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
              return FALSE;
            }
	  for(i=0;i<nbRow*nbCol; i++) tmp_int64[i] = ((long long *)matVariable->data)[i];

	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfInteger64(pvApiCtx, iVar, nbRow, nbCol, tmp_int64); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfInteger64InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_int64); MATIO_ERROR;
	    }

	  FREE(tmp_int64);
	  break;
#endif
	case I_UCHAR:
	  tmp_uint8 = (unsigned char *)MALLOC(nbRow*nbCol*sizeof(unsigned char));
	  if (tmp_uint8 == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
              return FALSE;
            }
	  for(i=0;i<nbRow*nbCol; i++) tmp_uint8[i] = ((unsigned char *)matVariable->data)[i];

	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfUnsignedInteger8(pvApiCtx, iVar, nbRow, nbCol, tmp_uint8); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_uint8); MATIO_ERROR;
	    }

	  FREE(tmp_uint8);
	  break;
	case I_UINT16:
	  tmp_uint16 = (unsigned short *)MALLOC(nbRow*nbCol*sizeof(unsigned short));
	  if (tmp_uint16 == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
              return FALSE;
            }
	  for(i=0;i<nbRow*nbCol; i++) tmp_uint16[i] = ((unsigned short *)matVariable->data)[i];

	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfUnsignedInteger16(pvApiCtx, iVar, nbRow, nbCol, tmp_uint16); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_uint16); MATIO_ERROR;
	    }

	  FREE(tmp_uint16);
	  break;
	case I_UINT32:
	  tmp_uint32 = (unsigned int *)MALLOC(nbRow*nbCol*sizeof(unsigned int));
	  if (tmp_uint32 == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
              return FALSE;
            }
	  for(i=0;i<nbRow*nbCol; i++) tmp_uint32[i] = ((unsigned int *)matVariable->data)[i];

	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfUnsignedInteger32(pvApiCtx, iVar, nbRow, nbCol, tmp_uint32); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfUnsignedInteger32InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_uint32); MATIO_ERROR;
	    }

	  FREE(tmp_uint32);
	  break;
#ifdef __SCILAB_INT64__
	case I_UINT64:
	  tmp_uint64 = (unsigned long long *)MALLOC(nbRow*nbCol*sizeof(unsigned long long));
	  if (tmp_uint64 == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
              return FALSE;
            }
	  for(i=0;i<nbRow*nbCol; i++) tmp_uint64[i] = ((unsigned long long *)matVariable->data)[i];

	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfUnsignedInteger64(pvApiCtx, iVar, nbRow, nbCol, tmp_uint64); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfUnsignedInteger64InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_uint64); MATIO_ERROR;
	    }

	  FREE(tmp_uint64);
	  break;
#endif
	}
    }
  else /* Multi-dimension array -> Scilab HyperMatrix */
    {
      CreateHyperMatrixVariable(iVar, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,  &integerType, &matVariable->rank, 
				matVariable->dims, matVariable->data, NULL, parent, item_position);
    }
  
  return TRUE;
}
